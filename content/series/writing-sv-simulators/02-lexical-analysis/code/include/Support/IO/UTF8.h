#ifndef SUPPORT_IO_UTF8_H
#define SUPPORT_IO_UTF8_H

#include <cstdint>

class CodePoint {
 public:
  static CodePoint make_1byte(std::uint8_t);
  static CodePoint make_2bytes(std::uint8_t, std::uint8_t);
  static CodePoint make_3bytes(std::uint8_t, std::uint8_t, std::uint8_t);
  static CodePoint make_4bytes(std::uint8_t, std::uint8_t, std::uint8_t,
                               std::uint8_t);

  std::size_t nbytes() const;
  bool is_1byte() const;
  bool is_2bytes() const;
  bool is_3bytes() const;
  bool is_4bytes() const;

 private:
  std::uint8_t data_[0];
};

#endif  // SUPPORT_IO_UTF8_H
