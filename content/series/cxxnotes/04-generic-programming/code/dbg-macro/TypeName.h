#ifndef CPPNOTES_DBG_TYPENAME_H
#define CPPNOTES_DBG_TYPENAME_H

#include <string>

#if defined(__clang__)
#define MY_PRETTY_FUNC __PRETTY_FUNCTION__
static constexpr std::size_t PREFIX_LENGTH =
    sizeof("std::string name_of() [T = ") - 1;
static constexpr std::size_t SUFFIX_LENGTH = sizeof("]") - 1;
#elif defined(__GNUC__)
#error "unsupported compiler"
#elif defined(_MSC_VER)
#error "unsupported compiler"
#else
#error "unsupported compiler"
#endif

template <typename T> std::string name_of() {
  std::string s{MY_PRETTY_FUNC};
  return s.substr(PREFIX_LENGTH, s.length() - PREFIX_LENGTH - SUFFIX_LENGTH);
}

#endif // CPPNOTES_DBG_TYPENAME_H
