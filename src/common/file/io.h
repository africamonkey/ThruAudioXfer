#pragma once

#include <string>

#include "google/protobuf/message.h"

namespace io {

bool ReadFromProtoInTextFormat(const std::string& text_file,
                               google::protobuf::Message* proto);

bool WriteToProtoInTextFormat(const google::protobuf::Message& proto,
                              const std::string& text_file);

}  // namespace io
