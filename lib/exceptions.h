//******************************************
//  Author : Yuwei Huang   
//  Created On : Mon Feb 19 2018
//  File : exceptions.h
//******************************************

#ifndef CMUS_CLIENT_LIB_EXCEPTIONS_H_
#define CMUS_CLIENT_LIB_EXCEPTIONS_H_

#include <stdexcept>

namespace cmusclient {

class AuthenticationError : public std::runtime_error {
 public:
  AuthenticationError();
  ~AuthenticationError() throw() override;
};

class CommandError : public std::runtime_error {
 public:
  CommandError(const std::string& message);
  ~CommandError() throw() override;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_EXCEPTIONS_H_
