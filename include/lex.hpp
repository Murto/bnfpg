#pragma once

#include <utility>
#include "util.hpp"

namespace bnfpg {

namespace token {

/**
 * Word token
 */
template <char... cs>
struct word;

/**
 * String token
 */
template <char... cs>
struct string;

/**
 * Equals token
 */
struct equals;

/**
 * Comma token
 */
struct comma;

/**
 * Quote token
 */
struct quote;

/**
 * Pipe token
 */
struct pipe;

/**
 * Left bracket token
 */
struct left_bracket;

/**
 * Right bracket token
 */
struct right_bracket;

/**
 * Left brace token
 */
struct left_brace;

/**
 * Right brace token
 */
struct right_brace;

/**
 * Semicolon token
 */
struct semicolon;

}

namespace {

template <char c>
struct ch {
  constexpr static char value = c;
};

template <typename parts, typename chars>
struct lex_word;

/**
 * Handle generic character
 */
template <typename... ps, char c, typename... cs>
struct lex_word<list<ps...>, list<ch<c>, cs...>> {
  using result = lex_word<list<ps..., ch<c>>, list<cs...>>;
  using type = typename result::type;
  using unused = typename result::unused;
};

/**
 * Handle end of word cases
 */
template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<' '>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<' '>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'\t'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'\t'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'\n'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'\n'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'\r'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'\r'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'='>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'='>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<','>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<','>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'"'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'"'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'\''>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'\''>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'|'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'|'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'['>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'['>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<']'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<']'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'{'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'{'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<'}'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<'}'>, cs...>;
};

template <typename... ps, typename... cs>
struct lex_word<list<ps...>, list<ch<';'>, cs...>> {
  using type = token::word<ps::value...>;
  using unused = list<ch<';'>, cs...>;
};

/**
 * Lexes a string token
 */
template <typename parts, typename chars>
struct lex_string;

template <typename... cs>
struct lex_string<list<>, list<ch<'"'>, cs...>> {
  using string = lex_string<list<ch<'"'>>, list<cs...>>;
  using type = typename string::type;
  using unused = typename string::unused;
};

template <typename... ps, char c, typename... cs>
struct lex_string<list<ch<'"'>, ps...>, list<ch<c>, cs...>> {
  using string = lex_string<list<ch<'"'>, ps..., ch<c>>, list<cs...>>;
  using type = typename string::type;
  using unused = typename string::unused;
};

template <typename... ps, typename p, typename... cs>
struct lex_string<list<ch<'"'>, p, ps...>, list<ch<'"'>, cs...>> {
  using type = token::string<'"', p::value, ps::value...>;
  using unused = list<cs...>;
};

template <typename... cs>
struct lex_string<list<>, list<ch<'\''>, cs...>> {
  using string = lex_string<list<ch<'\''>>, list<cs...>>;
  using type = typename string::type;
  using unused = typename string::unused;
};

template <typename... ps, char c, typename... cs>
struct lex_string<list<ch<'\''>, ps...>, list<ch<c>, cs...>> {
  using string = lex_string<list<ch<'\''>, ps..., ch<c>>, list<cs...>>;
  using type = typename string::type;
  using unused = typename string::unused;
};

template <typename... ps, typename p, typename... cs>
struct lex_string<list<ch<'\''>, p, ps...>, list<ch<'\''>, cs...>> {
  using type = token::string<'\'', p::value, ps::value...>;
  using unused = list<cs...>;
};


template <typename tokens, typename chars>
struct lex;

/**
 * Handle whitespace cases
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<' '>, cs...>> {
  using type = typename lex<list<ts...>, list<cs...>>::type;
};

template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'\t'>, cs...>> {
  using type = typename lex<list<ts...>, list<cs...>>::type;
};

template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'\n'>, cs...>> {
  using type = typename lex<list<ts...>, list<cs...>>::type;
};

template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'\r'>, cs...>> {
  using type = typename lex<list<ts...>, list<cs...>>::type;
};

/**
 * Handle word case
 */
template <typename... ts, char c, typename... cs>
struct lex<list<ts...>, list<ch<c>, cs...>> {
  using word = lex_word<list<>, list<ch<c>, cs...>>;
  using type = typename lex<list<ts..., typename word::type>, typename word::unused>::type;

};

/**
 * Handle equals case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'='>, cs...>> {
  using type = typename lex<list<ts..., token::equals>, list<cs...>>::type;
};

/**
 * Handle comma case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<','>, cs...>> {
  using type = typename lex<list<ts..., token::comma>, list<cs...>>::type;
};

/**
 * Handle string cases
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'"'>, cs...>> {
  using string = lex_string<list<>, list<ch<'"'>, cs...>>;
  using type = typename lex<list<ts..., typename string::type>, typename string::unused>::type;
};

template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'\''>, cs...>> {
  using string = lex_string<list<>, list<ch<'\''>, cs...>>;
  using type = typename lex<list<ts..., typename string::type>, typename string::unused>::type;
};

/**
 * Handle pipe case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'|'>, cs...>> {
  using type = typename lex<list<ts..., token::pipe>, list<cs...>>::type;
};

/**
 * Handle left-bracket case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'['>, cs...>> {
  using type = typename lex<list<ts..., token::left_bracket>, list<cs...>>::type;
};

/**
 * Handle right-bracket case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<']'>, cs...>> {
  using type = typename lex<list<ts..., token::right_bracket>, list<cs...>>::type;
};

/**
 * Handle left-brace case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'{'>, cs...>> {
  using type = typename lex<list<ts..., token::left_brace>, list<cs...>>::type;
};

/**
 * Handle right-brace case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<'}'>, cs...>> {
  using type = typename lex<list<ts..., token::right_brace>, list<cs...>>::type;
};

/**
 * Handle semicolon case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<';'>, cs...>> {
  using type = typename lex<list<ts..., token::semicolon>, list<cs...>>::type;
};

/**
 * Handle end of input
 */
template <typename... ts>
struct lex<list<ts...>, list<>> {
  using type = list<ts...>;
};

} // anonymous namespace

template <typename string, typename sequence>
struct lex_result;

template <typename string, std::size_t... n>
struct lex_result<string, std::index_sequence<n...>> {
  using cs = list<ch<string::get(n)>...>;
  using type = typename lex<list<>, cs>::type;
};

} // namespace bnfpg
