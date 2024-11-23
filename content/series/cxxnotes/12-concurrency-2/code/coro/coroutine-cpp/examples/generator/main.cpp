#include <optional>

#include "coroutine/coroutine.h"

template <typename T>
class List {
 public:
  bool has_more() const;
  T &value();
  void next();
};

template <typename Seq>
void iterate(Seq &&seq) {
  while (seq.has_more()) {
    co::yield(seq.value());
    seq.next();
  }
}

int main() {
  auto xs = List<int>{1, 2, 3};
  for (const auto &x : iterate(xs)) {
    
  }
}
