// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/wav/wav_writer.h"

#include <cmath>

#include "gtest/gtest.h"

#include "src/common/file/io.h"
#include "src/common/interface/proto/wav_params.pb.h"

namespace wav {

TEST(WavWriterTest, GenerateConstantFrequencyWavSingleChannel) {
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
      io::DeleteFileIfExists(temp_filename);
    }
  }
}

TEST(WavWriterTest, GenerateScale) {
  constexpr double kFrequencyPause = 0.0;
  constexpr double kFrequencyDo = 261.626;
  constexpr double kFrequencyRe = 293.665;
  constexpr double kFrequencyMi = 329.629;
  constexpr double kFrequencyFa = 349.228;
  constexpr double kFrequencySo = 391.995;
  constexpr double kFrequencyLa = 440.000;
  constexpr double kFrequencySi = 493.883;
  constexpr double kFrequencyHighDo = 523.251;
  struct Note {
    double frequency = 0.0;
    double duration = 0.0;
    Note(double frequency, double duration) : frequency(frequency), duration(duration) {}
  };
  const std::vector<Note> notes = {
      Note(kFrequencyDo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyRe, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyMi, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyFa, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencySo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyLa, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencySi, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyHighDo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyPause, 2.0),
  };
  const std::string temp_filename = "/tmp/scale.wav";
  interface::WavParams wav_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  WavWriter wav_writer(temp_filename, wav_params);
  for (const auto &note : notes) {
    int sample_count = static_cast<int>(std::floor(note.duration * wav_params.sample_rate()));
    for (int i = 0; i < sample_count; ++i) {
      const double t = 1.0 * i / wav_params.sample_rate();
      const double sample_d = std::sin(t * 2.0 * M_PI * note.frequency);
      wav_writer.AddSample(sample_d);
    }
  }
  wav_writer.Write();
  io::DeleteFileIfExists(temp_filename);
}

TEST(WavWriterTest, GenerateLittleStar) {
  constexpr double kFrequencyPause = 0.0;
  constexpr double kFrequencyDo = 261.626;
  constexpr double kFrequencyRe = 293.665;
  constexpr double kFrequencyMi = 329.629;
  constexpr double kFrequencyFa = 349.228;
  constexpr double kFrequencySo = 391.995;
  constexpr double kFrequencyLa = 440.000;
  struct Note {
    double frequency = 0.0;
    double duration = 0.0;
    Note(double frequency, double duration) : frequency(frequency), duration(duration) {}
  };
  const std::vector<Note> notes = {
      Note(kFrequencyDo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyDo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencySo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencySo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyLa, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyLa, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencySo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyPause, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyFa, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyFa, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyMi, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyMi, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyRe, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyRe, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyDo, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyPause, 0.20),
      Note(kFrequencyPause, 0.05),
      Note(kFrequencyPause, 2.0),
  };
  const std::string temp_filename = "/tmp/little_star.wav";
  interface::WavParams wav_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  WavWriter wav_writer(temp_filename, wav_params);
  for (const auto &note : notes) {
    int sample_count = static_cast<int>(std::floor(note.duration * wav_params.sample_rate()));
    for (int i = 0; i < sample_count; ++i) {
      const double t = 1.0 * i / wav_params.sample_rate();
      const double sample_d = std::sin(t * 2.0 * M_PI * note.frequency);
      wav_writer.AddSample(sample_d);
    }
  }
  wav_writer.Write();
  io::DeleteFileIfExists(temp_filename);
}

}  // namespace wav
