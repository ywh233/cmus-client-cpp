//******************************************
//  Author : Yuwei Huang   
//  Created On : Mon Feb 19 2018
//  File : string_buffer.h
//******************************************

#ifndef CMUS_CLIENT_LIB_STRING_BUFFER_H_
#define CMUS_CLIENT_LIB_STRING_BUFFER_H_

#include <asio.hpp>

namespace cmusclient {

class StringBuffer {
 public:
  StringBuffer();
  ~StringBuffer();

  asio::mutable_buffers_1 GetBuffer();
  char* GetBufferData();
  size_t GetStringLength() const;
  std::string GetString() const;

 private:
  static constexpr size_t kBufferSize = 4096;

  char buffer_[kBufferSize];

  StringBuffer(const StringBuffer&) = delete;
  StringBuffer(StringBuffer&&) = delete;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_STRING_BUFFER_H_
