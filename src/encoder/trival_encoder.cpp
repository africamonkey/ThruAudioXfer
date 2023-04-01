// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "trival_encoder.h"

#include "glog/logging.h"

namespace encoder {

TrivalEncoder::TrivalEncoder(int audio_sample_rate, interface::EncoderParams encoder_params) : EncoderBase(
    audio_sample_rate,
    std::move(encoder_params)) {
  CHECK(encoder_params_.has_trival_encoder_params());
  CHECK(encoder_params_.trival_encoder_params().has_encoder_rate());
  CHECK_GT(encoder_params_.trival_encoder_params().encoder_rate(), 0.0);
  encoder_rate_ = encoder_params_.trival_encoder_params().encoder_rate();
}

void TrivalEncoder::Encode(const std::function<bool(char*)> &get_next_byte,
                           const std::function<void(double)> &set_next_audio_sample) const {
  char next_byte;
  while (get_next_byte(&next_byte)) {
    set_next_audio_sample(next_byte);
  }
}

void TrivalEncoder::Decode(const std::function<bool(double*)> &get_next_audio_sample,
                           const std::function<void(char)> &set_next_byte) const {
  double next_sample;
  while (get_next_audio_sample(&next_sample)) {
    set_next_byte(static_cast<char>(next_sample));
  }
}

} // encoder
