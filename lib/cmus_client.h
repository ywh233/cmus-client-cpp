//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 16 2018
//  File : cmus_client.h
//******************************************

#ifndef CMUS_CLIENT_LIB_CMUS_CLIENT_H_
#define CMUS_CLIENT_LIB_CMUS_CLIENT_H_

#include <memory>

#include "connection_interface.h"
#include "status.h"

namespace cmusclient {

class CmusClient {
 public:
  // Throws |AuthenticationError| if the password is invalid.
  explicit CmusClient(std::unique_ptr<ConnectionInterface>&& interface,
                      const std::string& password);
  ~CmusClient();

  Status GetStatus();

  void Play();
  void Pause();
  void Stop();
  void Next();
  void Previous();

  // |position|: e.g. 10 (seconds), or 1:15.
  void Seek(const std::string& position);

  void SetValue(const std::string& key, const std::string& value);
  void ToggleValue(const std::string& key);

  // |volume|: e.g. 50, 50%, 100%, +10%, -10%.
  void SetVolume(const std::string& volume);

 private:
  std::string SendCommandAndGetReply(const std::string& command);
  void ProcessCommand(const std::string& command);

  std::unique_ptr<ConnectionInterface> interface_;

  CmusClient(const CmusClient&) = delete;
  CmusClient(CmusClient&&) = delete;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_CMUS_CLIENT_H_
