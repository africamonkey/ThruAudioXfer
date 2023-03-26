// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#pragma once

#include <string>

#include "google/protobuf/message.h"

#ifndef MUST_USE_RESULT
#define MUST_USE_RESULT __attribute__((warn_unused_result))
#endif

namespace io {

MUST_USE_RESULT bool ReadFromProtoInTextFormat(const std::string &text_file,
                                               google::protobuf::Message *proto);

MUST_USE_RESULT bool WriteToProtoInTextFormat(const google::protobuf::Message &proto,
                                              const std::string &text_file);

void DeleteFileIfExists(const std::string& file);

}  // namespace io

#undef MUST_USE_RESULT
