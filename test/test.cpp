#include <cassert>

#include "bnfpg.hpp"

constexpr static const char digit[] =
  "digit ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;";

constexpr static const char arithmetic[] =

  // Nonterminals
  "expr     ::= sub-expr ;"
  "sub-expr ::= sub-expr minus add-expr ;"
  "sub-expr ::= add-expr ;"
  "add-expr ::= add-expr plus mul-expr ;"
  "add-expr ::= mul-expr ;"
  "mul-expr ::= mul-expr times div-expr ;"
  "mul-expr ::= div-expr ;"
  "div-expr ::= div-expr divide neg-expr ;"
  "div-expr ::= neg-expr ;"
  "neg-expr ::= minus par-expr ;"
  "neg-expr ::= par-expr ;"
  "par-expr ::= left-par expr right-par ;"
  "par-expr ::= number ;"
  "number   ::= zero ;"
  "number   ::= non-zero digits ;"
  "digits   ::= digit digits ;"
  "digits   ::= '' ;"
  "digit    ::= non-zero ;"
  "digit    ::= zero ;"

  // Terminals
  "minus     ::= '-' ;"
  "plus      ::= '+' ;"
  "times     ::= '*' ;"
  "divide    ::= '/' ;"
  "left-par  ::= '(' ;"
  "right-par ::= ')' ;"
  "zero      ::= '0' ;"
  "non-zero  ::= '1' ;"
  "non-zero  ::= '2' ;"
  "non-zero  ::= '3' ;"
  "non-zero  ::= '4' ;"
  "non-zero  ::= '5' ;"
  "non-zero  ::= '6' ;"
  "non-zero  ::= '7' ;"
  "non-zero  ::= '8' ;"
  "non-zero  ::= '9' ;";

  constexpr static const char compact_arithmetic[] =

  // Nonterminals
  "expr ::= sub-expr ;"
  "sub-expr ::= sub-expr minus add-expr | add-expr ;"
  "add-expr ::= add-expr plus mul-expr | mul-expr ;"
  "mul-expr ::= mul-expr times div-expr | div-expr ;"
  "div-expr ::= div-expr divide neg-expr | neg-expr ;"
  "neg-expr ::= minus par-expr | par-expr ;"
  "par-expr ::= left-par expr right-par | number ;"
  "number   ::= zero | non-zero digits ;"
  "digits   ::= digit digits | '' ;"
  "digit    ::= non-zero | zero ;"

  // Terminals
  "minus     ::= '-' ;"
  "plus      ::= '+' ;"
  "times     ::= '*' ;"
  "divide    ::= '/' ;"
  "left-par  ::= '(' ;"
  "right-par ::= ')' ;"
  "zero      ::= '0' ;"
  "non-zero  ::= '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;";

int main() {
  using digit_grammar = bnfpg::grammar<digit>;
  assert(bnfpg::valid<digit_grammar>("0"));
  assert(bnfpg::valid<digit_grammar>("1"));
  assert(bnfpg::valid<digit_grammar>("2"));
  assert(bnfpg::valid<digit_grammar>("3"));
  assert(bnfpg::valid<digit_grammar>("4"));
  assert(bnfpg::valid<digit_grammar>("5"));
  assert(bnfpg::valid<digit_grammar>("6"));
  assert(bnfpg::valid<digit_grammar>("7"));
  assert(bnfpg::valid<digit_grammar>("8"));
  assert(bnfpg::valid<digit_grammar>("9"));
  
  using arithmetic_grammar = bnfpg::grammar<arithmetic>;
  assert(bnfpg::valid<arithmetic_grammar>("1 + 1"));
  
  using compact_arithmetic_grammar = bnfpg::grammar<compact_arithmetic>;
  assert(bnfpg::valid<compact_arithmetic_grammar>("1 + 1"));
}
