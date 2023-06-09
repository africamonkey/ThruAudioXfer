// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/common/file/io.h"

#include <filesystem>
#include <fstream>

#include "google/protobuf/text_format.h"

namespace io {

bool ReadFromProtoInTextFormat(const std::string& text_file,
                               google::protobuf::Message* proto) {
  std::ifstream input(text_file.data());
  if (input.fail()) {
    return false;
  }
  std::string file_contents((std::istreambuf_iterator<char>(input)),
                            std::istreambuf_iterator<char>());

  return google::protobuf::TextFormat::ParseFromString(file_contents, proto);
}

bool WriteToProtoInTextFormat(const google::protobuf::Message& proto,
                              const std::string& text_file) {
  std::string output_string;
  google::protobuf::TextFormat::PrintToString(proto, &output_string);

  std::ofstream output(text_file.data());
  if (output.fail()) {
    return false;
  }
  output << output_string;
  return true;
}

std::string GenerateTestFolder() {
  std::string default_test_folder = "/var/tmp/africamonkey";
  const char* test_folder_p = std::getenv("TEST_FOLDER");
  if (test_folder_p != nullptr) {
    default_test_folder = test_folder_p;
  }
  CreateFolderRecursively(default_test_folder);
  return default_test_folder;
}

void CreateFolderRecursively(const std::string& path) {
  std::filesystem::create_directories(path);
}

void DeleteFileIfExists(const std::string& file) {
  if (std::filesystem::exists(file)) {
    std::filesystem::remove(file);
  }
}

}  // namespace io
