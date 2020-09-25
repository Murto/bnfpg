#pragma once

#include <type_traits>

#include "parse.hpp"
#include "util.hpp"

namespace bnfpg {

namespace {

/**
 * Collects all symbols used on the left-hand-side of the grammar
 */
template <typename tree>
struct left_symbols;

template <typename n, typename... ss, typename... rs>
struct left_symbols<ast::rules<ast::rule<n, ss...>, rs...>> {
  using type = typename prepend<n, typename left_symbols<ast::rules<rs...>>::type>::type;
};

template <>
struct left_symbols<ast::rules<>> {
  using type = list<>;
};

/**
 * Collects all symbols used on the right-hand-side of rules in the grammar
 */
template <typename tree>
struct right_symbols {
  using type = list<>;
};

template <char... cs>
struct right_symbols<ast::symbol<cs...>> {
  using type = list<ast::symbol<cs...>>;
};

template <typename... ss>
struct right_symbols<ast::sequence<ss...>> {
  using type = typename concat<typename right_symbols<ss>::type...>::type;
};

template <typename n, typename... ss>
struct right_symbols<ast::rule<n, ss...>> {
  using type = typename concat<typename right_symbols<ss>::type...>::type;
};

template <typename... rs>
struct right_symbols<ast::rules<rs...>> {
  using type = typename concat<typename right_symbols<rs>::type...>::type;
};


/**
 * Checks if all symbols used within productions are defined
 */
template <typename tree>
struct symbols_defined {
  using lefts = typename left_symbols<tree>::type;
  using rights = typename right_symbols<tree>::type;
  constexpr static const bool value = size<typename difference<rights, lefts>::type>::value == 0;
};

} // anonymous namespace

template <typename tree>
struct validate_result {
  static_assert(symbols_defined<tree>::value, "Some symbols undefined");
  using type = tree;
};

} // namespace bnfpg
