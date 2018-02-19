//******************************************
//  Author : Yuwei Huang   
//  Created On : Sun Feb 18 2018
//  File : ip_connection_interface.h
//******************************************

#ifndef CMUS_CLIENT_LIB_IP_CONNECTION_INTERFACE_H_
#define CMUS_CLIENT_LIB_IP_CONNECTION_INTERFACE_H_

#include <asio.hpp>
#include <asio/ip/tcp.hpp>

#include "connection_interface.h"
#include "string_buffer.h"

namespace cmusclient {

class IpConnectionInterface : public ConnectionInterface {
 public:
  IpConnectionInterface(const std::string& host_name, const std::string& port);
  ~IpConnectionInterface() override;

  // ConnectionInterface overrides.
  void Send(const std::string& message) override;
  std::string Receive() override;

 private:
  asio::io_service service_;
  asio::ip::tcp::socket socket_{service_};
  asio::ip::tcp::resolver resolver_{service_};

  StringBuffer receive_buffer_;

  IpConnectionInterface(const IpConnectionInterface&) = delete;
  IpConnectionInterface(IpConnectionInterface&&) = delete;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_IP_CONNECTION_INTERFACE_H_
