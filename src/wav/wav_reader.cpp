// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "wav_reader.h"

#include <utility>

#include "glog/logging.h"

namespace wav {

WavReader::WavReader(std::string filename) : filename_(std::move(filename)), infile_() {
  infile_ = std::ifstream(filename_, std::ios::binary);
  CHECK(!infile_.fail());
  infile_.read(wav_header_.riff_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.riff_header), "RIFF");
  infile_.read((char*)&wav_header_.wav_size, 4);
  infile_.read(wav_header_.wave_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.wave_header), "WAVE");
  infile_.read(wav_header_.fmt_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.fmt_header), "fmt ");

  infile_.read((char*)&wav_header_.fmt_chunk_size, 4);
  CHECK_GE(wav_header_.fmt_chunk_size, 16);
  infile_.read((char*)&wav_header_.audio_format, 2);
  CHECK_EQ(wav_header_.audio_format, 1) << "Only support PCM.";
  infile_.read((char*)&wav_header_.num_channels, 2);
  infile_.read((char*)&wav_header_.sample_rate, 4);
  infile_.read((char*)&wav_header_.byte_rate, 4);
  infile_.read((char*)&wav_header_.sample_alignment, 2);
  infile_.read((char*)&wav_header_.bit_depth, 2);
  if (wav_header_.fmt_chunk_size > 16) {
    int extra_size = wav_header_.fmt_chunk_size - 16;
    std::vector<char> buffer(extra_size);
    infile_.read(buffer.data(), extra_size);
  }
  infile_.read(wav_header_.data_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.data_header), "data");
  infile_.read((char*)&wav_header_.data_bytes, 4);
  num_read_bytes_ = 0;
}

WavReader::~WavReader() {
  Close();
}

void WavReader::Close() {
  if (infile_.is_open()) {
    infile_.close();
  }
}

bool WavReader::IsEof() const {
  return !infile_.is_open() || infile_.eof() || num_read_bytes_ >= wav_header_.data_bytes;
}

double WavReader::GetSample() {
  return 0.0;
}

WavHeader WavReader::GetWavHeader() const {
  return wav_header_;
}

std::string WavReader::HeaderCharsToString(char header[4]) {
  std::string header_string;
  for (int i = 0; i < 4; ++i) {
    header_string += header[i];
  }
  return header_string;
}

}  // namespace wav
