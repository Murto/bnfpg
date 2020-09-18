#pragma once

#include <utility>

#include "ct_string.hpp"
#include "lex.hpp"

namespace bnfpg {

template <const char *cs>
struct grammar {
  using string = ct_string<cs>;
  using sequence = std::make_index_sequence<string::size>;
  using tokens = typename lex_result<string, sequence>::type;

  template <typename it_type>
  constexpr static bool valid([[maybe_unused]] it_type begin, [[maybe_unused]] it_type end) {
    return false;
  }
};

} // namespace bnfpg
