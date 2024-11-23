#include <optional>

#include "coroutine/coroutine.h"

void integers(int begin = 0, std::optional<int> end = std::nullopt,
              int step = 1) {
  if (end) {
    for (; begin != end.value(); begin += step) {
      co::yield(begin);
    }
  } else {
    for (; true; begin += step) {
      co::yield(begin);
    }
  }
}

int main() {}
