#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "fmt/fmt.h"

TEST(libfmt, Basic) {
  auto res = cxxnotes::format("hello, {}", "libfmt");
  EXPECT_EQ(res, "hello, libfmt");
}

TEST(libfmt, Indexed) {
  auto res = cxxnotes::format("{1}{0}", "a", "b");
  EXPECT_EQ(res, "ab");
}

TEST(FormatArgumentShell, print_to) {
  std::unique_ptr<cxxnotes::FormatArgumentShell> a =
      std::make_unique<cxxnotes::FormatArgument<const char[2]>>("x");
}
TEST(fill_format_args, Basic) {
  std::vector<cxxnotes::FormatArgumentShell*> args;
  cxxnotes::fill_format_args(args, "x", "y", "z");
  ASSERT_EQ(args.size(), 3);
  ASSERT_EQ(args[0]->)
}
