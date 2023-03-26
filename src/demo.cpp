#include <cmath>

#include "src/common/file/io.h"
#include "src/common/interface/proto/wav_params.pb.h"
#include "src/wav/wav_writer.h"

#include "glog/logging.h"

int main() {
  const std::string temp_filename = "/tmp/constant_frequency.wav";
  interface::WavParams wav_params;
  CHECK(io::ReadFromProtoInTextFormat("params/wav_params.txt", &wav_params));
  wav::WavWriter wav_writer(temp_filename, wav_params);
  const int sample_count = wav_params.sample_rate() * 5; // 5 seconds of audio
  const double frequency = 440.00;  // in Hz.
  for (int i = 0; i < sample_count; ++i) {
    const double t = 1.0 * i / wav_params.sample_rate();
    const double sample_d = std::sin(t * 2.0 * M_PI * frequency);
    const double volume = 0.2;
    wav_writer.AddSample(volume * sample_d);
  }
  wav_writer.Write();
  return 0;
}
