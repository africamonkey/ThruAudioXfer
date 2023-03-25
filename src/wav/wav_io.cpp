#include "src/wav/wav_io.h"

#include <cmath>
#include <iostream>
#include <fstream>

#include "src/common/math_utils.h"
#include "src/wav/wav_header.h"

#include "glog/logging.h"

namespace wav {

// Sample rate: samples per second.
// Sample function: maps from sample iteration to sample data. Sample data should be in [-1, 1].
void WriteToWavFile(const char* filename,
                    int sample_rate,
                    std::function<double(int)> sample_function) {
  // Open the output file for writing in binary mode
  std::ofstream outfile(filename, std::ios::binary);
  WavHeader wav_header = WavHeaderBuilder()
      .fmt_chunk_size(16)
      .audio_format(1)
      .num_channels(1)
      .sample_rate(sample_rate)
      .bit_depth(16)
      .wav_header();
  // Write the WAV file header to the file
  outfile.write((char*)&wav_header, sizeof(wav_header));

  // Write the WAV data to the file
  int sample_count = 0;
  for (sample_count = 0; sample_count < std::numeric_limits<int>::max(); ++sample_count) {
    double sample_d = sample_function(sample_count);
    if (std::isnan(sample_d)) {
      break;
    }
    CHECK_GT(sample_d, -1 - math::kEpsilon) << "sample at " << sample_count << " is NOT in [-1, 1] range.";
    CHECK_LT(sample_d, 1 + math::kEpsilon) << "sample at " << sample_count << " is NOT in [-1, 1] range.";
    sample_d = math::Clamp(sample_d, -1.0, 1.0);
    const auto sample = static_cast<short>(sample_d * std::numeric_limits<short>::max());
    outfile.write((char*)&sample, sizeof(sample));
  }

  // Close the file
  outfile.close();

  // Now that we know the size of the data chunk, update the header
  int data_bytes = sample_count * static_cast<int>(sizeof(short));
  wav_header = wav::WavHeaderBuilder(wav_header).data_bytes(data_bytes).wav_header();

  // Open the file again and overwrite the header
  outfile.open(filename, std::ios::binary | std::ios::in | std::ios::out);
  outfile.seekp(0);
  outfile.write((char*)&wav_header, sizeof(wav_header));

  // Close the file again
  outfile.close();
}

}  // namespace wav
