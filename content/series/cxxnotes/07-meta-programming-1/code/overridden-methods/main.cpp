#include <iostream>
#include <type_traits>

template <typename T, typename U>
struct is_different : std::integral_constant<bool, !std::is_same_v<T, U>> {};

template <typename T, typename U>
using is_different_t = typename is_different<T, U>::type;

template <typename T, typename U>
constexpr bool is_different_v = is_different<T, U>::value;

struct Base {
  virtual void foo() = 0;
  virtual void bar() = 0;
  virtual void qux(int) = 0;

  void buzz();
};

struct Derived1 : Base {
  void foo() override;
  void qux(int) override;
};

struct Derived2 : Base {
  void bar() override;
};

struct Derived3 : Base {
  void buzz();
};

template <typename T>
struct overrides_foo : is_different<decltype(&Base::foo), decltype(&T::foo)> {};

template <typename T> constexpr bool overrides_foo_v = overrides_foo<T>::value;

template <typename T>
struct overrides_bar : is_different<decltype(&Base::bar), decltype(&T::bar)> {};

template <typename T> constexpr bool overrides_bar_v = overrides_bar<T>::value;

template <typename T>
struct overrides_qux : is_different<decltype(&Base::qux), decltype(&T::qux)> {};

template <typename T> constexpr bool overrides_qux_v = overrides_qux<T>::value;

template <typename T>
struct overrides_buzz
    : is_different<decltype(&Base::buzz), decltype(&T::buzz)> {};

template <typename T>
constexpr bool overrides_buzz_v = overrides_buzz<T>::value;

int main() {
  std::cout << std::boolalpha << overrides_foo_v<Base> << ' '
            << overrides_bar_v<Base> << ' ' << overrides_qux_v<Base> << ' '
            << overrides_buzz_v<Base> << '\n';
  std::cout << std::boolalpha << overrides_foo_v<Derived1> << ' '
            << overrides_bar_v<Derived1> << ' '
            << overrides_qux_v<Derived1> << ' '
            << overrides_buzz_v<Derived1> << '\n';
  std::cout << std::boolalpha << overrides_foo_v<Derived2> << ' '
            << overrides_bar_v<Derived2> << ' '
            << overrides_qux_v<Derived2> << ' '
            << overrides_buzz_v<Derived2> << '\n';
  std::cout << std::boolalpha << overrides_foo_v<Derived3> << ' '
            << overrides_bar_v<Derived3> << ' '
            << overrides_qux_v<Derived3> << ' '
            << overrides_buzz_v<Derived3> << '\n';
}
