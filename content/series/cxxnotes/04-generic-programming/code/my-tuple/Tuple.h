#ifndef CXXNOTES_TUPLE_H
#define CXXNOTES_TUPLE_H

#include <type_traits>

class Unit {};

template <typename Head, typename... Tail>
class Tuple
    : public std::conditional_t<sizeof...(Tail) == 0, Unit, Tuple<Tail...>> {
 public:
 private:
  Head data_;
};

#endif  // CXXNOTES_TUPLE_H
