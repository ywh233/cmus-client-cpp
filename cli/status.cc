//******************************************
//  Author : Yuwei Huang   
//  Created On : Sun Feb 18 2018
//  File : status.cc
//******************************************

#include <memory>

#include <iostream>
#include <system_error>

#include "lib/cmus_client.h"
#include "lib/unix_connection_interface.h"

using namespace cmusclient;

int main() {
  std::unique_ptr<UnixConnectionInterface> interface;
  try {
    interface.reset(new UnixConnectionInterface());
  } catch(const std::system_error& err) {
    std::cerr << "Failed to open connection: " << err.what() << std::endl;
    return 1;
  }
  CmusClient client(std::move(interface));

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
  return 0;
}
