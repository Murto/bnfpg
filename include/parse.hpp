#pragma once

#include "lex.hpp"
#include "util.hpp"

namespace bnfpg {

namespace ast {

template <char... cs>
struct word;

template <typename... words>
struct symbol;

template<char... cs>
struct terminal;

template <typename... asts>
struct sequence;

template <typename sequence>
struct none_or_one;

template <typename sequence>
struct none_or_more;

template <typename name, typename... ss>
struct rule;

template <typename... rs>
struct rules;

} // namespace ast

namespace {

/**
 * Consumes a single expected type from the head of a list
 */
template <typename expected, typename types>
struct consume;

template <typename expected, typename... types>
struct consume<expected, list<expected, types...>> {
  using type = list<types...>;
};

/**
 * Parses a single symbol from a token list
 */
template <typename parts, typename tokens>
struct parse_symbol;

template <typename... ws, char... cs, typename... ts>
struct parse_symbol<list<ws...>, list<token::word<cs...>, ts...>> {
  using next = parse_symbol<list<ws..., ast::word<cs...>>, list<ts...>>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename... ws, typename... ts>
struct parse_symbol<list<ws...>, list<ts...>> {
  using type = ast::symbol<ws...>;
  using unused = list<ts...>;
};

/**
 * Parses a single sequence on the right-hand side of a rule
 */
template <typename parts, typename tokens>
struct parse_sequence;

template <char... cs, typename... ts>
struct parse_sequence<ast::sequence<>, list<token::word<cs...>, ts...>> {
  using symbol = parse_symbol<list<>, list<token::word<cs...>, ts...>>;
  using next = parse_sequence<ast::sequence<typename symbol::type>, typename symbol::unused>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <char... cs, typename... ts>
struct parse_sequence<ast::sequence<>, list<token::string<cs...>, ts...>> {
  using next = parse_sequence<ast::sequence<ast::terminal<cs...>>, list<ts...>>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename... ts>
struct parse_sequence<ast::sequence<>, list<token::left_bracket, ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using consumed = typename consume<token::right_bracket, typename sequence::unused>::type;
  using next = parse_sequence<ast::sequence<ast::none_or_one<typename sequence::type>>, consumed>;
  using type = typename next::type;
  using unused = typename next::unused;
};
template <typename... ts>
struct parse_sequence<ast::sequence<>, list<token::left_brace, ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using consumed = typename consume<token::right_brace, typename sequence::unused>::type;
  using next = parse_sequence<ast::sequence<ast::none_or_more<typename sequence::type>>, consumed>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename p, typename... ps, char... cs, typename... ts>
struct parse_sequence<ast::sequence<p, ps...>, list<token::comma, token::word<cs...>, ts...>> {
  using symbol = parse_symbol<list<>, list<token::word<cs...>, ts...>>;
  using next = parse_sequence<ast::sequence<p, ps..., typename symbol::type>, typename symbol::unused>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename p, typename... ps, char... cs, typename... ts>
struct parse_sequence<ast::sequence<p, ps...>, list<token::comma, token::string<cs...>, ts...>> {
  using next = parse_sequence<ast::sequence<p, ps..., ast::terminal<cs...>>, list<ts...>>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename p, typename... ps, typename... ts>
struct parse_sequence<ast::sequence<p, ps...>, list<token::comma, token::left_bracket, ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using consumed = typename consume<token::right_bracket, typename sequence::unused>::type;
  using next = parse_sequence<ast::sequence<p, ps..., ast::none_or_one<typename sequence::type>>, consumed>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename p, typename... ps, typename... ts>
struct parse_sequence<ast::sequence<p, ps...>, list<token::comma, token::left_brace, ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using consumed = typename consume<token::right_brace, typename sequence::unused>::type;
  using next = parse_sequence<ast::sequence<p, ps..., ast::none_or_more<typename sequence::type>>, consumed>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename p, typename... ps, typename... ts>
struct parse_sequence<ast::sequence<p, ps...>, list<ts...>> {
  using type = ast::sequence<p, ps...>;
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

template <typename... ss, typename... ts>
struct parse_sequences<list<ss...>, list<token::pipe, ts...>> {
  using sequence = parse_sequence<ast::sequence<>, list<ts...>>;
  using next = parse_sequences<list<ss..., typename sequence::type>, typename sequence::unused>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename... ss, typename... ts>
struct parse_sequences<list<ss...>, list<token::semicolon, ts...>> {
  using type = ast::sequence<ss...>;
  using unused = list<ts...>;
};

/**
 * Parses a single grammar rule from a token list
 */
template <typename parts, typename tokens>
struct parse_rule;

template <typename... ts>
struct parse_rule<list<>, list<ts...>> {

  // Parse nonterminal symbol
  using symbol = parse_symbol<list<>, list<ts...>>;

  // Consume equals token
  using consumed = typename consume<token::equals, typename symbol::unused>::type;

  // Parse rules for nonterminal
  using sequences = parse_sequences<list<>, consumed>;

  template <typename... ss>
  using rule_wrapper = ast::rule<typename symbol::type, ss...>;

  using type = typename rewrap<typename sequences::type, rule_wrapper>::type;

  //using yeet = typename debug<type>::type;

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

/**
 * Parses an EBNF grammar
 */
template <typename rules, typename tokens>
struct parse;

template <typename... rs, typename... ts>
struct parse<list<rs...>, list<ts...>> {
  using type = typename parse_rules<list<>, list<ts...>>::type;
};

/**
 * Return list of rules once all tokens are consumed
 */
template <typename... rs>
struct parse<list<rs...>, list<>> {
  using type = ast::rules<rs...>;
};

} // anonymous namespace

template <typename tokens>
struct parse_result {
  using type = typename parse<list<>, tokens>::type;
};

} // namespace bnfpg
