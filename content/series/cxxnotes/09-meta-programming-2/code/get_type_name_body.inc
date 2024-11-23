auto func = std::string_view{__PRETTY_FUNCTION__};
#if defined(__clang__)
  constexpr std::size_t OFFSET = 38;
#else
# error "compiler not supported"
#endif
  return func.substr(OFFSET, func.length() - OFFSET - 1);
