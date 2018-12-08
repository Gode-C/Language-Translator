/*******************************************************************************
 * Name            : token.cc
 * Project         : 3081
 * Description     : token class
 * Copyright       : group faraday All rights reserved.
 * Original Author : Alish Akhmetzyanov, Cyrus Chen
 ******************************************************************************/

#include "../include/token.h"
#include <stdio.h>

namespace fcal {
namespace scanner {

  Token::Token() {}

  Token::Token(std::string lexeme, TokenType terminal, Token* next)
      :lexeme_(lexeme), terminal_(terminal), next_(next) {}

  void Token::SetTerminal(TokenType terminal_set) {
    terminal_ = terminal_set;
  }

  void Token::SetLexeme(std::string lexeme_set) {
    lexeme_ = lexeme_set;
  }

  void Token::SetNext(Token* next_set) {
    next_ = next_set;
  }

  TokenType Token::terminal() {
    return terminal_;
  }

  std::string Token::lexeme() {
    return lexeme_;
  }

  Token* Token::next() {
    return next_;
  }

}  // namespace scanner
}  // namespace fcal
