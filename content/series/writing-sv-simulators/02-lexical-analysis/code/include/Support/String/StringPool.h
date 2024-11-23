#ifndef SUPPORT_STRING_POOL_H
#define SUPPORT_STRING_POOL_H

#include <cstddef>
#include <string>
#include <string_view>

#include "Support/Utility/Singleton.h"

namespace xyz {

class StringRef;

class StringPool : public Singleton<StringPool> {
 public:
  StringRef make(const char *s);
};

class StringRef {
 public:
  StringRef(const std::string &s);
  StringRef(const char *s);
  StringRef(std::string_view s);
  StringRef(const StringRef &);

 public:
  /// \brief O(1)
  const char *c_str() const;
  /// \brief O(n)
  std::string to_string() const;

 private:
  std::size_t id_;
};

}  // namespace xyz

#endif  // SUPPORT_STRING_POOL_H
