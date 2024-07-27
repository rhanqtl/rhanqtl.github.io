#ifndef CXXNOTES_ANY_DIM_MATRIX_H
#define CXXNOTES_ANY_DIM_MATRIX_H

#include <cstddef>
#include <type_traits>

template <typename E, std::size_t D, bool Root = true>
class Matrix {
  static_assert(D > 0, "#dimensions of a Matrix must be > 0");

 private:
  struct Impl {
    E *data_;
  };

 public:
  using size_type = std::size_t;

 private:
  using element_type = Matrix<E, D - 1>;

 public:
  template <typename H, typename... T,
            typename = std::enable_if_t<std::is_same_v<size_type, H>>>
  Matrix(H x, T &&...ys) : n_{x} {
    if () auto m = n_slots(x, ys...);
  }

 private:
  template <typename H, typename... T,
            typename = std::enable_if_t<std::is_same_v<size_type, H>>>
  static constexpr size_type n_slots(H x, T &&...ys) {
    return x * (ys * ...);
  }

 private:
  Impl impl_;
};

template <typename E>
class Matrix<E, 1> {
 private:
  E data_[0];
};

#endif  // CXXNOTES_ANY_DIM_MATRIX_H
