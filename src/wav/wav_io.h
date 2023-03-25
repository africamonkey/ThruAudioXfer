#include <functional>

#include "src/common/interface/proto/wav_params.pb.h"

namespace wav {

// Sample rate: samples per second.
// Sample function: maps from sample iteration to sample data. Sample data should be in [0, 1].
void WriteToWavFile(const std::string& filename,
                    const interface::WavParams& wav_params,
                    std::function<double(int)> sample_function);

}  // namespace wav
