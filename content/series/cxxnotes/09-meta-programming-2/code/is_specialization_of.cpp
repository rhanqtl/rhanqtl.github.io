#include <type_traits>
#include <utility>
#include <vector>

template <typename T, template <typename...> class Container>
struct is_specialization_of {
  // We need a context in which template arguments can be deduced.
  template <typename... Args>
  static auto check(Container<Args...>) -> std::true_type;

  static auto check(...) -> std::false_type;

  static constexpr bool value = decltype(check(std::declval<T>()))::value;
};

template <typename T, template <typename...> class Container>
constexpr bool is_specialization_of_v =
    is_specialization_of<T, Container>::value;

int main() {
  static_assert(is_specialization_of_v<std::vector<int>, std::vector>);
}
