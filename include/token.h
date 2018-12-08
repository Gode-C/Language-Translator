/*******************************************************************************
 * Name            : token.h
 * Project         : f3081
 * Module          : token
 * Description     : Header file for token class
 * Copyright       : group faraday. All rights reserved.
 * Original Author : Alish, Akhmetzyanov, Cyrus Chen
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_TOKEN_H_
#define PROJECT_INCLUDE_TOKEN_H_

#include <string>



/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {


/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/*!
  All the Token type needed for this language.
*/
enum kTokenEnumType {
    kIntKwd,
    kFloatKwd,
    kBoolKwd,
    kTrueKwd,
    kFalseKwd,
    kStringKwd,
    kMatrixKwd,
    kLetKwd,
    kInKwd,
    kEndKwd,
    kIfKwd,
    kThenKwd,
    kElseKwd,
    kRepeatKwd,
    kWhileKwd,
    kPrintKwd,
    kToKwd,
    // Constants
    kIntConst,
    kFloatConst,
    kStringConst,
    // Names
    kVariableName,
    // Punctuation
    kLeftParen,
    kRightParen,
    kLeftCurly,
    kRightCurly,
    kLeftSquare,
    kRightSquare,
    kSemiColon,
    kColon,
    // Operators
    kAssign,
    kPlusSign,
    kStar,
    kDash,
    kForwardSlash,
    kLessThan,
    kLessThanEqual,
    kGreaterThan,
    kGreaterThanEqual,
    kEqualsEquals,
    kNotEquals,
    kAndOp,
    kOrOp,
    kNotOp,
    // Special terminal types
    kEndOfFile,
    kLexicalError
};

typedef enum kTokenEnumType TokenType;


/*!
  Token class, built as an linked list node. Including 2 parts, terminal and lexme
*/
class Token {
 private:
  TokenType terminal_;
  std::string lexeme_;
  Token* next_;

 public:
  Token(std::string lexeme, TokenType terminal, Token* next);
  Token();

  void SetTerminal(TokenType terminal_set);
  void SetLexeme(std::string lexeme_set);
  void SetNext(Token* next_set);

  TokenType terminal();
  std::string lexeme();
  Token* next();
};


}  // namespace scanner
}  // namespace fcal

#endif  // PROJECT_INCLUDE_TOKEN_H_
