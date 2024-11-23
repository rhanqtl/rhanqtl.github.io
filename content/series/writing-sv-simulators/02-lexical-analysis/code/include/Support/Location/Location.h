#pragma once

#include <cstdint>
#include <string>

namespace xyz {

class Location {
 public:
  Location(uint32_t file_id, uint32_t line, uint32_t column);

 public:
  const char *file() const;
  const char *absolute_file() const;
  const char *line() const;
  const char *column() const;

 public:
  std::string to_string() const;

 private:
  uint32_t file_id_;
  uint32_t line_;
  uint32_t column_;
};

}  // namespace xyz
