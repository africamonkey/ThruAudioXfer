#include <cmath>
#include <iostream>
#include <fstream>

#include "src/wav_header.h"

int main() {
  // Open the output file for writing in binary mode
  std::ofstream outfile("tmp/output.wav", std::ios::binary);

  WavHeader wav_header = WavHeaderBuilder()
                             .fmt_chunk_size(16)
                             .audio_format(1)
                             .num_channels(1)
                             .sample_rate(44100)
                             .bit_depth(16)
                             .wav_header();
  // Write the WAV file header to the file
  outfile.write((char*)&wav_header, sizeof(wav_header));

  // Write the WAV data to the file
  const int sample_count = 44100 * 5; // 5 seconds of audio
  short sample;
  const double frequency = 440.00;  // in Hz.
  for (int i = 0; i < sample_count; i++) {
    const double t = 1.0 * i / wav_header.sample_rate;
    const double sample_d = std::sin(t * 2.0 * std::acos(-1.0) * frequency);
    const double volume = 0.2;
    const short sample = volume * sample_d * std::numeric_limits<short>::max();
    outfile.write((char*)&sample, sizeof(sample));
  }

  // Close the file
  outfile.close();

  // Now that we know the size of the data chunk, update the header
  int data_bytes = sample_count * sizeof(sample);
  wav_header = WavHeaderBuilder(wav_header).data_bytes(data_bytes).wav_header();

  // Open the file again and overwrite the header
  outfile.open("output.wav", std::ios::binary | std::ios::in | std::ios::out);
  outfile.seekp(0);
  outfile.write((char*)&wav_header, sizeof(wav_header));

  // Close the file again
  outfile.close();
}