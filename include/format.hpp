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

/**
 * Merges adjacent terminals into a single terminal
 */
template <typename tree>
struct reduce_terminals;

template <char... cs0, char... cs1, typename... ss>
struct reduce_terminals<ast::sequence<ast::terminal<cs0...>, ast::terminal<cs1...>, ss...>> {
  using type = typename reduce_terminals<ast::sequence<ast::terminal<cs0..., cs1...>, ss...>>::type;
};

template <typename s, typename... ss>
struct reduce_terminals<ast::sequence<s, ss...>> {
  using type = typename prepend<s, typename reduce_terminals<ast::sequence<ss...>>::type>::type;
};

template <>
struct reduce_terminals<ast::sequence<>> {
  using type = ast::sequence<>;
};

template <typename n, typename... ss>
struct reduce_terminals<ast::rule<n, ss...>> {
  using type = ast::rule<n, typename reduce_terminals<ss>::type...>;
};

template <typename... rs>
struct reduce_terminals<ast::rules<rs...>> {
  using type = ast::rules<typename reduce_terminals<rs>::type...>;
};

template <typename tree>
struct format {
  using squashed = typename squash<tree>::type;
  using reduced = typename reduce_terminals<squashed>::type;
  using type = reduced;
};

} // anonymous namespace

template <typename tree>
struct format_result {
  using type = typename format<tree>::type;
};

} // namespace bnfpg
