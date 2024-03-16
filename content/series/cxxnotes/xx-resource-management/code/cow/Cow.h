#ifndef COW_H
#define COW_H

#include <functional>
#include <utility>
#include <variant>

template <typename T>
class Cow {
 public:
  using owned_type = T;
  using borrowed_type = std::reference_wrapper<T>;

 private:
  template <typename... Args>
  Cow(Args &&...args);
  Cow(T *ptr);
  Cow(T &ref);

  ~Cow() = default;

 public:
  template <typename... Args>
  static Cow make_owned(Args &&...args);

  static Cow make_borrowed(T *ptr);
  static Cow make_borrowed(T &ref);

  bool is_borrowed() const;
  bool is_owned() const;

  const T &borrow() const;
  const T &borrow();
  T &borrow_mut();

 private:
  std::variant<owned_type, borrowed_type> data_;
};

template <typename T>
template <typename... Args>
Cow<T>::Cow(Args &&...args) : data_{owned_type{std::forward<Args>(args)...}} {}

template <typename T>
Cow<T>::Cow(T *ptr) : data_{borrowed_type{*ptr}} {}

template <typename T>
Cow<T>::Cow(T &ref) : data_{borrowed_type{ref}} {}

template <typename T>
bool Cow<T>::is_borrowed() const {
  return std::holds_alternative<borrowed_type>(data_);
}

template <typename T>
bool Cow<T>::is_owned() const {
  return std::holds_alternative<owned_type>(data_);
}

#endif  // COW_H
