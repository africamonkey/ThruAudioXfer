// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/wav/wav_reader.h"

#include <cmath>

#include "gtest/gtest.h"

#include "src/common/file/io.h"
#include "src/common/interface/proto/wav_params.pb.h"
#include "src/wav/wav_writer.h"

namespace wav {

TEST(WavReaderTest, ReadConstantFrequencyWav) {
  interface::WavParams wav_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  for (int num_channels = 1; num_channels <= 2; ++num_channels) {
    for (int bit_depth = 16; bit_depth <= 32; bit_depth += 8) {
      wav_params.set_num_channels(num_channels);
      wav_params.set_bit_depth(bit_depth);
      const std::string temp_filename =
          std::string() + "/tmp/constant_frequency_" + std::to_string(wav_params.num_channels()) + "_"
              + std::to_string(wav_params.bit_depth()) + ".wav";
      WavWriter wav_writer(temp_filename, wav_params);
      const int sample_count = wav_params.sample_rate() * 5; // 5 seconds of audio
      const double frequency = 440.00;  // in Hz.
      for (int i = 0; i < sample_count; ++i) {
        const double t = 1.0 * i / wav_params.sample_rate();
        if (num_channels == 1) {
          const double sample_double = std::sin(t * 2.0 * M_PI * frequency);
          wav_writer.AddSample(sample_double);
        } else {
          const double sample_double_0 = std::sin(t * 2.0 * M_PI * frequency);
          const double sample_double_1 = std::cos(t * 2.0 * M_PI * frequency);
          wav_writer.AddSample(sample_double_0, sample_double_1);
        }
      }
      wav_writer.Write();

      system((std::string() + "ls -l " + temp_filename).c_str());

      const double max_error = 1.0 / (double) (1ll << (wav_params.bit_depth() - 1));
      WavReader wav_reader(temp_filename);
      for (int i = 0; i < sample_count; ++i) {
        ASSERT_FALSE(wav_reader.IsEof());
        const std::pair<double, double> sample_to_evaluate = wav_reader.GetSample();
        const double t = 1.0 * i / wav_params.sample_rate();
        const double sample_correct_0 = std::sin(t * 2.0 * M_PI * frequency);
        const double sample_correct_1 = num_channels == 1 ? sample_correct_0 : std::cos(t * 2.0 * M_PI * frequency);
        ASSERT_NEAR(sample_correct_0, sample_to_evaluate.first, max_error)
                      << "num_channels=" << num_channels << ", bit_depth=" << bit_depth << ", i=" << i << ", t=" << t;
        ASSERT_NEAR(sample_correct_1, sample_to_evaluate.second, max_error)
                      << "num_channels=" << num_channels << ", bit_depth=" << bit_depth << ", i=" << i << ", t=" << t;
      }
      ASSERT_TRUE(wav_reader.IsEof());
      wav_reader.Close();
      io::DeleteFileIfExists(temp_filename);
    }
  }
}

}  // namespace wav
