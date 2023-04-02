// Copyright (c) 2023. Kaiqi Wang - All Rights Reserved

#pragma once

namespace math {

constexpr double kEpsilon = 1e-9;

template <typename T>
constexpr T Sqr(T x) {
  return x * x;
}

}  // namespace math
