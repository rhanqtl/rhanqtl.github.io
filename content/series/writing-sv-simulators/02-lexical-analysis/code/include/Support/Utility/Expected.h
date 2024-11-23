#ifndef SUPPORT_UTILITY_EXPECTED_H
#define SUPPORT_UTILITY_EXPECTED_H

namespace xyz {

template <typename T>
class Expected {
 public:
  T& operator*();
  const T& operator*() const;

  operator bool() const { return false; }
};

}  // namespace xyz

#endif  // SUPPORT_UTILITY_EXPECTED_H
