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
#include "string_buffer.h"

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
  asio::io_service service_;
  asio::local::stream_protocol::socket socket_{service_};

  StringBuffer receive_buffer_;

  UnixConnectionInterface(const UnixConnectionInterface&) = delete;
  UnixConnectionInterface(UnixConnectionInterface&&) = delete;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_UNIX_CONNECTION_INTERFACE_H_
