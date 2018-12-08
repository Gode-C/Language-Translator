/*******************************************************************************
 * Name            : parser.h
 * Project         : fcal
 * Module          : parser
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Erik Van Wyk
 * Modifications by: Dan Challou, John Harwell,Alish Akhmetzyanov, Cyrus Chen
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <assert.h>
#include <stdio.h>
#include "../include/ext_token.h"
#include "../include/parse_result.h"
#include "../include/parser.h"
#include "../include/scanner.h"
#include "../include/ast.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace ast = fcal::ast;
namespace fcal {
namespace parser {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
/*!
 * @brief Parser::~Parser()
 * @param none
 * @return none
 */
Parser::~Parser() {
  if (scanner_) delete scanner_;

  scanner::ExtToken *to_delete1;
  curr_token_ = tokens_;
  while (curr_token_) {
    to_delete1 = curr_token_;
    curr_token_ = curr_token_->next();
    delete to_delete1;
  } /* while() */

  scanner::Token *curr_scanner_token = stokens_;
  scanner::Token *to_delete2;
  while (curr_scanner_token) {
    to_delete2 = curr_scanner_token;
    curr_scanner_token = curr_scanner_token->next();
    delete to_delete2;
  } /* while() */
} /* Parser::~Parser() */

/*!
 * @brief Parser::Parse
 * @param const char *text
 * @return ParseResult pr
 */
ParseResult Parser::Parse(const char *text) {
  assert(text != NULL);

  ParseResult pr;
  try {
          scanner_ = new scanner::Scanner();
          stokens_ = scanner_->Scan(text);
          tokens_ = tokens_->ExtendTokenList(this, stokens_);

          assert(tokens_ != NULL);
          curr_token_ = tokens_;
          pr = ParseProgram();
  }
  catch (std::string errMsg) {
          pr.ok(false);
          pr.errors(errMsg);
          pr.ast(NULL);
  }
  return pr;
} /* Parser::parse() */

/*
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

/*!
 * @brief Parser::ParseProgram, for Program in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::ParseProgram() {
  ParseResult pr;
  // root
  // Program ::= varName '(' ')' '{' Stmts '}'
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  match(scanner::kLeftParen);
  match(scanner::kRightParen);
  match(scanner::kLeftCurly);

  ParseResult pr_stmts = parse_stmts();
  /// add Program to our ast
  pr.ast(new ast::Program(name, dynamic_cast<ast::Stmts*>(pr_stmts.ast())));

  match(scanner::kRightCurly);
  match(scanner::kEndOfFile);
  return pr;
} /* Parser::ParseProgram() */

