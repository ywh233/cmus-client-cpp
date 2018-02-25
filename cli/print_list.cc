//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 24 2018
//  File : print_list.cc
//******************************************

#include <cxxopts.hpp>

#include <iostream>
#include <memory>

#include "lib/cmus_client.h"
#include "lib/exceptions.h"

#include "utils.h"

using namespace cmusclient;

void PrintList(std::unique_ptr<ConnectionInterface>&& interface,
               const std::string& passwd,
               CmusClient::View view) {
  try {
    CmusClient client(std::move(interface), passwd);

    std::vector<Metadata> list = client.GetList(view);

    for (const auto& data : list) {
      std::cout << "file " << data.filename << std::endl
                << "duration " << data.duration << std::endl
                << "codec " << data.codec << std::endl
                << "bitrate " << data.bitrate << std::endl;
      PrintTags(data.tags);
    }
  } catch (const AuthenticationError& err) {
    std::cerr << err.what() << std::endl;
  }
}

int main(int argc, char** argv) {
  cxxopts::Options options(
      argv[0],
      "List files in cmus, similar to \"cmus-remote -C save -e -l -\".");
  options.add_options()
      ("l", "Print library views")
      ("L", "Print filtered library views")
      ("p", "Print playlist view")
      ("q", "Print queue view");

  std::string passwd;
  std::unique_ptr<ConnectionInterface> interface;
  auto parse_result =
      ParseArgsAndConnect(&options, argc, argv, &interface, &passwd);
  CmusClient::View view = CmusClient::View::LIBRARY;
  if (parse_result->count("l")) {
    view = CmusClient::View::LIBRARY;
  } else if (parse_result->count("L")) {
    view = CmusClient::View::FILTERED_LIBRARY;
  } else if (parse_result->count("p")) {
    view = CmusClient::View::PLAYLIST;
  } else if (parse_result->count("q")) {
    view = CmusClient::View::QUEUE;
  } else {
    ShowHelp(options, 1);
  }
  PrintList(std::move(interface), passwd, view);

  return 0;
}
