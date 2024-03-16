#include "Debug.h"

#include <map>
#include <vector>

using namespace std;

int main() {

  int x = dbg(42);
  dbg(x);

  vector xs = {1, 2, 3, 4};
  dbg(xs);

  map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
  dbg(m);
}
