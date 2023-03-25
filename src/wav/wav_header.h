#include <cstring>

namespace wav {

struct WavHeader {
  // RIFF Header
  char riff_header[4];  // Contains "RIFF"
  int wav_size;         // Size of the WAV file
  char wave_header[4];  // Contains "WAVE"

  // Format Header
  char fmt_header[4];      // Contains "fmt "
  int fmt_chunk_size;      // Size of the format chunk
  short audio_format;      // Audio format (1 for PCM)
  short num_channels;      // Number of channels
  int sample_rate;         // Sample rate
  int byte_rate;           // Byte rate
  short sample_alignment;  // Sample alignment
  short bit_depth;         // Bit depth

  // Data Header
  char data_header[4];  // Contains "data"
  int data_bytes;       // Number of bytes in data
};

class WavHeaderBuilder {
 public:
  WavHeaderBuilder() {
    memcpy(wav_header_.riff_header, "RIFF", 4);
    memcpy(wav_header_.wave_header, "WAVE", 4);
    memcpy(wav_header_.fmt_header, "fmt ", 4);
    memcpy(wav_header_.data_header, "data", 4);
    wav_header_.fmt_chunk_size = 16;
    wav_header_.audio_format = 1;
    wav_header_.num_channels = 1;
    wav_header_.sample_rate = 44100;
    wav_header_.bit_depth = 16;
    wav_header_.data_bytes = 0;
    UpdateSampleAlignment();
    UpdateByteRate();
    UpdateWavSize();
  }
  WavHeaderBuilder(WavHeader wav_header) {
    wav_header_ = std::move(wav_header);
    UpdateSampleAlignment();
    UpdateByteRate();
    UpdateWavSize();
  }
  ~WavHeaderBuilder() = default;

  const WavHeader& wav_header() {
    return wav_header_;
  }
  WavHeaderBuilder& fmt_chunk_size(int fmt_chunk_size) {
    wav_header_.fmt_chunk_size = fmt_chunk_size;
    return *this;
  }
  WavHeaderBuilder& audio_format(short audio_format) {
    wav_header_.audio_format = audio_format;
    return *this;
  }
  WavHeaderBuilder& num_channels(short num_channels) {
    wav_header_.num_channels = num_channels;
    UpdateSampleAlignment();
    return *this;
  }
  WavHeaderBuilder& sample_rate(int sample_rate) {
    wav_header_.sample_rate = sample_rate;
    UpdateByteRate();
    return *this;
  }
  WavHeaderBuilder& bit_depth(short bit_depth) {
    wav_header_.bit_depth = bit_depth;
    UpdateSampleAlignment();
    return *this;
  }
  WavHeaderBuilder& data_bytes(int data_bytes) {
    wav_header_.data_bytes = data_bytes;
    UpdateWavSize();
    return *this;
  }
  void UpdateSampleAlignment() {
    wav_header_.sample_alignment = wav_header_.num_channels * (wav_header_.bit_depth / 8);
    UpdateByteRate();
  }
  void UpdateByteRate() {
    wav_header_.byte_rate = wav_header_.sample_rate * wav_header_.sample_alignment;
  }
  void UpdateWavSize() {
    wav_header_.wav_size = wav_header_.data_bytes + sizeof(WavHeader) - 8;
  }

 private:
  WavHeader wav_header_;
};

}  // namespace wav
