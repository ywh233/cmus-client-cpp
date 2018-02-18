//******************************************
//  Author : Yuwei Huang   
//  Created On : Sun Feb 18 2018
//  File : ip_connection_interface.cc
//******************************************

#include "ip_connection_interface.h"

namespace cmusclient {

IpConnectionInterface::IpConnectionInterface(const std::string& host_name,
                                             const std::string& port) {
  asio::connect(socket_, resolver_.resolve({host_name, port}));
}

IpConnectionInterface::~IpConnectionInterface() {}

void IpConnectionInterface::Send(const std::string& message) {
  socket_.send(asio::buffer(message.data(), message.length()));
}

std::string IpConnectionInterface::Receive() {
  socket_.receive(asio::buffer(receive_buffer_, kReceiveBufferSize));
  return std::string(receive_buffer_, kReceiveBufferSize);
}

}  // namespace cmusclient
