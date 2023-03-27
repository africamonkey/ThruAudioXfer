// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/encoder/encoder_base.h"

#include "glog/logging.h"

namespace encoder {

EncoderBase::EncoderBase(int audio_sample_rate, interface::EncoderParams encoder_params)
    : audio_sample_rate_(audio_sample_rate), encoder_params_(std::move(encoder_params)) {
  CHECK_GT(audio_sample_rate_, 0);
}

}  // namespace encoder
