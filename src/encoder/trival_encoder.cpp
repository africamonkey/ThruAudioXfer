// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "trival_encoder.h"

#include <cmath>

#include "glog/logging.h"
#include "src/common/math/math_utils.h"

namespace encoder {

TrivalEncoder::TrivalEncoder(int audio_sample_rate, interface::EncoderParams encoder_params) : EncoderBase(
    audio_sample_rate,
    std::move(encoder_params)) {
  CHECK(encoder_params_.has_trival_encoder_params());
  CHECK(encoder_params_.trival_encoder_params().has_encoder_rate());
  CHECK_GT(encoder_params_.trival_encoder_params().encoder_rate(), 0.0);
  encoder_rate_ = encoder_params_.trival_encoder_params().encoder_rate();
  encode_frequency_for_bit_0_ = encoder_params_.trival_encoder_params().encode_frequency_for_bit_0();
  encode_frequency_for_bit_1_ = encoder_params_.trival_encoder_params().encode_frequency_for_bit_1();
  encode_frequency_for_rest_ = encoder_params_.trival_encoder_params().encode_frequency_for_rest();
  minimum_absolute_amplitude_ = encoder_params_.trival_encoder_params().minimum_absolute_amplitude();
  maximum_standard_error_ = encoder_params_.trival_encoder_params().maximum_standard_error();
  CHECK_GT(audio_sample_rate_, encoder_rate_ * 2);
}

void TrivalEncoder::Encode(const std::function<bool(char *)> &get_next_byte,
                           const std::function<void(double)> &set_next_audio_sample) const {
  char next_byte;
  int current_bit_count = 0;
  // +1s
  for (int i = 0; i < audio_sample_rate_; ++i) {
    set_next_audio_sample(0.0);
  }
  while (get_next_byte(&next_byte)) {
    int next_byte_to_int = (int) static_cast<unsigned char>(next_byte);
    // little endian
    for (int j = 0; j < 8; ++j) {
      int next_count = static_cast<int>((current_bit_count + 1) * audio_sample_rate_ / encoder_rate_);
      int curr_count = static_cast<int>(current_bit_count * audio_sample_rate_ / encoder_rate_);
      int delta_count = next_count - curr_count;
      double frequency =
          (next_byte_to_int & (1 << j)) ? encode_frequency_for_bit_1_ : encode_frequency_for_bit_0_;
      for (int sample = 0; sample < delta_count; ++sample) {
        double amplitude = std::sin(2 * M_PI * frequency * (double) sample / audio_sample_rate_);
        set_next_audio_sample(amplitude);
      }
      ++current_bit_count;
      for (int sample = 0; sample < delta_count; ++sample) {
        double amplitude = std::sin(2 * M_PI * encode_frequency_for_rest_ * (double) sample / audio_sample_rate_);
        set_next_audio_sample(amplitude);
      }
    }
  }
  // +1s
  for (int i = 0; i < audio_sample_rate_; ++i) {
    set_next_audio_sample(0.0);
  }
}

void TrivalEncoder::Decode(const std::function<bool(double *)> &get_next_audio_sample,
                           const std::function<void(char)> &set_next_byte) const {
  const int window_size = static_cast<int>(audio_sample_rate_ / encoder_rate_);
  // Find first period.
  std::vector<double> sample_window;
  double next_sample;
  int last_byte = 0;
  int last_byte_bit_count = 0;
  int current_bit_count = 0;
  while (get_next_audio_sample(&next_sample)) {
    sample_window.push_back(next_sample);
    while ((int) sample_window.size() > window_size) {
      sample_window.erase(sample_window.begin());
    }
    if ((int) sample_window.size() != window_size) {
      continue;
    }
    double amplitude_for_bit_0 = 0.0;
    double amplitude_for_bit_1 = 0.0;
    double std_error_for_bit_0 = 0.0;
    double std_error_for_bit_1 = 0.0;
    GetAmplitudeAndStandardError(sample_window,
                                 encode_frequency_for_bit_0_,
                                 &amplitude_for_bit_0,
                                 &std_error_for_bit_0);
    GetAmplitudeAndStandardError(sample_window,
                                 encode_frequency_for_bit_1_,
                                 &amplitude_for_bit_1,
                                 &std_error_for_bit_1);
    LOG(ERROR) << amplitude_for_bit_0 << " " << amplitude_for_bit_1 << " " << std_error_for_bit_0 << " " << std_error_for_bit_1 << " " << current_bit_count;
    if (std::max(std::abs(amplitude_for_bit_0), std::abs(amplitude_for_bit_1)) < minimum_absolute_amplitude_) {
      continue;
    }
    if (std::min(std_error_for_bit_0, std_error_for_bit_1) > maximum_standard_error_) {
      continue;
    }
    LOG(ERROR) << "valid bit";
    last_byte |= (1 << last_byte_bit_count) * static_cast<int>(std_error_for_bit_1 < std_error_for_bit_0);
    ++last_byte_bit_count;
    ++current_bit_count;
    if (last_byte_bit_count == 8) {
      set_next_byte(static_cast<char>(last_byte));
      LOG(ERROR) << "submit " << static_cast<char>(last_byte);
      last_byte = 0;
      last_byte_bit_count = 0;
    }
    sample_window.clear();
  }
  LOG(INFO) << "Total bit count: " << current_bit_count;
  CHECK_EQ(last_byte_bit_count, 0) << "Decode error: Incomplete data.";
}

void TrivalEncoder::GetAmplitudeAndStandardError(const std::vector<double> &samples,
                                                 double frequency,
                                                 double *amplitude,
                                                 double *std_error) const {
  *CHECK_NOTNULL(amplitude) = 0;
  *CHECK_NOTNULL(std_error) = 0;
  CHECK(!samples.empty());
  for (int i = 0; i < (int)samples.size(); ++i) {
    const double x_i = std::sin(2 * M_PI * frequency * (double) i / audio_sample_rate_);
    const double y_i = samples[i];
    *amplitude += std::abs(y_i);
    if (x_i * y_i <= math::kEpsilon) {
      *std_error += 1;
    }
  }
  *amplitude *= M_PI_2 / (int)samples.size();
  *std_error /= (int)samples.size();
}

} // encoder
