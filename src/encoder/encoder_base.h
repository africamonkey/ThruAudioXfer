// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#pragma once

#include <functional>
#include <string>

#include "src/common/interface/proto/encoder_params.pb.h"

namespace encoder {

class EncoderBase {
 public:
  EncoderBase(int audio_sample_rate, interface::EncoderParams encoder_params);
  virtual ~EncoderBase() = default;

  virtual void Encode(const std::function<char(void)> &get_next_byte,
                      const std::function<void(double)> &set_next_audio_sample) const = 0;
  virtual void Decode(const std::function<double(void)> &get_next_audio_sample,
                      const std::function<void(char)> &set_next_byte) const = 0;

 protected:
  int audio_sample_rate_;
  interface::EncoderParams encoder_params_;
};

}  // namespace encoder
