#ifndef SUPPORT_IO_UTF8_INPUT_STREAM_H
#define SUPPORT_IO_UTF8_INPUT_STREAM_H

#include <cstddef>
#include <cstdint>

#include "Support/IO/InputStream.h"
#include "Support/Utility/Expected.h"

namespace xyz {

using Rune = int32_t;

class UTF8InputStream : public InputStream<Rune> {
 public:
  Expected<Rune> read_chars(std::size_t) override;
};

}  // namespace xyz

#endif  // SUPPORT_IO_UTF8_INPUT_STREAM_H
