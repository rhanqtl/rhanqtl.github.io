#ifndef SUPPORT_IO_INPUT_STREAM_H
#define SUPPORT_IO_INPUT_STREAM_H

#include <cstdint>
#include <string>

#include "Support/Utility/Expected.h"

namespace xyz {

template <typename CharT>
class InputStream {
 public:
  virtual Expected<CharT> read_chars(std::size_t n = 1) = 0;
};

}  // namespace xyz

#endif  // SUPPORT_IO_INPUT_STREAM_H
