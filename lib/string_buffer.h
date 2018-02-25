//******************************************
//  Author : Yuwei Huang
//  Created On : Mon Feb 19 2018
//  File : string_buffer.h
//******************************************

#ifndef CMUS_CLIENT_LIB_STRING_BUFFER_H_
#define CMUS_CLIENT_LIB_STRING_BUFFER_H_

#include <asio.hpp>

#include <stdexcept>

namespace cmusclient {

class StringBuffer {
 public:
  StringBuffer() = default;
  ~StringBuffer() = default;

  template <typename SocketType>
  std::string ReadFromSocket(SocketType* socket) {
    std::string output;
    auto buffer = asio::buffer(buffer_, kBufferSize);
    while (true) {
      size_t received_length = socket->receive(buffer);
      if (received_length == 0) {
        throw std::runtime_error("Unexpected EOL");
      }

      output += std::string(buffer_, received_length);

      // cmus reply has two ending pattern, single \n character or two
      // consecutive \n's.
      if (output.length() == 1) {
        if (output.front() == '\n') {
          break;
        }
        continue;
      }

      assert(output.length() > 1);
      auto iter = output.cend();
      if (*(iter - 1) == '\n' && *(iter - 2) == '\n') {
        break;
      }
    }
    return output;
  }

 private:
  static constexpr size_t kBufferSize = 4096;

  char buffer_[kBufferSize];

  StringBuffer(const StringBuffer&) = delete;
  StringBuffer(StringBuffer&&) = delete;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_STRING_BUFFER_H_
