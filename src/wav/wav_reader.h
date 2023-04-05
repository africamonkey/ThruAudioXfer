// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#pragma once

#include <fstream>
#include <functional>

#include "src/wav/wav_header.h"

namespace wav {

class WavReader {
 public:
  explicit WavReader(std::string filename);
  ~WavReader();

  void Close();
  bool IsEof() const;
  // Sample data should be in [-1, 1].
  // Remember to check IsEof() before calling.
  double GetSample();
  WavHeader GetWavHeader() const;

 private:
  static std::string HeaderCharsToString(char header[4]);

  std::string filename_;
  std::ifstream infile_;
  WavHeader wav_header_;
  size_t num_read_bytes_;
};

}  // namespace wav
