//******************************************
//  Author : Author   
//  Created On : Sun Feb 18 2018
//  File : unix_connection_interface.cc.cc
//******************************************

#include "unix_connection_interface.h"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

namespace cmusclient {

namespace {

const char kCmusDefaultSocketPathToHome[] = "/.config/cmus/socket";

std::string GetCmusDefaultSocketPath() {
  const char* home_dir;

  if ((home_dir = getenv("HOME")) == NULL) {
    home_dir = getpwuid(getuid())->pw_dir;
  }

  return std::string(home_dir) + kCmusDefaultSocketPathToHome;
}

}  // namespace

UnixConnectionInterface::UnixConnectionInterface()
  : UnixConnectionInterface(GetCmusDefaultSocketPath()) {}

UnixConnectionInterface::UnixConnectionInterface(
    const std::string& socket_path) {
  asio::local::stream_protocol::endpoint endpoint(socket_path);
  socket_.connect(endpoint);
}

UnixConnectionInterface::~UnixConnectionInterface() {}

void UnixConnectionInterface::Send(const std::string& message) {
  socket_.send(asio::buffer(message.data(), message.length()));
}

std::string UnixConnectionInterface::Receive() {
  socket_.receive(receive_buffer_.GetBuffer());
  return receive_buffer_.GetString();
}

}  // namespace cmusclient
