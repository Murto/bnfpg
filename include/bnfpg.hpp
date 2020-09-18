#pragma once

#include <string>

#include "grammar.hpp"

namespace bnfpg {

namespace {

const char* begin(const char* str) {
  return str;
}

const char* end(const char* str) {
  while (*str != '\0') {
    ++str;
  }
  return str;
}

} // anonymous namespace

/**
 * Validates an input sequence only
 */
template <typename grammar, typename it_type>
bool valid(it_type begin, it_type end) noexcept {
  return grammar::valid(begin, end);
}

template <typename grammar>
bool valid(const char* str) noexcept {
  return valid<grammar>(begin(str), end(str));
}

template <typename grammar>
bool valid(std::string str) noexcept {
  return valid<grammar>(str.begin(), str.end());
}

} // namespace bnfpg
