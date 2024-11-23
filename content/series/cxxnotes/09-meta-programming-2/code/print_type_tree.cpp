#include <array>
#include <iostream>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "print_type_name.h"

using Masks = std::vector<bool>;

std::array<std::string_view, 3> corners = {{"|--", "`--", "|"}};

template <typename T>
struct template_name {
  static constexpr auto get() -> std::string_view { return get_impl(T{}); }

  template <template <typename...> typename TT, typename... Args>
  static constexpr auto get_impl(TT<Args...>) -> std::string_view {
    return get_type_name<TT>();
  }
};

//===
//
//===
template <typename T>
struct is_specialization {
 private:
  template <template <typename...> typename TT, typename... Args>
  static auto check(TT<Args...>) -> std::true_type;
  static auto check(...) -> std::false_type;

 public:
  static constexpr bool value = decltype(check(std::declval<T>()))::value;
};

template <typename T>
constexpr bool is_specialization_v = is_specialization<T>::value;

//===
//
//===
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

//===
//
//===

template <typename T, std::size_t D>
void print_tree_impl(Masks &masks, bool last_child);

template <std::size_t D, std::size_t... Is, typename... Ts>
void helper(std::index_sequence<Is...>, std::tuple<Ts...>, Masks &masks) {
  (print_tree_impl<Ts, D>(masks, Is + 1 == sizeof...(Is)), ...);
}

template <typename T, std::size_t D>
void print_tree_impl(Masks &masks, bool last_child) {
  if constexpr (is_specialization_v<T>) {
    using tuple = extract_template_arguments_t<T>;
    using indexes = std::make_index_sequence<std::tuple_size_v<tuple>>;
    for (auto b : masks) {
      std::cout << (b ? "|   " : "    ");
    }
    std::cout << (last_child ? "`-- " : "|-- ") << template_name<T>::get()
              << '\n';
    masks.push_back(!last_child);
    helper<D + 1>(indexes{}, tuple{}, masks);
    masks.pop_back();
  } else {
    for (auto b : masks) {
      std::cout << (b ? "|   " : "    ");
    }
    std::cout << (last_child ? "`-- " : "|-- ") << get_type_name<T>() << '\n';
  }
}

template <typename T>
void print_tree() {
  auto masks = Masks{};
  print_tree_impl<T, /* D= */ 0>(masks, true);
}

int main() { print_tree<std::vector<int>>(); }
