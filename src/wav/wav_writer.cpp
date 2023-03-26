#include "src/wav/wav_writer.h"

#include <cmath>
#include <iostream>

#include "src/common/math/math_utils.h"

#include "glog/logging.h"

namespace wav {

WavWriter::WavWriter(const std::string &filename, const interface::WavParams &wav_params)
    : filename_(filename), outfile_(), wav_header_(WavHeader()), sample_count_(0), is_finish_writing_(false) {
  // Open the output file for writing in binary mode
  outfile_ = std::ofstream(filename_.data(), std::ios::binary);
  wav_header_ = WavHeaderBuilder()
      .fmt_chunk_size(16)
      .audio_format(1)
      .num_channels(1)
      .sample_rate(wav_params.sample_rate())
      .bit_depth(16)
      .wav_header();
  // Write the WAV file header to the file
  outfile_.write((char *) &wav_header_, sizeof(wav_header_));
}

WavWriter::~WavWriter() {
  Write();
}

void WavWriter::AddSample(double sample_d) {
  CHECK(!is_finish_writing_) << "Do not add sample after finishing writing.";
  CHECK_GT(sample_d, -1 - math::kEpsilon) << "sample " << sample_d << " is NOT in [-1, 1] range.";
  CHECK_LT(sample_d, 1 + math::kEpsilon) << "sample " << sample_d << " is NOT in [-1, 1] range.";
  sample_d = math::Clamp(sample_d, -1.0, 1.0);
  const auto sample = static_cast<short>(sample_d * std::numeric_limits<short>::max());
  outfile_.write((char *) &sample, sizeof(sample));
  ++sample_count_;
}

void WavWriter::Write() {
  if (is_finish_writing_) {
    return;
  }
  // Close the file
  outfile_.close();

  // Now that we know the size of the data chunk, update the header
  int data_bytes = sample_count_ * static_cast<int>(sizeof(short));
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
