#include "bnfpg.hpp"

constexpr static const char maths_grammar[] =

  // non terminals
  "expression = add expression ;"
  "add expression = subtract expression , [ plus , add expression ] ;"
  "subtract expression = multiply expression , [ minus , subtract expression ] ;"
  "multiply expression = divide expression , [ multiply , multiply expression ] ;"
  "divide expression = minus expression , [ divide , divide expression ] ;"
  "minus expression = [ minus ] , parenthesis expression ;"
  "parenthesis expression = left parenthesis , expression , right parenthesis"
  "                       | number ;"
  "number = zero | non zero , { digit } ;"

  // Terminals
  "plus = \"+\" ;"
  "minus = \"-\" ;"
  "multiply = \"*\" ;"
  "divide = \"/\""
  "left parenthesis = \"(\" ;"
  "right parenthesis = \")\" ;"
  "zero = \"0\" ;"
  "non zero = \"1\" | \"2\" | \"3\" | \"4\" | \"5\" | \"6\" | \"7\" | \"8\" | \"9\" ;"
  "digit = \"0\" | \"1\" | \"2\" | \"3\" | \"4\" | \"5\" | \"6\" | \"7\" | \"8\" | \"9\" ;";


int main() {
  using maths_parser = bnfpg::parser<maths_grammar>;
}
