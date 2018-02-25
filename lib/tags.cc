//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 24 2018
//  File : tags.cc
//******************************************

#include "tags.h"

#include <regex>
#include <sstream>

namespace cmusclient {

namespace {

// G1: Key, G2: Value
const std::regex kTagExp(R"exp(^tag (\S+) (.*)$)exp");

void SetTag(Tags* tags, const std::string& key, const std::string& value) {
  if (key == "album") {
    tags->album = value;
  } else if (key == "artist") {
    tags->artist = value;
  } else if (key == "comment") {
    tags->comment = value;
  } else if (key == "date") {
    tags->date = value;
  } else if (key == "genre") {
    tags->genre = value;
  } else if (key == "title") {
    tags->title = value;
  } else if (key == "tracknumber") {
    tags->tracknumber = value;
  }
}

}  // namespace

// static
Tags Tags::ParseTags(const std::string& str) {
  std::istringstream stream(str);
  std::string line;
  Tags tags;

  while (std::getline(stream, line)) {
    std::smatch tag_match;
    if (std::regex_search(line, tag_match, kTagExp)) {
      SetTag(&tags, tag_match[1], tag_match[2]);
    }
  }
  return tags;
}

}  // namespace cmusclient
