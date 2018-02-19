//******************************************
//  Author : Yuwei Huang
//  Created On : Mon Feb 19 2018
//  File : exceptions.cc
//******************************************

#include "exceptions.h"

namespace cmusclient {

AuthenticationError::AuthenticationError()
    : std::runtime_error("Authentication failed") {}

AuthenticationError::~AuthenticationError() throw() {}

CommandError::CommandError(const std::string& message)
    : std::runtime_error(message) {}

CommandError::~CommandError() throw() {}

}  // namespace cmusclient