// MatrixDecl
// identical purpose of parse_decl, handles special matrix syntax.
ParseResult Parser::parse_matrix_decl() {
  ParseResult pr;
  ParseResult preparse;
  match(scanner::kMatrixKwd);
  match(scanner::kVariableName);
  Token* matrix_id = new Token(prev_token_->lexeme(),
    prev_token_->terminal(), NULL);

  // Decl ::= 'matrix' varName '[' Expr ':' Expr ']' varName ':' varName  '='
  // Expr ';'
  if (attempt_match(scanner::kLeftSquare)) {
    preparse = parse_expr(0);
    ast::Expr* row = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kColon);
    preparse = parse_expr(0);
    ast::Expr* column = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kRightSquare);
    preparse = parse_variable_name();
    ast::Expr* row_iterator = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kColon);
    preparse = parse_variable_name();
    ast::Expr* column_iterator = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kAssign);
    preparse = parse_expr(0);
    ast::Expr* matrix_expr = dynamic_cast<ast::Expr*>(preparse.ast());
    pr.ast(new ast::MatrixBodyDecl(matrix_id, row, column, row_iterator,
        column_iterator, matrix_expr));
  } else if (attempt_match(scanner::kAssign)) {
    // Decl ::= 'matrix' varName '=' Expr ';'
    preparse = parse_expr(0);
    ast::Expr* matrix_expr = dynamic_cast<ast::Expr*>(preparse.ast());
    pr.ast(new ast::MatrixDecl(matrix_id, matrix_expr));
  } else {
    throw((std::string) "Bad Syntax of Matrix Decl in in parseMatrixDecl");
  }

  match(scanner::kSemiColon);

  return pr;
}
// standardDecl
// Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
/*!
 * @brief Parser::parse_standard_decl, for standard decl in ast.
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_standard_decl() {
  ParseResult pr;

  // attempt to match standard decl
  TokenType terminal = kLexicalError;

  if (attempt_match(scanner::kIntKwd)) {  // Type ::= intKwd
    terminal = kIntKwd;
  } else if (attempt_match(scanner::kFloatKwd)) {  // Type ::= floatKwd
    terminal = kFloatKwd;
  } else if (attempt_match(scanner::kStringKwd)) {  // Type ::= stringKwd
    terminal = kStringKwd;
  } else if (attempt_match(scanner::kBoolKwd)) {  // Type ::= boolKwd
    terminal = kBoolKwd;
  }
  match(scanner::kVariableName);

  // construct token to be passed as a parameter
  if (terminal == kLexicalError) {
    pr.ast(NULL);
  } else {
    Token* name = new Token(prev_token_->lexeme(),
    terminal, NULL);
    pr.ast(new ast::Decls(name));
  }

  match(scanner::kSemiColon);
  return pr;
}

/*!
 * @brief Parser::parse_decl, for Decl in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_decl() {
  ParseResult pr;
  // Decl :: matrix variableName ....
  if (next_is(scanner::kMatrixKwd)) {
    pr = parse_matrix_decl();
  } else {
    // Decl ::= Type variableName semiColon
    pr = parse_standard_decl();
  }
  return pr;
}

/*!
 * @brief Parser::parse_stmts for Stmts in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_stmts() {
  ParseResult pr;
  if (!next_is(scanner::kRightCurly) && !next_is(scanner::kInKwd)) {
    // Stmts ::= Stmt Stmts
    ParseResult pr_stmt = parse_stmt();
    ParseResult pr_stmts = parse_stmts();

    // add new StmtStmts to our ast tree
    pr.ast(new ast::StmtStmts(dynamic_cast<ast::Stmt*>(pr_stmt.ast()),
    dynamic_cast<ast::Stmts*>(pr_stmts.ast())));
  } else {
    // Stmts ::=
    // nothing to match.k
    pr.ast(new ast::EmptyStmts());
  }
  return pr;
}


/*!
 * @brief Parser::parse_stmt for Stmt in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_stmt() {
  ParseResult pr;
  ParseResult preparse;
  // Stmt ::= Decl
  if (next_is(scanner::kIntKwd) || next_is(scanner::kFloatKwd) ||
      next_is(scanner::kMatrixKwd) || next_is(scanner::kStringKwd) ||
      next_is(scanner::kBoolKwd)) {
    pr = parse_decl();

    // add DeclStmt to our ast
    pr.ast(new ast::DeclStmt(dynamic_cast<ast::Decl*>(pr.ast())));

  } else if (attempt_match(scanner::kLeftCurly)) {
    // Stmt ::= '{' Stmts '}'
    // pre-parse our ast first and then create a branch
    preparse = parse_stmts();
    pr.ast(new ast::BodyStmt(dynamic_cast<ast::Stmts*>(preparse.ast())));

    match(scanner::kRightCurly);
  } else if (attempt_match(scanner::kIfKwd)) {
    // Stmt ::= 'if' '(' Expr ')' Stmt
    // Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    match(scanner::kLeftParen);

    preparse = parse_expr(0);
    ast::Expr* cond = dynamic_cast<ast::Expr*>(preparse.ast());

    match(scanner::kRightParen);

    preparse = parse_stmt();
    ast::Stmt* then = dynamic_cast<ast::Stmt*>(preparse.ast());

    if (attempt_match(scanner::kElseKwd)) {
      preparse = parse_stmt();
      ast::Stmt* elseStmt = dynamic_cast<ast::Stmt*>(preparse.ast());
      pr.ast(new ast::IfElseStmt(cond, then, elseStmt));
    } else {     // otherwise it is just If statement
      pr.ast(new ast::IfStmt(cond, then));
    }
  } else if (attempt_match(scanner::kVariableName)) {
    /*
     * Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']'
     * '=' Expr ';'
     */
    Token* assignment_token = new Token(prev_token_->lexeme(),
                                        prev_token_->terminal(), NULL);

    if (attempt_match(scanner::kLeftSquare)) {
      preparse = parse_expr(0);
      ast::Expr* row = dynamic_cast<ast::Expr*>(preparse.ast());
      match(scanner::kColon);
      preparse = parse_expr(0);
      ast::Expr* column = dynamic_cast<ast::Expr*>(preparse.ast());
      match(scanner::kRightSquare);
      match(kAssign);
      preparse = parse_expr(0);
      ast::Expr* assign_expr = dynamic_cast<ast::Expr*>(preparse.ast());
      pr.ast(new ast::MatrixStmt(assignment_token, row, column, assign_expr));
    } else {
      match(scanner::kAssign);

      preparse = parse_expr(0);
      ast::Expr* assignment = dynamic_cast<ast::Expr*>(preparse.ast());

      pr.ast(new ast::AssignStmt(assignment_token, assignment));

      match(scanner::kSemiColon);
    }

  } else if (attempt_match(scanner::kPrintKwd)) {
    // Stmt ::= 'print' '(' Expr ')' ';'
    match(scanner::kLeftParen);
    preparse = parse_expr(0);
    ast::Expr* printStmt = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kRightParen);
    match(scanner::kSemiColon);
    pr.ast(new ast::PrintStmt(printStmt));
  } else if (attempt_match(scanner::kRepeatKwd)) {
    // Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    match(scanner::kLeftParen);
    match(scanner::kVariableName);
    Token* repeat = new Token(prev_token_->lexeme(),
                              prev_token_->terminal(), NULL);

    match(scanner::kAssign);

    // start parsing repeat body
    preparse = parse_expr(0);
    ast::Expr* begin = dynamic_cast<ast::Expr*>(preparse.ast());

    match(scanner::kToKwd);

    preparse = parse_expr(0);
    ast::Expr* end = dynamic_cast<ast::Expr*>(preparse.ast());

    match(scanner::kRightParen);
    preparse = parse_stmt();
    ast::Stmt* repeat_stmt = dynamic_cast<ast::Stmt*>(preparse.ast());


    pr.ast(new ast::RepeatStmt(repeat, begin, end, repeat_stmt));

  } else if (attempt_match(scanner::kWhileKwd)) {
    // Stmt ::= 'while' '(' Expr ')' Stmt
    match(scanner::kLeftParen);

    preparse = parse_expr(0);
    ast::Expr* predicate = dynamic_cast<ast::Expr*>(preparse.ast());

    match(scanner::kRightParen);

    preparse = parse_stmt();
    ast::Stmt* body = dynamic_cast<ast::Stmt*>(preparse.ast());

    pr.ast(new ast::WhileStmt(predicate, body));
  } else if (attempt_match(scanner::kSemiColon)) {
    // Stmt ::= ';
    // parsed a skip
    pr.ast(new ast::SemiColonStmt());
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a statement");
  }
  // Stmt ::= variableName assign Expr semiColon
  return pr;
}


