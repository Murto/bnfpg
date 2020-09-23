#pragma once

#include <type_traits>
#include <utility>

#include "util.hpp"

namespace bnfpg {

namespace token {

/**
 * Word token
 */
template <char... cs>
struct symbol;

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
 * Pipe token
 */
struct pipe;

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

template <char c>
struct valid_symbol_char {
  constexpr static const bool value = c == '_' || c == '-' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
};

/**
 * Handles lexing of symbols
 */
template <typename chars, typename symbol = token::symbol<>, bool valid = true>
struct lex_symbol;

template <char c0, char c1, typename... cs, char... ps>
struct lex_symbol<list<ch<c0>, ch<c1>, cs...>, token::symbol<ps...>, true> {
  static_assert(valid_symbol_char<c0>::value, "Invalid symbol character");
  using next = lex_symbol<list<ch<c1>, cs...>, token::symbol<ps..., c0>, valid_symbol_char<c1>::value>;
  using type = typename next::type;
  using unused = typename next::unused;
};

template <typename... cs, char... ps>
struct lex_symbol<list<cs...>, token::symbol<ps...>, false> {
  using type = token::symbol<ps...>;
  using unused = list<cs...>;
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

template <typename... ps, typename... cs>
struct lex_string<list<ch<'"'>, ps...>, list<ch<'"'>, cs...>> {
  using type = token::string<ps::value...>;
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

template <typename... ps, typename... cs>
struct lex_string<list<ch<'\''>, ps...>, list<ch<'\''>, cs...>> {
  using type = token::string<ps::value...>;
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
 * Handle symbol case
 */
template <typename... ts, char c, typename... cs>
struct lex<list<ts...>, list<ch<c>, cs...>> {
  using symbol = lex_symbol<list<ch<c>, cs...>>;
  using type = typename lex<list<ts..., typename symbol::type>, typename symbol::unused>::type;

};

/**
 * Handle equals case
 */
template <typename... ts, typename... cs>
struct lex<list<ts...>, list<ch<':'>, ch<':'>, ch<'='>, cs...>> {
  using type = typename lex<list<ts..., token::equals>, list<cs...>>::type;
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
