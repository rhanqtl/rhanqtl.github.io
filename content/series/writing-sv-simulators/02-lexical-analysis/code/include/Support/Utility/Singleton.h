#pragma once

namespace xyz {

template <typename T>
class Singleton {
 public:
  static T *instance();
};

}  // namespace xyz
