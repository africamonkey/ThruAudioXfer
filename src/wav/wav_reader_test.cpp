// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/wav/wav_reader.h"

#include <cmath>

#include "gtest/gtest.h"

#include "src/common/file/io.h"
#include "src/common/interface/proto/wav_params.pb.h"
#include "src/wav/wav_writer.h"

namespace wav {

TEST(WavReaderTest, ReadConstantFrequencyWavSingleChannel) {
  const std::string temp_filename = "/tmp/constant_frequency.wav";
  interface::WavParams wav_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  wav_params.set_num_channels(1);
  WavWriter wav_writer(temp_filename, wav_params);
  const int sample_count = wav_params.sample_rate() * 5; // 5 seconds of audio
  const double frequency = 440.00;  // in Hz.
  for (int i = 0; i < sample_count; ++i) {
    const double t = 1.0 * i / wav_params.sample_rate();
    const double sample_d = std::sin(t * 2.0 * M_PI * frequency);
    wav_writer.AddSample(sample_d);
  }
  wav_writer.Write();

  const double max_error = 1.0 / (double) (1ll << (wav_params.bit_depth() - 1));
  WavReader wav_reader(temp_filename);
  for (int i = 0; i < sample_count; ++i) {
    ASSERT_FALSE(wav_reader.IsEof());
    const std::pair<double, double> sample_to_evaluate = wav_reader.GetSample();
    const double t = 1.0 * i / wav_params.sample_rate();
    const double sample_correct = std::sin(t * 2.0 * M_PI * frequency);
    ASSERT_NEAR(sample_correct, sample_to_evaluate.first, max_error) << i;
    ASSERT_NEAR(sample_correct, sample_to_evaluate.second, max_error) << i;
  }
  ASSERT_TRUE(wav_reader.IsEof());
  io::DeleteFileIfExists(temp_filename);
}

TEST(WavReaderTest, ReadConstantFrequencyWavDualChannel) {
  const std::string temp_filename = "/tmp/constant_frequency.wav";
  interface::WavParams wav_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  wav_params.set_num_channels(2);
  WavWriter wav_writer(temp_filename, wav_params);
  const int sample_count = wav_params.sample_rate() * 5; // 5 seconds of audio
  const double frequency = 440.00;  // in Hz.
  for (int i = 0; i < sample_count; ++i) {
    const double t = 1.0 * i / wav_params.sample_rate();
    const double sample_d_0 = std::sin(t * 2.0 * M_PI * frequency);
    const double sample_d_1 = std::cos(t * 2.0 * M_PI * frequency);
    wav_writer.AddSample(sample_d_0, sample_d_1);
  }
  wav_writer.Write();

  const double max_error = 1.0 / (double) (1ll << (wav_params.bit_depth() - 1));
  WavReader wav_reader(temp_filename);
  for (int i = 0; i < sample_count; ++i) {
    ASSERT_FALSE(wav_reader.IsEof());
    const std::pair<double, double> sample_to_evaluate = wav_reader.GetSample();
    const double t = 1.0 * i / wav_params.sample_rate();
    const double sample_correct_0 = std::sin(t * 2.0 * M_PI * frequency);
    const double sample_correct_1 = std::cos(t * 2.0 * M_PI * frequency);
    ASSERT_NEAR(sample_correct_0, sample_to_evaluate.first, max_error) << i;
    ASSERT_NEAR(sample_correct_1, sample_to_evaluate.second, max_error) << i;
  }
  ASSERT_TRUE(wav_reader.IsEof());
  io::DeleteFileIfExists(temp_filename);
}

}  // namespace wav
