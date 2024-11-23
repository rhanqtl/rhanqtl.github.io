#ifndef REFLECT_ACCESS__H
#define REFLECT_ACCESS__H

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "ast/fwd.h"

namespace reflect {
template <typename T>
struct is_ast_node {
 private:
  static constexpr auto check() -> bool {
    return check_impl(
        std::make_index_sequence<std::tuple_size_v<ast::Nodes>>());
  }
  template <std::size_t... Is>
  static constexpr auto check_impl(std::index_sequence<Is...>) -> bool {
    return std::is_same_v<T, ast::Expr> ||
           (std::is_same_v<T, std::tuple_element_t<Is, ast::Nodes>> || ...);
  }

 public:
  static constexpr bool value = check();
};

template <typename T>
constexpr bool is_ast_node_v = is_ast_node<T>::value;
}  // namespace reflect

namespace reflect {
template <typename T>
struct Access {
  static constexpr auto kSize = sizeof(T);
  static constexpr auto kClassID = T::kClassID;
  static constexpr auto kHasSuper =
      !std::is_same_v<typename T::super_type, void>;
  static constexpr auto kNumFields = T::field_list::size;

  using super_type = typename T::super_type;
  using class_id_type = typename T::class_id_type;

  template <std::size_t I>
  using FieldAt = typename T::field_list::template At<I>;
};
}  // namespace reflect

#endif  // REFLECT_ACCESS__H
