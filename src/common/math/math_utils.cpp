#include <cmath>

#include "glog/logging.h"

namespace math {

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

}  // namespace math