/*!
 * @brief Parser::parse_expr for Expr in ast
 * @param int rbp
 * @return ParseResult pr
 */
ParseResult Parser::parse_expr(int rbp) {
  /* Examine current token, without consuming it, to call its
     associated parse methods.  The ExtToken objects have 'nud' and
     'led' methods that are dispatchers that call the appropriate
     parse methods.*/
  ParseResult left = curr_token_->nud();

  while (rbp < curr_token_->lbp()) {
    left = curr_token_->led(left);
  }

  return left;
}

/*
 * parse methods for Expr productions
 * ----------------------------------
 */

// Expr ::= trueKwd
/*!
 * @brief Parser::parse_true_kwd for true_kwd in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_true_kwd() {
  ParseResult pr;
  match(scanner::kTrueKwd);
  pr.ast(new ast::TrueExpr());
  return pr;
}

// Expr ::= falseKwd
/*!
 * @brief Parser::parse_false_kwd for false_kwd in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_false_kwd() {
  ParseResult pr;
  match(scanner::kFalseKwd);
  pr.ast(new ast::FalseExpr());
  return pr;
}

// Expr ::= intConst
/*!
 * @brief Parser::parse_int_const for int_const in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_int_const() {
  ParseResult pr;
  match(scanner::kIntConst);
  Token* parameter = new Token(prev_token_->lexeme(),
          prev_token_->terminal(), NULL);
  pr.ast(new ast::IntConstExpr(parameter));
  return pr;
}

// Expr ::= floatConst
/*!
 * @brief Parser::parse_float_const for float_const in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_float_const() {
  ParseResult pr;
  match(scanner::kFloatConst);
  Token* parameter = new Token(prev_token_->lexeme(),
          prev_token_->terminal(), NULL);
  pr.ast(new ast::FloatConstExpr(parameter));
  return pr;
}

// Expr ::= stringConst
/*!
 * @brief Parser::parse_string_const for string_const in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_string_const() {
  ParseResult pr;
  match(scanner::kStringConst);
  Token* parameter = new Token(prev_token_->lexeme(),
          prev_token_->terminal(), NULL);
  pr.ast(new ast::StringConstExpr(parameter));
  return pr;
}

// Expr ::= variableName .....
/*!
 * @brief Parser::parse_variable_name for variable_name in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_variable_name() {
  ParseResult pr;
  ParseResult preparse;
  match(scanner::kVariableName);

  Token* id = new Token(prev_token_->lexeme(),
                          prev_token_->terminal(), NULL);

  if (attempt_match(scanner::kLeftSquare)) {
    // Case it's a matrix
    preparse = parse_expr(0);
    ast::Expr* row = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kColon);
    preparse = parse_expr(0);
    ast::Expr* column = dynamic_cast<ast::Expr*>(preparse.ast());
    match(scanner::kRightSquare);
    pr.ast(new ast::MatrixRefExpr(id, row, column));
  }  else if (attempt_match(scanner::kLeftParen)) {
    // Expr ::= varableName '(' Expr ')'
    preparse = parse_expr(0);
    ast::Expr* function_name = dynamic_cast<ast::Expr*>(preparse.ast());

    match(scanner::kRightParen);

    pr.ast(new ast::FuncExpr(id, function_name));
  } else {
    // variable
    pr.ast(new ast::VarExpr(id));
  }
  return pr;
}

// Expr ::= leftParen Expr rightParen
/*!
 * @brief Parser::parse_nested_expr for nested_expressions
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_nested_expr() {
  ParseResult pr;
  ParseResult preparse;

  match(scanner::kLeftParen);

  preparse = parse_expr(0);
  ast::Expr* body = dynamic_cast<ast::Expr*>(preparse.ast());

  match(scanner::kRightParen);
  pr.ast(new ast::ParenExpr(body));
  return pr;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr
/*!
 * @brief Parser::parse_if_expr for if_expr in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_if_expr() {
  ParseResult pr;
  ParseResult preparse;

  match(scanner::kIfKwd);

  preparse = parse_expr(0);
  ast::Expr* predicate = dynamic_cast<ast::Expr*>(preparse.ast());

  match(scanner::kThenKwd);

  preparse = parse_expr(0);
  ast::Expr* then = dynamic_cast<ast::Expr*>(preparse.ast());

  match(scanner::kElseKwd);

  preparse = parse_expr(0);
  ast::Expr* else_kwd = dynamic_cast<ast::Expr*>(preparse.ast());

  pr.ast(new ast::IfElseExpr(predicate, then, else_kwd));
  return pr;
}

// Expr ::= 'let' Stmts 'in' Expr 'end'
/*!
 * @brief Parser::parse_let_expr for let_expr in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_let_expr() {
  ParseResult pr;
  ParseResult preparse;

  match(scanner::kLetKwd);

  preparse = parse_stmts();
  ast::Stmts* let_stmts = dynamic_cast<ast::Stmts*>(preparse.ast());

  match(scanner::kInKwd);

  preparse = parse_expr(0);
  ast::Expr* then_stmt = dynamic_cast<ast::Expr*>(preparse.ast());

  match(scanner::kEndKwd);

  pr.ast(new ast::LetExpr(let_stmts, then_stmt));
  return pr;
}

// Expr ::= '!' Expr
/*!
 * @brief Parser::parse_not_expr for not_expr in ast
 * @param none
 * @return ParseResult pr
 */
