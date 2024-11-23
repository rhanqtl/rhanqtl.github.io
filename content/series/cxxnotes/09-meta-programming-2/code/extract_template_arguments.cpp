#include <iostream>
#include <utility>
#include <vector>

#include "print_type_name.h"

template <typename T>
struct extract_template_arguments {
 private:
  // We need a context in which template arguments can be deduced.
  template <template <typename...> class TT, typename... Args>
  static auto check(TT<Args...>) -> std::tuple<Args...>;
  static auto check(...) -> void;

 public:
  using type = decltype(check(std::declval<T>()));
};

template <typename T>
using extract_template_arguments_t =
    typename extract_template_arguments<T>::type;

int main() {
  std::cout << get_type_name<extract_template_arguments_t<std::vector<int>>>()
            << '\n';
}
