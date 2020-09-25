#pragma once

#include <utility>

#include "ct_string.hpp"
#include "format.hpp"
#include "lex.hpp"
#include "parse.hpp"
#include "validate.hpp"

namespace bnfpg {

template <const char *cs>
struct grammar {
  using string = ct_string<cs>;
  using sequence = std::make_index_sequence<string::size>;
  using tokens = typename lex_result<string, sequence>::type;
  using tree = typename parse_result<tokens>::type;
  using validated = typename validate_result<tree>::type;
  using formatted = typename format_result<validated>::type;

  template <typename it_type>
  constexpr static bool valid([[maybe_unused]] it_type begin, [[maybe_unused]] it_type end) {
    return false;
  }
};

} // namespace bnfpg
