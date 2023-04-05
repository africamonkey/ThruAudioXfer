// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/wav/wav_writer.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "glog/logging.h"

#include "src/common/math/math_utils.h"

namespace wav {

WavWriter::WavWriter(std::string filename, const interface::WavParams &wav_params)
    : filename_(std::move(filename)),
      outfile_(),
      wav_header_(WavHeader()),
      sample_count_(0),
      is_finish_writing_(false) {
  // Open the output file for writing in binary mode
  outfile_ = std::ofstream(filename_, std::ios::binary);
  CHECK(!outfile_.fail());
  wav_header_ = WavHeaderBuilder()
      .fmt_chunk_size(16)
      .audio_format(1)
      .num_channels(wav_params.num_channels())
      .sample_rate(wav_params.sample_rate())
      .bit_depth(static_cast<short>(wav_params.bit_depth()))
      .wav_header();
  // Write the WAV file header to the file
  outfile_.write((char *) &wav_header_, sizeof(wav_header_));
}

WavWriter::~WavWriter() {
  Write();
}

void WavWriter::AddSample(double sample_double) {
  CHECK(!is_finish_writing_) << "Do not add sample after finishing writing.";
  if (wav_header_.num_channels == 2) {
    AddSample(sample_double, sample_double);
    return;
  }
  CHECK_GT(sample_double, -1 - math::kEpsilon) << "sample " << sample_double << " is NOT in [-1, 1] range.";
  CHECK_LT(sample_double, 1 + math::kEpsilon) << "sample " << sample_double << " is NOT in [-1, 1] range.";
  sample_double = std::clamp(sample_double, -1.0, 1.0);
  const auto sample = static_cast<int>(sample_double * ((1 << (wav_header_.bit_depth - 1)) - 1));
  outfile_.write((char *) &sample, wav_header_.bit_depth / 8);
  ++sample_count_;
}

void WavWriter::AddSample(double sample_double_0, double sample_double_1) {
  CHECK(!is_finish_writing_) << "Do not add sample after finishing writing.";
  CHECK_EQ(wav_header_.num_channels, 2);
  CHECK_GT(sample_double_0, -1 - math::kEpsilon) << "sample " << sample_double_0 << " is NOT in [-1, 1] range.";
  CHECK_LT(sample_double_0, 1 + math::kEpsilon) << "sample " << sample_double_0 << " is NOT in [-1, 1] range.";
  CHECK_GT(sample_double_1, -1 - math::kEpsilon) << "sample " << sample_double_1 << " is NOT in [-1, 1] range.";
  CHECK_LT(sample_double_1, 1 + math::kEpsilon) << "sample " << sample_double_1 << " is NOT in [-1, 1] range.";
  sample_double_0 = std::clamp(sample_double_0, -1.0, 1.0);
  sample_double_1 = std::clamp(sample_double_1, -1.0, 1.0);
  const auto sample_0 = static_cast<int>(sample_double_0 * ((1 << (wav_header_.bit_depth - 1)) - 1));
  const auto sample_1 = static_cast<int>(sample_double_1 * ((1 << (wav_header_.bit_depth - 1)) - 1));
  outfile_.write((char *) &sample_0, wav_header_.bit_depth / 8);
  outfile_.write((char *) &sample_1, wav_header_.bit_depth / 8);
  ++sample_count_;
}

void WavWriter::Write() {
  if (is_finish_writing_) {
    return;
  }
  // Close the file
  outfile_.close();

  // Now that we know the size of the data chunk, update the header
  int data_bytes = sample_count_ * wav_header_.num_channels * static_cast<int>(wav_header_.bit_depth / 8);
  wav_header_ = WavHeaderBuilder(wav_header_).data_bytes(data_bytes).wav_header();

  // Open the file again and overwrite the header
  outfile_.open(filename_.data(), std::ios::binary | std::ios::in | std::ios::out);
  outfile_.seekp(0);
  outfile_.write((char *) &wav_header_, sizeof(wav_header_));

  // Close the file again
  outfile_.close();
  is_finish_writing_ = true;
}

}  // namespace wav