ParseResult Parser::parse_not_expr() {
  ParseResult pr;
  ParseResult preparse;

  match(scanner::kNotOp);

  preparse = parse_expr(0);
  ast::Expr* not_expr = dynamic_cast<ast::Expr*>(preparse.ast());

  pr.ast(new ast::NotExpr(not_expr));
  return pr;
}
// Expr ::= Expr plusSign Expr
/*!
 * @brief Parser::parse_addition for addition in ast
 * @param ParseResult prLeft
 * @return ParseResult pr
 */
ParseResult Parser::parse_addition(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ParseResult preparse_left;
  ParseResult preparse_right;

  match(scanner::kPlusSign);

  preparse_left = parse_expr(prev_token_->lbp());
  ast::Expr* left_operand = dynamic_cast<ast::Expr*>(prLeft.ast());
  ast::Expr* right_operand = dynamic_cast<ast::Expr*>(preparse_left.ast());

  pr.ast(new ast::BinaryOperators(left_operand, right_operand, kPlusSign));
  return pr;
}

// Expr ::= Expr star Expr
/*!
 * @brief Parser::parse_multiplication for multiplication in ast
 * @param ParseResult prLeft
 * @return ParseResult pr
 */
ParseResult Parser::parse_multiplication(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ParseResult preparse_left;
  ParseResult preparse_right;

  match(scanner::kStar);

  preparse_left = parse_expr(prev_token_->lbp());
  ast::Expr* left_operand = dynamic_cast<ast::Expr*>(prLeft.ast());
  ast::Expr* right_operand = dynamic_cast<ast::Expr*>(preparse_left.ast());

  pr.ast(new ast::BinaryOperators(left_operand, right_operand, kStar));
  return pr;
}

