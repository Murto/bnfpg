#pragma once

#include <utility>
#include "util.hpp"

namespace bnfpg {

namespace {

template <char c>
struct ch;

} // anonymous namespace

template <typename string, typename sequence>
struct lex_result;

template <typename string, std::size_t... n>
struct lex_result<string, std::index_sequence<n...>> {
  using chs = list<ch<string::get(n)>...>;
  using type = char;
};

} // namespace bnfpg
