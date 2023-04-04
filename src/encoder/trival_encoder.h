// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#pragma once

#include "src/encoder/encoder_base.h"

namespace encoder {

class TrivalEncoder final : EncoderBase {
 public:
  TrivalEncoder(int audio_sample_rate, interface::EncoderParams encoder_params);

  void Encode(const std::function<bool(char *)> &get_next_byte,
              const std::function<void(double)> &set_next_audio_sample) const override;

  void Decode(const std::function<bool(double *)> &get_next_audio_sample,
              const std::function<void(char)> &set_next_byte) const override;

  void GetAmplitudeAndStandardError(const std::vector<double> &samples,
                                    double frequency,
                                    double *amplitude,
                                    double *std_error) const;

 private:
  double encoder_rate_ = 0.0;
  double encode_frequency_for_bit_0_ = 0.0;
  double encode_frequency_for_bit_1_ = 0.0;
  double minimum_absolute_amplitude_ = 0.0;
  double maximum_standard_error_ = 0.0;
};

} // encoder
