//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 18 2018
//  File : status.h
//******************************************

#ifndef CMUS_CLIENT_LIB_STATUS_H_
#define CMUS_CLIENT_LIB_STATUS_H_

#include <map>
#include <string>

#include "tags.h"

namespace cmusclient {

struct Status {
  enum class PlayerStatus {
    STOPPED,
    PLAYING,
    PAUSED,
  };

  const unsigned int kMaxVolume = 100u;

  static Status ParseStatus(const std::string& str);

  unsigned int GetLeftVolume() const;
  unsigned int GetRightVolume() const;

  PlayerStatus status = PlayerStatus::STOPPED;
  std::string filename;
  unsigned int duration = 0;
  unsigned int position = 0;

  Tags tags;

  std::map<std::string, std::string> settings;
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_STATUS_H_
