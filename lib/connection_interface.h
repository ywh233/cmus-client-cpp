//******************************************
//  Author : Yuwei Huang   
//  Created On : Sun Feb 18 2018
//  File : connection_interface.h
//******************************************

#ifndef CMUS_CLIENT_LIB_CONNECTION_INTERFACE_H_
#define CMUS_CLIENT_LIB_CONNECTION_INTERFACE_H_

#include <string>

namespace cmusclient {

class ConnectionInterface {
 public:
  virtual ~ConnectionInterface() {}

  virtual void Send(const std::string& message) = 0;

  virtual std::string Receive() = 0;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_CONNECTION_INTERFACE_H_
