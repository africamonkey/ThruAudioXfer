#pragma once

#include <fstream>
#include <functional>

#include "src/common/interface/proto/wav_params.pb.h"
#include "src/wav/wav_header.h"

namespace wav {

class WavWriter {
 public:
  WavWriter(const std::string& filename, const interface::WavParams& wav_params);
  ~WavWriter();

  // Sample data should be in [-1, 1].
  void AddSample(double sample);
  void Write();

 private:
  std::string filename_;
  std::ofstream outfile_;
  WavHeader wav_header_;
  int sample_count_;
  bool is_finish_writing_;
};

}  // namespace wav
