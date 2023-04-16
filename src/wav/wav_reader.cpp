// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "wav_reader.h"

#include <utility>

#include "glog/logging.h"

namespace wav {

WavReader::WavReader(std::string filename) : filename_(std::move(filename)), infile_() {
  infile_.open(filename_, std::ios::binary | std::ios::in);
  CHECK(infile_.is_open()) << filename_;
  CHECK(!infile_.fail()) << filename_;
  infile_.read(wav_header_.riff_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.riff_header), "RIFF");
  infile_.read((char *) &wav_header_.wav_size, 4);
  infile_.read(wav_header_.wave_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.wave_header), "WAVE");
  infile_.read(wav_header_.fmt_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.fmt_header), "fmt ");

  infile_.read((char *) &wav_header_.fmt_chunk_size, 4);
  CHECK_GE(wav_header_.fmt_chunk_size, 16);
  infile_.read((char *) &wav_header_.audio_format, 2);
  CHECK_EQ(wav_header_.audio_format, 1) << "Only support PCM.";
  infile_.read((char *) &wav_header_.num_channels, 2);
  CHECK_GE(wav_header_.num_channels, 1) << "num_channels should be either 1 or 2.";
  CHECK_LE(wav_header_.num_channels, 2) << "num_channels should be either 1 or 2.";
  infile_.read((char *) &wav_header_.sample_rate, 4);
  infile_.read((char *) &wav_header_.byte_rate, 4);
  infile_.read((char *) &wav_header_.sample_alignment, 2);
  infile_.read((char *) &wav_header_.bit_depth, 2);
  CHECK(wav_header_.bit_depth == 16 || wav_header_.bit_depth == 24 || wav_header_.bit_depth == 32)
          << "bit_depth = " << wav_header_.bit_depth << " should be either 16, 24, or 32.";
  CHECK_EQ(wav_header_.sample_alignment, wav_header_.num_channels * (wav_header_.bit_depth / 8));
  CHECK_EQ(wav_header_.byte_rate, wav_header_.sample_rate * wav_header_.sample_alignment);
  if (wav_header_.fmt_chunk_size > 16) {
    int extra_size = wav_header_.fmt_chunk_size - 16;
    std::vector<char> buffer(extra_size);
    infile_.read(buffer.data(), extra_size);
  }
  char next_header[4];
  int next_bytes = 0;
  memset(next_header, 0, sizeof(next_header));
  while (!infile_.eof() && HeaderCharsToString(next_header) != "data") {
    std::vector<char> buffer(next_bytes);
    infile_.read(buffer.data(), next_bytes);
    infile_.read(next_header, 4);
    infile_.read((char*) &next_bytes, 4);
  }
  memcpy(wav_header_.data_header, next_header, 4);
  CHECK_EQ(HeaderCharsToString(wav_header_.data_header), "data");
  wav_header_.data_bytes = next_bytes;
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

std::pair<double, double> WavReader::GetSample() {
  CHECK(!IsEof());
  const int sample_max = static_cast<int>((1ll << (wav_header_.bit_depth - 1)) - 1);
  int sample_0 = 0;
  int sample_1 = 0;
  infile_.read((char *) &sample_0, wav_header_.bit_depth / 8);
  num_read_bytes_ += wav_header_.bit_depth / 8;
  if (wav_header_.num_channels == 1) {
    sample_1 = sample_0;
  } else {
    infile_.read((char *) &sample_1, wav_header_.bit_depth / 8);
    num_read_bytes_ += wav_header_.bit_depth / 8;
  }
  if (sample_0 > sample_max) {
    sample_0 -= 1 << (wav_header_.bit_depth);
  }
  if (sample_1 > sample_max) {
    sample_1 -= 1 << (wav_header_.bit_depth);
  }
  return std::make_pair(std::clamp(1.0 * sample_0 / sample_max, -1.0, 1.0),
                        std::clamp(1.0 * sample_1 / sample_max, -1.0, 1.0));
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
