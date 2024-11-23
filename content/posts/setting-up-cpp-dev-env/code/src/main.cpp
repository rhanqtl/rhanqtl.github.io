#include <iostream>
#include <string>

#include "demo/Support/Strings.h"

int main(int argc, char *argv[]) {
  std::string s;
  if (argc < 2) {
    s = "hello, world";
  } else {
    s = argv[1];
  }
  std::cout << "original: " << s << '\n'
            << "reversed: " << demo::strings::reverse(s) << std::endl;
}
