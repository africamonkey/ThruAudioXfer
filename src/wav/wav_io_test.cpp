#include "src/wav/wav_io.h"

#include "gtest/gtest.h"

#include "src/common/file/io.h"
#include "src/common/interface/proto/wav_params.pb.h"

TEST(WavIoTest, GenerateConstantFrequencyWav) {
  const std::string temp_filename = "/tmp/constant_frequency.wav";
  interface::WavParams wav_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  std::function<double(int)> sample_function = [&wav_params](int sample_iteration) {
    const int sample_count = wav_params.sample_rate() * 5; // 5 seconds of audio
    if (sample_iteration > sample_count) {
      return std::numeric_limits<double>::quiet_NaN();
    }
    const double frequency = 440.00;  // in Hz.
    const double t = 1.0 * sample_iteration / wav_params.sample_rate();
    const double sample_d = std::sin(t * 2.0 * M_PI * frequency);
    const double volume = 0.2;
    return volume * sample_d;
  };
  wav::WriteToWavFile(temp_filename, wav_params, sample_function);
  io::DeleteFileIfExists(temp_filename);
}
