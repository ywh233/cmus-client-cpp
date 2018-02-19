//******************************************
//  Author : Yuwei Huang
//  Created On : Sun Feb 18 2018
//  File : status.cc
//******************************************

#include "status.h"

#include <regex>
#include <sstream>

#include <iostream>

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
const std::regex kTagExp(R"exp(^tag (\S+) (.*)$)exp");

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

void SetTag(Status* status, const std::string& key, const std::string& value) {
  if (key == "album") {
    status->tags.album = value;
  } else if (key == "artist") {
    status->tags.artist = value;
  } else if (key == "comment") {
    status->tags.comment = value;
  } else if (key == "date") {
    status->tags.date = value;
  } else if (key == "genre") {
    status->tags.genre = value;
  } else if (key == "title") {
    status->tags.title = value;
  } else if (key == "tracknumber") {
    status->tags.tracknumber = value;
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

    std::smatch tag_match;
    if (std::regex_search(line, tag_match, kTagExp)) {
      SetTag(&status, tag_match[1], tag_match[2]);
      continue;
    }

    std::smatch setting_match;
    if (std::regex_search(line, setting_match, kSettingExp)) {
      status.settings[setting_match[1]] = setting_match[2];
    }
  }
  return status;
}

}  // namespace cmusclient
