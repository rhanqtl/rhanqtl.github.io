#ifndef MY_STRING_UTIL__H
#define MY_STRING_UTIL__H

#include <string>
#include <string_view>

namespace demo::strings {
auto reverse(std::string_view s) -> std::string;
}

#endif  // MY_STRING_UTIL__H
