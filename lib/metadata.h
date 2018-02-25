//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 24 2018
//  File : metadata.h
//******************************************

#ifndef CMUS_CLIENT_LIB_METADATA_H_
#define CMUS_CLIENT_LIB_METADATA_H_

#include <vector>

#include "tags.h"

namespace cmusclient {

struct Metadata {
  std::string filename;
  int duration = 0;
  std::string codec;
  int bitrate = 0;
  Tags tags;

  static std::vector<Metadata> ParseMetadataList(const std::string& str);
};

}  // namespace cmusclient

#endif  // CMUS_CLIENT_LIB_METADATA_H_
