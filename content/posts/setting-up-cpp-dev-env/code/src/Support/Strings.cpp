#include "demo/Support/Strings.h"

#include <algorithm>

namespace demo::strings {
auto reverse(std::string_view s) -> std::string {
  auto ans = std::string{s};
  std::reverse(ans.begin(), ans.end());
  return ans;
}
}  // namespace demo::strings
