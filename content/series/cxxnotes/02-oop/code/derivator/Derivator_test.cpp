#include "Derivator.h"

#include <gtest/gtest.h>

#include "AST.h"
#include "Ops.h"

TEST(Derivator, _) {
  auto &x = ExprManager::mk_symbol("x");
  auto &f = (x ^ 2) + 2 * x + 1;
  ASSERT_EQ(f.to_str(), "x²＋2x＋1");

  Derivator d;
  auto &f_prime = d.run(f);
  ASSERT_EQ(f_prime.to_str(), "2x＋2");
}
