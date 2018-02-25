//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 24 2018
//  File : tags.h
//******************************************

#ifndef CMUS_CLIENT_LIB_TAGS_H_
#define CMUS_CLIENT_LIB_TAGS_H_

#include <string>

namespace cmusclient {

struct Tags {
  std::string album;
  std::string artist;
  std::string comment;
  std::string date;
  std::string genre;
  std::string title;
  std::string tracknumber;

  // Parses one line and writes it to tags if it is a valid tag. Returns true if
  // the line is a valid tag.
  static bool ParseOneLineTag(const std::string& line, Tags* tags);
};

}

#endif  // CMUS_CLIENT_LIB_TAGS_H_
