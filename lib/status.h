//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 18 2018
//  File : status.h
//******************************************

#ifndef CMUS_CLIENT_LIB_STATUS_H_
#define CMUS_CLIENT_LIB_STATUS_H_

#include <map>
#include <string>

namespace cmusclient {

struct Status {
  enum class PlayerStatus {
    STOPPED,
    PLAYING,
    PAUSED,
  };

  struct Tags {
    std::string album;
    std::string artist;
    std::string comment;
    std::string date;
    std::string genre;
    std::string title;
    std::string tracknumber;
  };

  static Status ParseStatus(const std::string& str);

  PlayerStatus status = PlayerStatus::STOPPED;
  std::string filename;
  int duration = 0;
  int position = 0;

  Tags tags;

  std::map<std::string, std::string> settings;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_STATUS_H_
