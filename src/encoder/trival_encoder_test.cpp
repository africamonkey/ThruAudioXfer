// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#include "src/encoder/trival_encoder.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "src/common/file/io.h"
#include "src/common/interface/proto/encoder_params.pb.h"

TEST(TrivalEncoderTest, EncodeAndDecode) {
  interface::EncoderParams encoder_params;
  ASSERT_TRUE(io::ReadFromProtoInTextFormat("params/encoder_params.txt", &encoder_params));
  constexpr int kAudioSampleRate = 44100;
  encoder::TrivalEncoder trival_encoder(kAudioSampleRate, std::move(encoder_params));
  const std::string kStringToBeEncoded = "1f745684946ba0c5ccd19205003c387f637cfc736fe98af5c341c4c02bc54bb7";

  // encode
  std::vector<double> encoded_double;
  int encode_current_id = 0;
  std::function get_next_byte = [&kStringToBeEncoded, &encode_current_id](char* byte) -> bool {
    if (encode_current_id >= kStringToBeEncoded.size()) {
      return false;
    }
    *CHECK_NOTNULL(byte) = kStringToBeEncoded[encode_current_id++];
    return true;
  };
  std::function set_next_sample = [&encoded_double](double sample) {
    encoded_double.push_back(sample);
  };
  trival_encoder.Encode(get_next_byte, set_next_sample);

  // decode
  std::string decoded_string;
  int decode_current_id = 0;
  std::function get_next_sample = [&encoded_double, &decode_current_id](double* sample) -> bool {
    if (decode_current_id >= encoded_double.size()) {
      return false;
    }
    *CHECK_NOTNULL(sample) = encoded_double[decode_current_id++];
    return true;
  };
  std::function set_next_byte = [&decoded_string](char byte) {
    decoded_string += byte;
  };
  trival_encoder.Decode(get_next_sample, set_next_byte);
  EXPECT_EQ(kStringToBeEncoded, decoded_string);
}
