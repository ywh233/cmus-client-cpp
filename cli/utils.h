//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 24 2018
//  File : util.h
//******************************************

#include <cxxopts.hpp>
#include <iostream>
#include <memory>

#include "lib/cmus_client.h"
#include "lib/ip_connection_interface.h"
#include "lib/tags.h"
#include "lib/unix_connection_interface.h"

static void PrintTagLineIfExists(const std::string& tag_name,
                                 const std::string& text) {
  if (text.empty()) {
    return;
  }
  std::cout << "tag " << tag_name << " " << text << std::endl;
}

static void PrintTags(const cmusclient::Tags& tags) {
  PrintTagLineIfExists("title", tags.title);
  PrintTagLineIfExists("artist", tags.artist);
  PrintTagLineIfExists("album", tags.album);
  PrintTagLineIfExists("tracknumber", tags.tracknumber);
  PrintTagLineIfExists("date", tags.date);
  PrintTagLineIfExists("genre", tags.genre);
  PrintTagLineIfExists("comment", tags.comment);
}

static void ShowHelp(const cxxopts::Options& options, int exit_code) {
  std::cout << options.help({""}) << std::endl;
  exit(exit_code);
}

static std::unique_ptr<cxxopts::ParseResult> ParseArgsAndConnect(
    cxxopts::Options* options,
    int argc,
    char** argv,
    std::unique_ptr<cmusclient::ConnectionInterface>* out_interface,
    std::string* out_passwd) {
  options->add_options()(
      "hostname",
      "Host name of the server that runs cmus. Must be used with the port.",
      cxxopts::value<std::string>(),
      "HOSTNAME")
      ("port", "Port to listen to.", cxxopts::value<std::string>(), "PORT")
      ("passwd", "Password.", cxxopts::value<std::string>(), "PASSWD")
      ("h,help", "Print help");

  std::unique_ptr<cxxopts::ParseResult> result;
  try {
    result.reset(new cxxopts::ParseResult(options->parse(argc, argv)));

    // Help
    if (result->count("h")) {
      ShowHelp(*options, 0);
    }

    try {
      if (result->count("hostname")) {
        if (!result->count("port")) {
          std::cerr << "--hostname and --port must both be set." << std::endl;
          ShowHelp(*options, 1);
        }

        std::string hostname = (*result)["hostname"].as<std::string>();
        std::string port = (*result)["port"].as<std::string>();
        out_interface->reset(
            new cmusclient::IpConnectionInterface(hostname, port));
      } else {
        out_interface->reset(new cmusclient::UnixConnectionInterface());
      }
    } catch (const std::system_error& err) {
      std::cerr << "Failed to open connection: " << err.what() << std::endl;
      exit(1);
    }

    if (result->count("passwd") && out_passwd) {
      *out_passwd = (*result)["passwd"].as<std::string>();
    }
  } catch (const cxxopts::OptionException& e) {
    std::cerr << "Error parsing options: " << e.what() << std::endl;
    ShowHelp(*options, 1);
  }

  return result;
}
