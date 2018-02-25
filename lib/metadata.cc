//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 24 2018
//  File : metadata.cc
//******************************************

#include "metadata.h"

#include <regex>
#include <sstream>

namespace cmusclient {

namespace {

// G1: Key, G2: Value
const std::regex kFileExp(R"exp(^file (.*)$)exp");

// G1: Duration
const std::regex kDurationExp(R"exp(^duration (.*)$)exp");

// G1: Codec
const std::regex kCodecExp(R"exp(^codec (.*)$)exp");

// G1: Bitrate
const std::regex kBitrateExp(R"exp(^bitrate (.*)$)exp");

}  // namespace

// static
std::vector<Metadata> Metadata::ParseMetadataList(const std::string& str) {
  std::istringstream stream(str);
  std::string line;

  std::vector<Metadata> metadata_list;
  Metadata metadata;

  while (std::getline(stream, line)) {
    std::smatch file_match;
    if (std::regex_search(line, file_match, kFileExp)) {
      if (!metadata.filename.empty()) {
        metadata_list.push_back(metadata);
        metadata = Metadata();
      }
      metadata.filename = file_match[1];
      continue;
    }

    if (Tags::ParseOneLineTag(line, &metadata.tags)) {
      continue;
    }

    std::smatch duration_match;
    if (std::regex_search(line, duration_match, kDurationExp)) {
      metadata.duration = std::stoi(duration_match[1]);
      continue;
    }

    std::smatch codec_match;
    if (std::regex_search(line, codec_match, kCodecExp)) {
      metadata.codec = codec_match[1];
      continue;
    }

    std::smatch bitrate_match;
    if (std::regex_search(line, bitrate_match, kBitrateExp)) {
      metadata.bitrate = std::stoi(bitrate_match[1]);
      continue;
    }
  }

  if (!metadata.filename.empty()) {
    metadata_list.push_back(metadata);
  }
  return metadata_list;
}

}  // namespace cmusclient
