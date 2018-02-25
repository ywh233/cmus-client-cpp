//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 18 2018
//  File : status.cc
//******************************************

#include "status.h"

#include <regex>
#include <sstream>

namespace cmusclient {

namespace {

// G1: status
const std::regex kStatusExp(R"exp(^status (.*)$)exp");

// G1: filename
const std::regex kFileExp(R"exp(^file (.*)$)exp");

// G1: Duration
const std::regex kDurationExp(R"exp(^duration (.*)$)exp");

// G1: Position
const std::regex kPositionExp(R"exp(^position (.*)$)exp");

// G1: Key, G2: Value
const std::regex kSettingExp(R"exp(^set (\S+) (.*)$)exp");

void SetPlayerStatus(Status* status, const std::string& player_status) {
  if (player_status == "stopped") {
    status->status = Status::PlayerStatus::STOPPED;
  } else if (player_status == "playing") {
    status->status = Status::PlayerStatus::PLAYING;
  } else if (player_status == "paused") {
    status->status = Status::PlayerStatus::PAUSED;
  }
}

}  // namespace

Status Status::ParseStatus(const std::string& str) {
  std::istringstream stream(str);
  std::string line;
  Status status;

  while (std::getline(stream, line)) {
    std::smatch status_match;
    if (std::regex_search(line, status_match, kStatusExp)) {
      SetPlayerStatus(&status, status_match[1]);
      continue;
    }

    std::smatch file_match;
    if (std::regex_search(line, file_match, kFileExp)) {
      status.filename = file_match[1];
      continue;
    }

    std::smatch duration_match;
    if (std::regex_search(line, duration_match, kDurationExp)) {
      status.duration = std::stoi(duration_match[1]);
      continue;
    }

    std::smatch position_match;
    if (std::regex_search(line, position_match, kPositionExp)) {
      status.position = std::stoi(position_match[1]);
      continue;
    }

    std::smatch setting_match;
    if (std::regex_search(line, setting_match, kSettingExp)) {
      status.settings[setting_match[1]] = setting_match[2];
    }
  }
  status.tags = Tags::ParseTags(str);
  return status;
}

}  // namespace cmusclient
