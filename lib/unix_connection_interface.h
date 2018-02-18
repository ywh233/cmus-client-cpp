//******************************************
//  Author : Yuwei Huang   
//  Created On : Sun Feb 18 2018
//  File : unix_connection_interface.h
//******************************************

#ifndef CMUS_CLIENT_LIB_UNIX_CONNECTION_INTERFACE_H_
#define CMUS_CLIENT_LIB_UNIX_CONNECTION_INTERFACE_H_

#include <asio.hpp>
#include <asio/local/stream_protocol.hpp>

#include "connection_interface.h"

namespace cmusclient {

class UnixConnectionInterface : public ConnectionInterface {
 public:
  // Constructor will throw exception when the connection fails to establish.
  // Default constructor uses the default socket path of cmus.
  UnixConnectionInterface();

  UnixConnectionInterface(const std::string& socket_path);

  ~UnixConnectionInterface() override;

  // ConnectionInterface overrides.
  void Send(const std::string& message) override;
  std::string Receive() override;

 private:
  static constexpr int kReceiveBufferSize = 4096;

  asio::io_service service_;
  asio::local::stream_protocol::socket socket_{service_};

  char receive_buffer_[kReceiveBufferSize];

  UnixConnectionInterface(const UnixConnectionInterface&) = delete;
  UnixConnectionInterface(UnixConnectionInterface&&) = delete;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_UNIX_CONNECTION_INTERFACE_H_
