#ifndef TESTS_ASSERTION_HELPER_HPP
#define TESTS_ASSERTION_HELPER_HPP

#include <cmath>

#include <glm/glm.hpp>

template <class Vector>
bool vec_almost_equal(const Vector& lhs, const Vector& rhs) {
  return glm::length(lhs - rhs) < 0.000001;
}

inline bool float_almost_equal(const float lhs, const float rhs) { return std::abs(lhs - rhs) < 0.000001; }

#endif