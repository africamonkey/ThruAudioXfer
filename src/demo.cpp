#include <cmath>

#include "src/wav_io.h"

int main() {
  constexpr int kSampleRate = 44100;
  const int sample_count = kSampleRate * 5; // 5 seconds of audio
  std::function<double(int)> sample_function = [](int sample_iteration) {
    if (sample_iteration > sample_count) {
      return std::numeric_limits<double>::quiet_NaN();
    }
    const double frequency = 440.00;  // in Hz.
    const double t = 1.0 * sample_iteration / kSampleRate;
    const double sample_d = std::sin(t * 2.0 * M_PI * frequency);
    const double volume = 0.2;
    return volume * sample_d;
  };

  wav::WriteToWavFile("tmp/output.wav", kSampleRate, sample_function);

  return 0;
}
