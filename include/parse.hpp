#pragma once

#include "lex.hpp"
#include "util.hpp"

namespace bnfpg {

namespace ast {

template <char... cs>
struct symbol;

template<char... cs>
struct terminal;

template <typename... asts>
struct sequence;

template <typename name, typename... ss>
struct rule;

template <typename... rs>
struct rules;

} // namespace ast

namespace {

/**
 * Parses a single sequence on the right-hand side of a rule
 */
template <typename parts, typename tokens>
struct parse_sequence;

template <typename... ps, char... cs, typename... ts>
struct parse_sequence<ast::sequence<ps...>, list<token::symbol<cs...>, ts...>> {
  using next = parse_sequence<ast::sequence<ps..., ast::symbol<cs...>>, list<ts...>>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename... ps, char... cs, typename... ts>
struct parse_sequence<ast::sequence<ps...>, list<token::string<cs...>, ts...>> {
  using next = parse_sequence<ast::sequence<ps..., ast::terminal<cs...>>, list<ts...>>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename... ps, typename... ts>
struct parse_sequence<ast::sequence<ps...>, list<ts...>> {
  static_assert(size<list<ps...>>::value > 0, "Sequence expected");
  using type = ast::sequence<ps...>;
  using unused = list<ts...>;
};

/**
 * Parses all sequences on the right-hand side of a rule
 */
template <typename sequences, typename tokens>
struct parse_sequences;

template <typename... ts>
struct parse_sequences<list<>, list<ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using next = parse_sequences<list<typename sequence::type>, typename sequence::unused>;
  using type = typename next::type;
  using unused = typename next::unused;

};

template <typename s, typename... ss, typename... ts>
struct parse_sequences<list<s, ss...>, list<token::pipe, ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using next = parse_sequences<list<s, ss..., typename sequence::type>, typename sequence::unused>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename s, typename... ss, typename... ts>
struct parse_sequences<list<s, ss...>, list<token::semicolon, ts...>> {
  using type = list<s, ss...>;
  using unused = list<ts...>;
};

/**
 * Parses a single grammar rule from a token list
 */
template <typename parts, typename tokens>
struct parse_rule;

template <char... cs, typename... ts>
struct parse_rule<list<>, list<token::symbol<cs...>, token::equals, ts...>> {

  // Parse right hand side
  using sequences = parse_sequences<list<>, list<ts...>>;

  template <typename... ss>
  using rule_wrapper = ast::rule<ast::symbol<cs...>, ss...>;

  using type = typename rewrap<typename sequences::type, rule_wrapper>::type;

  using unused = typename sequences::unused;
};

/**
 * Parses all grammar rules from a token list
 */
template <typename rules, typename tokens>
struct parse_rules;

template <typename... rs, typename... ts>
struct parse_rules<list<rs...>, list<ts...>> {
  using rule = parse_rule<list<>, list<ts...>>;
  using type = typename parse_rules<list<rs..., typename rule::type>, typename rule::unused>::type;
  
};

template <typename... rs>
struct parse_rules<list<rs...>, list<>> {
  using type = ast::rules<rs...>;
};

template <typename tokens>
struct parse {
  using type = typename parse_rules<list<>, tokens>::type;
};

} // anonymous namespace

template <typename tokens>
struct parse_result {
  using type = typename parse<tokens>::type;
};

} // namespace bnfpg
