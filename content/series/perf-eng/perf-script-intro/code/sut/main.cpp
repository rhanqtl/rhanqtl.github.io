#include <assert.h>

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <string>

enum class fill_random_data_tag {};
template <typename T>
T generate_random() {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<T> dist(-100, 100);
  return dist(gen);
}

template <class T, typename Alloc>
class MatrixImpl {
 public:
  using size_type = std::size_t;
  using value_type = T;
  using pointer = value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;

  using allocator_type = Alloc;

 protected:
  MatrixImpl(size_type n) : data_{allocator_type().allocate(n)}, n_{n} {}
  ~MatrixImpl() { allocator_type().deallocate(data_, n_); }

  void fill_with_random() {
    std::cout << n_ << '\n';
    get_allocator().construct(data_, 1);
    for (size_type i = 1; i < n_; i++) {
      get_allocator().construct(data_ + i, data_[i - 1] * 2 + 1);
    }
  }

 protected:
  reference at(size_type i) { return data_[i]; }
  const_reference at(size_type i) const { return data_[i]; }

 private:
  allocator_type get_allocator() { return allocator_type{}; }

 protected:
  T *const data_;
  const size_type n_;
};

// template <typename T>
// class Vector : MatrixImpl<T> {
//  private:
//   using impl_type = MatrixImpl<T>;

//  public:
//   using size_type = typename MatrixImpl<T>::size_type;
//   using value_type = typename MatrixImpl<T>::value_type;
//   using reference = typename impl_type::reference;
//   using const_reference = typename impl_type::const_reference;

//  public:
//   Vector(size_type n, const T &val = T()) : MatrixImpl<T>(n, val), n_{n} {
//     auto *p = impl_type::data_;
//     for (size_type i = 0; i < n; i++) {
//       *p++ = val;
//     }
//   }
//   Vector(std::initializer_list<T> init)
//       : MatrixImpl<T>(init.size(), T()), n_{init.size()} {
//     std::copy_n(init.begin(), init.end(), impl_type::data_);
//   }

//  public:
//   reference operator()(size_type i) { return impl_type::at(i); }
//   const value_type &operator()(size_type i) const {
//     return *(MatrixImpl<T>::data_ + i);
//   }

//  private:
//   size_type n_;
// };

template <typename T, typename Alloc = std::allocator<T>>
class Matrix : MatrixImpl<T, Alloc> {
 private:
  using impl_type = MatrixImpl<T, Alloc>;

 public:
  using size_type = typename impl_type::size_type;
  using value_type = typename impl_type::value_type;
  using reference = typename impl_type::reference;
  using const_reference = typename impl_type::const_reference;

 public:
  Matrix(size_type n_rows, size_type n_cols, const T &val = T())
      : MatrixImpl<T, Alloc>(n_rows * n_cols), nr_{n_rows}, nc_{n_cols} {
    auto *p = impl_type::data_;
    for (size_type i = 0; i < n_rows; i++) {
      for (size_type j = 0; j < n_cols; j++) {
        *p++ = val;
      }
    }
  }
  // Matrix(std::initializer_list<std::initializer_list<T>> init)
  //     : MatrixImpl<T>(init.size() * init.begin()->size(), T()),
  //       nr_{init.size()},
  //       nc_{init.begin()->size()} {
  //   auto *p = impl_type::data_;
  //   for (auto i = init.begin(); i != init.end(); i++) {
  //     assert(i->size() == nc_);
  //     for (auto j = i->begin(); j != i->end(); ++j) {
  //       *p++ = *j;
  //     }
  //   }
  // }
  Matrix(size_type n_rows, size_type n_cols, fill_random_data_tag)
      : MatrixImpl<T, Alloc>(n_rows * n_cols), nr_{n_rows}, nc_{n_cols} {
    impl_type::fill_with_random();
  }

 public:
  size_type n_rows() const { return nr_; }
  size_type n_columns() const { return nc_; }
  size_type n_elements() const { return n_rows() * n_columns(); }

  reference operator()(size_type i, size_type j) {
    return impl_type::at(i * n_columns() + j);
  }
  const_reference operator()(size_type i, size_type j) const {
    return impl_type::at(i * n_columns() + j);
  }

  //  public:
  //   class RowIterator {
  //    public:
  //      reference operator*
  //   };
  //   friend RowIterator;

 private:
  const size_type nr_, nc_;
};

template <typename T>
inline Matrix<T> operator*(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  assert(lhs.n_columns() == rhs.n_rows());

  Matrix<T> ans(lhs.n_rows(), rhs.n_columns());
  for (typename Matrix<T>::size_type i = 0; i < lhs.n_rows(); i++) {
    for (typename Matrix<T>::size_type j = 0; j < rhs.n_columns(); j++) {
      for (typename Matrix<T>::size_type k = 0; k < lhs.n_columns(); k++) {
        ans(i, j) += lhs(i, k) * rhs(k, j);
      }
    }
  }
  return ans;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Matrix<T> &m) {
  for (typename Matrix<T>::size_type i = 0; i < m.n_rows(); i++) {
    if (i != 0) out << '\n';
    for (typename Matrix<T>::size_type j = 0; j < m.n_columns(); j++) {
      out << '\t' << m(i, j);
    }
  }
  return out;
}

int main(int argc, char *argv[]) {
  size_t info[4] = {128, 128, 128, 128};
  for (int i = 1; i < argc; i++) {
    auto res = std::stoul(argv[i]);
    if (res > 0) info[i - 1] = res;
  }
  Matrix<int> lhs(info[0], info[1], fill_random_data_tag());
  Matrix<int> rhs(info[2], info[3], fill_random_data_tag());

  std::cout << (lhs * rhs) << '\n';
}
