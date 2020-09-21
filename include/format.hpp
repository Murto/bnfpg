#pragma once

#include "parse.hpp"

namespace bnfpg {

namespace {

/**
 * Expands grammar rules with many sequences into many grammar rules with
 * single sequences
 */
template <typename tree>
struct expand;

template <typename n, typename... ss>
struct expand<ast::rule<n, ss...>> {
  template <typename s> struct rulify { using type = ast::rule<n, s>; };
  using type = typename map<rulify, ast::rules<ss...>>::type;
};

template <typename... rs>
struct expand<ast::rules<rs...>> {
  using type = typename concat<typename expand<rs>::type...>::type;
};

template <typename n, typename s, typename... rs>
struct expand<ast::rules<ast::rule<n, s>, rs...>> {
  using rule = ast::rule<n, s>;
  using type = typename append<rule, typename expand<ast::rules<rs...>>::type>::type;
};

template <>
struct expand<ast::rules<>> {
  using type = ast::rules<>;
};

/**
 * Flattens singleton sequences into their nested ast component
 */
template <typename tree>
struct flatten {
  using type = tree;
};

template <typename s>
struct flatten<ast::none_or_one<ast::none_or_one<s>>> {
  using type = typename flatten<ast::none_or_one<s>>::type;
};

template <typename s>
struct flatten<ast::none_or_one<ast::none_or_more<s>>> {
  using type = typename flatten<ast::none_or_more<s>>::type;
};

template <typename s>
struct flatten<ast::none_or_more<ast::none_or_one<s>>> {
  using type = typename flatten<ast::none_or_more<s>>::type;
};

template <typename s>
struct flatten<ast::none_or_more<ast::none_or_more<s>>> {
  using type = typename flatten<ast::none_or_more<s>>::type;
};

template <typename s>
struct flatten<ast::none_or_one<ast::sequence<s>>> {
  using type = typename flatten<ast::none_or_one<s>>::type;
};

template <typename s>
struct flatten<ast::none_or_more<ast::sequence<s>>> {
  using type = typename flatten<ast::none_or_more<s>>::type;
};

template <typename... ss>
struct flatten<ast::sequence<ss...>> {
  using type = ast::sequence<typename flatten<ss>::type...>;
};

template <typename... rs>
struct flatten<ast::rules<rs...>> {
  using type = ast::rules<typename flatten<rs>::type...>;
};

/**
 * Simplifies grammar rules by removing `none_or_one`s, and instead generating
 * all possible rules.
 *
 * Note: Assumes that rules are a pair consisting of a name and a sequence.
 */
template <typename tree>
struct simplify;

template <>
struct simplify<ast::sequence<>> {
  using type = list<>;
};

template <typename s>
struct simplify {
  using type = ast::sequence<s>;
};

template <typename... ss0, typename... ss1>
struct simplify<ast::sequence<ast::none_or_one<ast::sequence<ss0...>>, ss1...>> {
  using nested = typename simplify<ast::sequence<ss0...>>::type;
  template <typename prefix> struct branchify { using type = typename simplify<typename concat<prefix, ast::sequence<ss1...>>::type>::type; };
  using type = typename map<branchify, nested>::type;
};

template <typename... ss, typename s>
struct simplify<ast::sequence<ast::none_or_one<s>, ss...>> {
  using next = typename simplify<ast::sequence<ss...>>::type;
  template <typename suffix> struct branchify { using type = typename concat<ast::none_or_one<s>, suffix>::type; };
  using type = typename concat<next, typename map<branchify, next>::type>::type;
};

template <typename n, typename s>
struct simplify<ast::rule<n, s>> {
  using branches = typename simplify<s>::type;
  template <typename suffix> struct rulify { using type = ast::rule<n, suffix>; };
  using type = typename rewrap<typename map<rulify, branches>::type, ast::rules>::type;
};

template <typename... rs>
struct simplify<ast::rules<rs...>> {
  using type = typename concat<typename simplify<rs>::type...>::type;
};

template <typename tree>
struct format {
  using expanded = typename expand<tree>::type;
  using flattened = typename flatten<expanded>::type;
  using simplified = typename simplify<flattened>::type;
  using type = simplified;
};

} // anonymous namespace

template <typename tree>
struct format_result {
  using type = typename format<tree>::type;
};

} // namespace bnfpg
