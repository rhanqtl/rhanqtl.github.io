#ifndef CPPNOTES_DBG_DEBUG_H
#define CPPNOTES_DBG_DEBUG_H

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "TypeName.h"
#include "TypeTraits.h"

//===

inline void print_header(const char *file, int line, const char *func,
                         const char *expr) {
  std::printf("%s:%d (%s) %s%s", file, line, func, expr, " = ");
}

//===

template <typename R>
inline void print_separated_kv_list(const R &rng,
                                    const std::string &sep = ", ") {
  bool first = true;
  for (auto it = rng.cbegin(), ed = rng.cend(); it != ed; ++it) {
    if (first)
      first = false;
    else
      std::cout << sep;
    const auto &[k, v] = *it;
    std::cout << '[' << k << ']' << " = " << v;
  }
}

template <typename R>
inline void print_separated_seq_list(const R &rng,
                                     const std::string &sep = ", ") {
  std::size_t i = 0;
  bool first = true;
  for (auto it = rng.cbegin(), ed = rng.cend(); it != ed; ++it) {
    if (first)
      first = false;
    else
      std::cout << sep;
    std::cout << '[' << i++ << ']' << " = " << *it;
  }
}

template <typename _C>
inline void print_content_impl(const _C &rng, const std::string &sep = ", ") {
  using R = std::remove_reference_t<std::remove_cv_t<_C>>;
  if constexpr (is_associative_container_v<R>) {
    print_separated_kv_list(rng);
  } else if constexpr (is_sequential_container_v<R>) {
    print_separated_seq_list(rng);
  } else {
    throw std::invalid_argument("NYI");
  }
}

// ====

template <typename T, typename = std::enable_if_t<!is_container_v<T>>>
inline void print_content(T &&expr) {
  std::cout << expr;
}

template <typename C, typename = std::enable_if_t<is_container_v<C>>>
inline void print_content(const C &cont) {
  print_content_impl(cont);
}

//===

template <typename T> inline void print_type() {
  std::printf(" ( type = %s)\n", name_of<T>().c_str());
}

//===

template <typename T>
inline T &&dbg_handler(const char *file, int line, const char *func, T &&expr,
                       const char *expr_str) {
  print_header(file, line, func, expr_str);
  print_content(std::forward<T>(expr));
  print_type<T>();
  return std::forward<T>(expr);
}

#define dbg(x) dbg_handler(__FILE__, __LINE__, __FUNCTION__, x, #x)

#endif // CPPNOTES_DBG_DEBUG_H
