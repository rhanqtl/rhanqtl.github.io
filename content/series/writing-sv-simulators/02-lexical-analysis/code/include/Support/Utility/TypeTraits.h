#ifndef SUPPORT_UTILITY_TYPE_TRAITS_H
#define SUPPORT_UTILITY_TYPE_TRAITS_H

#include <cstddef>

template <typename T, std::size_t N>
constexpr std::size_t length_of(const T (&)[N]) {
  return N;
}

#endif  // SUPPORT_UTILITY_TYPE_TRAITS_H