// Expr ::= Expr dash Expr
/*!
 * @brief Parser::parse_subtraction for subtraction in ast
 * @param ParseResult prLeft
 * @return ParseResult pr
 */
ParseResult Parser::parse_subtraction(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ParseResult preparse_left;
  ParseResult preparse_right;

  match(scanner::kDash);

  preparse_left = parse_expr(prev_token_->lbp());
  ast::Expr* left_operand = dynamic_cast<ast::Expr*>(prLeft.ast());
  ast::Expr* right_operand = dynamic_cast<ast::Expr*>(preparse_left.ast());

  pr.ast(new ast::BinaryOperators(left_operand, right_operand, kDash));
  return pr;
}

// Expr ::= Expr forwardSlash Expr
/*!
 * @brief Parser::parse_division for division in ast
 * @param ParseResult prLeft
 * @return ParseResult pr
 */
ParseResult Parser::parse_division(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ParseResult preparse_left;
  ParseResult preparse_right;

  match(scanner::kForwardSlash);

  preparse_left = parse_expr(prev_token_->lbp());
  ast::Expr* left_operand = dynamic_cast<ast::Expr*>(prLeft.ast());
  ast::Expr* right_operand = dynamic_cast<ast::Expr*>(preparse_left.ast());

  pr.ast(new ast::BinaryOperators(left_operand, right_operand, kForwardSlash));
  return pr;
}

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
*/
/*!
 * @brief Parser::parse_relational_expr for relational_expr in ast
 * @param ParseResult prLeft
 * @return ParseResult pr
 */
