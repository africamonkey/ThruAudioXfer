#pragma once

namespace math {

constexpr double kEpsilon = 1e-9;

double Clamp(double value, double lower_bound, double upper_bound);

}  // namespace math
