#include <numeric>

#include "external/com_google_glog/_virtual_includes/glog/glog/logging.h"

namespace math {

constexpr double kEpsilon = 1e-9;

double Clamp(double value, double lower_bound, double upper_bound) {
  CHECK(!std::isnan(value));
  if (value < lower_bound) {
    return lower_bound;
  }
  if (value > upper_bound) {
    return upper_bound;
  }
  return value;
}

};
