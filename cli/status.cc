//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 18 2018
//  File : status.cc
//******************************************

#include <cxxopts.hpp>

#include <iostream>
#include <memory>

#include "lib/cmus_client.h"
#include "lib/exceptions.h"

#include "utils.h"

using namespace cmusclient;

void PrintStatus(std::unique_ptr<ConnectionInterface>&& interface,
                 const std::string& passwd) {
  try {
    CmusClient client(std::move(interface), passwd);

    Status status = client.GetStatus();
    std::cout << "status ";
    switch (status.status) {
      case Status::PlayerStatus::STOPPED:
        std::cout << "stopped";
        break;
      case Status::PlayerStatus::PLAYING:
        std::cout << "playing";
        break;
      case Status::PlayerStatus::PAUSED:
        std::cout << "paused";
        break;
      default:
        std::cerr << "Unknown player status.";
        exit(1);
    }
    std::cout << std::endl;

    std::cout << "file " << status.filename << std::endl
              << "duration " << status.duration << std::endl
              << "position " << status.position << std::endl;

    PrintTags(status.tags);

    for (const auto& kv : status.settings) {
      std::cout << "set " << kv.first << " " << kv.second << std::endl;
    }
  } catch (const AuthenticationError& err) {
    std::cerr << err.what() << std::endl;
  }
}

int main(int argc, char** argv) {
  cxxopts::Options options(
      argv[0], "cmus status printer. Similar to \"cmus-remote -Q\"");

  std::string passwd;
  std::unique_ptr<ConnectionInterface> interface;
  ParseArgsAndConnect(&options, argc, argv, &interface, &passwd);
  PrintStatus(std::move(interface), passwd);

  return 0;
}
