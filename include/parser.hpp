#pragma once

#include "ct_string.hpp"
#include "lex.hpp"

namespace bnfpg {

template <const char *cs>
struct parser {
  using string = ct_string<cs>;
  using sequence = std::make_index_sequence<string::size>;
  using tokens = typename lex_result<string, sequence>::type;
};

} // namespace bnfpg