ParseResult Parser::parse_relational_expr(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ParseResult preparse_right;

  next_token();
  // just advance token, since examining it in parse_expr caused
  // this method being called.

  preparse_right = parse_expr(prev_token_->lbp());

  ast::Expr* left_expr = dynamic_cast<ast::Expr*>(prLeft.ast());
  ast::Expr* right_expr = dynamic_cast<ast::Expr*>(preparse_right.ast());

  if (prev_token_->terminal() == kLexicalError) {
    pr.ast(NULL);
  } else {
    pr.ast(new ast::BinaryOperators(left_expr,
        right_expr, prev_token_->terminal()));
  }


  return pr;
}

// Helper function used by the parser.
/*!
 * @brief Parser::match for matching expressions in ast
 * @param TokenType &tt
 * @return none
 */
void Parser::match(const scanner::TokenType &tt) {
  if (!attempt_match(tt)) {
    throw(make_error_msg_expected(tt));
  }
}
/*!
 * @brief Parser::attempt_match for matching expressions in ast
 * @param TokenType &tt
 * @return bool
 */
bool Parser::attempt_match(const scanner::TokenType &tt) {
  if (curr_token_->terminal() == tt) {
    next_token();
    return true;
  }
  return false;
}
/*!
 * @brief Parser::next_is for returning next pointer in ast
 * @param TokenType &tt
 * @return bool
 */
bool Parser::next_is(const scanner::TokenType &tt) {
  return curr_token_->terminal() == tt;
}
/*!
 * @brief Parser::next_token for returning next token in ast
 * @param none
 * @return none
 */
void Parser::next_token() {
  if (curr_token_ == NULL) {
    throw(std::string(
        "Internal Error: should not call nextToken in unitialized state"));
  } else if (curr_token_->terminal() == scanner::kEndOfFile &&
           curr_token_->next() == NULL) {
    prev_token_ = curr_token_;
  } else if (curr_token_->terminal() != scanner::kEndOfFile &&
             curr_token_->next() == NULL) {
    throw(make_error_msg("Error: tokens end with endOfFile"));
  } else {
    prev_token_ = curr_token_;
    curr_token_ = curr_token_->next();
  }
}

/*!
 * @brief Parser::terminal_description for terminal description in ast
 * @param TokenType &terminal
 * @return String s
 */
std::string Parser::terminal_description(const scanner::TokenType &terminal) {
  scanner::Token *dummy_token = new scanner::Token("", terminal, NULL);
  scanner::ExtToken *dummy_ext_token = new scanner::ExtToken(this, dummy_token);
  std::string s = dummy_ext_token->ExtendToken
          (this, dummy_token)->description();
  delete dummy_token;
  delete dummy_ext_token;
  return s;
}

/*!
 * @brief Parser::make_error_msg_expected, returns error message
 * @param  const TokenType &terminal
 * @return string S
 */
std::string Parser::make_error_msg_expected(
    const scanner::TokenType &terminal) {
  std::string s = (std::string) "Expected " + terminal_description(terminal) +
                  " but found " + curr_token_->description();
  return s;
}


/*!
 * @brief Parser::make_error_msg, returns error message
 * @param  const TokenType &terminal
 * @return string S
 */
std::string Parser::make_error_msg(const scanner::TokenType &terminal) {
  std::string s = "Unexpected symbol " + terminal_description(terminal);
  return s;
}

/*!
 * @brief Parser::make_error_msg, returns error message
 * @param  const char *msg
 * @return string S
 */
std::string Parser::make_error_msg(const char *msg) { return msg; }

} /* namespace parser */
} /* namespace fcal */
