//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 18 2018
//  File : status.cc
//******************************************

#include <cxxopts.hpp>

#include <iostream>
#include <memory>
#include <system_error>

#include "lib/cmus_client.h"
#include "lib/exceptions.h"
#include "lib/ip_connection_interface.h"
#include "lib/unix_connection_interface.h"

using namespace cmusclient;

void PrintStatus(std::unique_ptr<ConnectionInterface>&& interface,
                 const std::string& passwd) {
  try {
    CmusClient client(std::move(interface), passwd);

    Status status = client.GetStatus();
    std::cout << "file: " << status.filename << std::endl
              << "duration: " << status.duration << std::endl
              << "position: " << status.position << std::endl
              << "tag title: " << status.tags.title << std::endl
              << "tag artist: " << status.tags.artist << std::endl
              << "tag album: " << status.tags.album << std::endl
              << "tag tracknumber: " << status.tags.tracknumber << std::endl
              << "tag date: " << status.tags.date << std::endl
              << "tag genre: " << status.tags.genre << std::endl
              << "tag comment: " << status.tags.comment << std::endl;

    for (const auto& kv : status.settings) {
      std::cout << "set " << kv.first << " " << kv.second << std::endl;
    }
  } catch (const AuthenticationError& err) {
    std::cerr << err.what() << std::endl;
  }
}

void ShowHelp(const cxxopts::Options& options, int exit_code) {
  std::cout << options.help({""}) << std::endl;
  exit(exit_code);
}

int main(int argc, char** argv) {
  cxxopts::Options options(
      argv[0], "cmus status printer. Similar to \"cmus-remote -Q\"");
  options.add_options()
      ("hostname",
          "Host name of the server that runs cmus. Must be used with the port.",
          cxxopts::value<std::string>(), "HOSTNAME")
      ("port", "Port to listen to.", cxxopts::value<std::string>(), "PORT")
      ("passwd", "Password.",cxxopts::value<std::string>(), "PASSWD")
      ("h,help", "Print help");

  std::unique_ptr<ConnectionInterface> interface;
  std::string passwd;

  try {
    auto result = options.parse(argc, argv);

    // Help
    if (result.count("h")) {
      ShowHelp(options, 0);
    }

    try {
      if (result.count("hostname")) {
        if (!result.count("port")) {
          std::cerr << "--hostname and --port must both be set." << std::endl;
          ShowHelp(options, 1);
        }

        std::string hostname = result["hostname"].as<std::string>();
        std::string port = result["port"].as<std::string>();
        interface.reset(new IpConnectionInterface(hostname, port));
      } else {
        interface.reset(new UnixConnectionInterface());
      }
    } catch (const std::system_error& err) {
      std::cerr << "Failed to open connection: " << err.what() << std::endl;
      exit(1);
    }

    if (result.count("passwd")) {
      passwd = result["passwd"].as<std::string>();
    }

  } catch (const cxxopts::OptionException& e) {
    std::cerr << "Error parsing options: " << e.what() << std::endl;
    ShowHelp(options, 1);
  }

  PrintStatus(std::move(interface), passwd);

  return 0;
}
