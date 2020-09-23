#pragma once

#include <type_traits>

#include "parse.hpp"
#include "util.hpp"

namespace bnfpg {

namespace {

template <typename rule0, typename rule1>
struct reduce_rule;

template <typename n0, typename... ss0, typename n1, typename... ss1>
struct reduce_rule<ast::rule<n0, ss0...>, ast::rule<n1, ss1...>> {
  using type = ast::rule<n1, ss0..., ss1...>;
};

template <typename rules>
using reduce_rules = foldl<reduce_rule, ast::rule<void>, rules>;

template <typename rule>
struct symbol_of;

template <typename n, typename... ss>
struct symbol_of<ast::rule<n, ss...>> {
  using type = n;
};

template <typename symbol, typename rules>
struct rules_of {
  template <typename rule>
  struct pred {
    constexpr static const bool value = std::is_same<symbol, typename symbol_of<rule>::type>::value;
  };
  using type = typename filter<pred, rules>::type;
};

/**
 * Squashes `ast::rule`s for the same nonterminal into a single `ast::rule`
 */
template <typename tree>
struct squash;

template <typename... rs>
struct squash<ast::rules<rs...>> {

  // Get unique list of nonterminals
  using symbols = typename unique<ast::rules<typename symbol_of<rs>::type...>>::type;

  template <typename symbol>
  using pred_wrapper = rules_of<symbol, ast::rules<rs...>>;

  // Constuct a list of predicates, each associated with a unique nonterminal
  using rule_map = typename map<pred_wrapper, symbols>::type;

  using type = typename map<reduce_rules, rule_map>::type;
};

template <typename tree>
struct format {
  using type = typename squash<tree>::type;
};

} // anonymous namespace

template <typename tree>
struct format_result {
  using type = typename format<tree>::type;
};

} // namespace bnfpg
