#include <string_view>

template <typename T>
std::string_view get_type_name() {
#include "get_type_name_body.inc"
}

template <template <typename...> typename T>
std::string_view get_type_name() {
#include "get_type_name_body.inc"
}
