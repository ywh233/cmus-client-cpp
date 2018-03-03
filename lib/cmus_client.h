//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 16 2018
//  File : cmus_client.h
//******************************************

#ifndef CMUS_CLIENT_LIB_CMUS_CLIENT_H_
#define CMUS_CLIENT_LIB_CMUS_CLIENT_H_

#include <memory>

#include "connection_interface.h"
#include "metadata.h"
#include "status.h"

namespace cmusclient {

class CmusClient {
 public:
  enum class View {
    LIBRARY = 1,
    SORTED_LIBRARY = 2,
    PLAYLIST = 3,
    PLAY_QUEUE = 4,
    BROWSER = 5,
    FILTERS = 6,
    SETTINGS = 7,
  };

  enum class MetadataListSource {
    LIBRARY,
    FILTERED_LIBRARY,
    PLAYLIST,
    QUEUE,
  };

  // Connects to the default unix socket, i.e. ~/.config/cmus/socket
  CmusClient();

  // Connects to the player via TCP/IP.
  CmusClient(const std::string& hostname,
             const std::string& port,
             const std::string& password);

  // Connects to the player using a custom connection interface.
  // Throws |AuthenticationError| if the password is invalid.
  explicit CmusClient(std::unique_ptr<ConnectionInterface>&& interface,
                      const std::string& password);
  ~CmusClient();

  Status GetStatus();

  void GoToView(View view);

  std::vector<Metadata> GetMetadataList(MetadataListSource source);

  void Play();
  void Pause();
  void Stop();
  void Next();
  void Previous();

  // Selects a track on current view that has the matching string.
  void Search(const std::string& str);

  // Presses Enter on current view.
  void Activate();

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
