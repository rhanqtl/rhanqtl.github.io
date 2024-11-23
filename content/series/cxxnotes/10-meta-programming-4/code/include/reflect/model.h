#ifndef REFLECT_MODEL__H
#define REFLECT_MODEL__H

#include <array>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/is_begin_parens.hpp>
#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace reflect {
template <typename C, typename T, T C::*P>
struct Field {
  using type = T;

  static constexpr decltype(P) pointer = P;
};

template <typename... Ts>
struct FieldList {
  using tuple = std::tuple<Ts...>;
  static constexpr std::size_t kSize = sizeof...(Ts);

  template <std::size_t I>
  using At = std::tuple_element_t<I, tuple>;
};
}  // namespace reflect

#define _FIELD(_r, cls, i, field) \
  BOOST_PP_COMMA_IF(i)            \
  reflect::Field<cls, decltype(field), &cls::field>

#define _FIELD_NAME(_r, cls, i, field)                     \
  BOOST_PP_COMMA_IF(i) {                                   \
    BOOST_PP_STRINGIZE(cls) "::" BOOST_PP_STRINGIZE(field) \
  }

#define META_INFO(cls, id, super, ...)                                    \
  using class_id_type = decltype(id);                                     \
  static constexpr int kClassID = static_cast<int>(id);                   \
  using super_type = super;                                               \
  using field_list = reflect::FieldList<BOOST_PP_SEQ_FOR_EACH_I(          \
      _FIELD, cls, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))>;               \
  static constexpr std::string_view kClassName = BOOST_PP_STRINGIZE(cls); \
  static constexpr std::array<std::string_view,                           \
                              BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)>        \
      kFieldNames = {{BOOST_PP_SEQ_FOR_EACH_I(                            \
          _FIELD_NAME, cls, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))}}

#endif  // REFLECT_MODEL__H
