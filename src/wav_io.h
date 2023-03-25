#include <functional>

namespace wav {

// Sample rate: samples per second.
// Sample function: maps from sample iteration to sample data. Sample data should be in [0, 1].
void WriteToWavFile(const char* filename,
                    int sample_rate,
                    std::function<double(int)> sample_function);

}  // namespace wav
