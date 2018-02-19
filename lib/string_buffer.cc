//******************************************
//  Author : Yuwei Huang
//  Created On : Mon Feb 19 2018
//  File : string_buffer.cc
//******************************************

#include "string_buffer.h"

#include <string.h>

namespace cmusclient {

StringBuffer::StringBuffer() {}

StringBuffer::~StringBuffer() {}

asio::mutable_buffers_1 StringBuffer::GetBuffer() {
  return asio::buffer(buffer_, kBufferSize);
}

char* StringBuffer::GetBufferData() {
  return buffer_;
}

size_t StringBuffer::GetStringLength() const {
  return strnlen(buffer_, kBufferSize);
}

std::string StringBuffer::GetString() const {
  return std::string(buffer_, GetStringLength());
}

}  // namespace cmusclient
