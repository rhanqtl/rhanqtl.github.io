#pragma once

#include "Support/Location/Location.h"
namespace xyz::ast {

class Node {
 public:
  virtual Location begin_pos() = 0;
  virtual Location end_pos() = 0;
};

}  // namespace xyz::ast
