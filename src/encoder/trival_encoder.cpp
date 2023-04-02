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
      double sign = (next_byte_to_int & (1 << j)) ? 1.0 : -1.0;
      for (int sample = 0; sample < delta_count; ++sample) {
        double amplitude = sign * std::sin(M_PI / delta_count * sample);
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
  double last_std_error = std::numeric_limits<double>::infinity();
  int last_byte = 0;
  int last_byte_bit_count = 0;
  int current_bit_count = 0;
  while (get_next_audio_sample(&next_sample)) {
    sample_window.push_back(next_sample);
    while ((int)sample_window.size() > window_size) {
      sample_window.erase(sample_window.begin());
    }
    if ((int)sample_window.size() != window_size) {
      continue;
    }
    double amplitude = 0.0;
    double std_error = 0.0;
    GetAmplitudeAndStandardError(sample_window, &amplitude, &std_error);
    if (std::abs(amplitude) < minimum_absolute_amplitude_) {
      continue;
    }
    if (std_error > maximum_standard_error_) {
      continue;
    }
    if (std_error < last_std_error) {
      last_std_error = std_error;
      continue;
    }
    last_byte |= (1 << last_byte_bit_count) * (amplitude > 0.0 ? 1 : 0);
    ++last_byte_bit_count;
    ++current_bit_count;
    if (last_byte_bit_count == 8) {
      set_next_byte(static_cast<char>(last_byte));
      last_byte = 0;
      last_byte_bit_count = 0;
    }
    sample_window.erase(sample_window.begin(), sample_window.begin() + static_cast<int>(sample_window.size() / 2));
  }
  LOG(INFO) << "Total bit count: " << current_bit_count;
  CHECK_EQ(last_byte_bit_count, 0) << "Decode error: Incomplete data.";
}

void TrivalEncoder::GetAmplitudeAndStandardError(const std::vector<double> &samples,
                                                 double *amplitude,
                                                 double *std_error) const {
  *CHECK_NOTNULL(amplitude) = 0;
  *CHECK_NOTNULL(std_error) = 0;
  CHECK(!samples.empty());
  const double real_sample_counts = audio_sample_rate_ / encoder_rate_;
  // min_A 1/n * \Sigma_{i=0}^{n-1} (y_i - A * x_i)^2
  // You can expand it to a quadratic function of A.
  double a = 0, b = 0, c = 0;
  for (int i = 0; i < (int)samples.size(); ++i) {
    const double x_i = std::sin(M_PI / real_sample_counts * i);
    const double y_i = samples[i];
    a += math::Sqr(x_i);
    b += -2 * x_i * y_i;
    c += math::Sqr(y_i);
  }
  if (std::abs(a) < math::kEpsilon) {
    *amplitude = 0;
    *std_error = std::sqrt(c / (double)samples.size());
    return;
  }
  *amplitude = -b / (2 * a);
  *std_error = std::sqrt(std::max(0.0, (a * math::Sqr(*amplitude) + b * (*amplitude) + c) / (double)samples.size()));
}

} // encoder
