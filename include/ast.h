/*******************************************************************************
 * Name            : ast.h
 * Project         : fcal
 * Module          : ast
 * Description     : Header file for AST
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell, Alish Akhmetzyanov, Cyrus Chen
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_AST_H_
#define PROJECT_INCLUDE_AST_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <string>
#include "include/token.h"
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
using namespace fcal::scanner;
namespace fcal {
namespace ast {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Node() {}
};

/** class Stmts
 * brief abstract class for Stmts in AST
 */
class Stmts: public Node {};

/** class Stmt
 * brief abstract class for Stmt in AST
 */
class Stmt: public Node {};

/** class Program
 * brief abstract class for Program in AST
 */
class Program: public Node {
 public:
  Program(std::string var_name, Stmts* stmts);
  std::string unparse();
  std::string CppCode();
 private:
  std::string var_name_;
  Stmts* stmts_;
};

/** class Expr
 * brief abstract class for Expr in AST
 */
class Expr: public Node {};

/** class Decl
 * brief abstract class for Decl in AST
 */
class Decl: public Node {
 public:
  explicit Decl(Token* id);
 protected:
  Token* id_;
};

/** class Decls
 * brief abstract class for Decls in AST
 */
class Decls: public Decl {
 public:
  explicit Decls(Token* id);
  std::string unparse();
  std::string CppCode();
};

/** class EmptyStmts
 * brief abstract class for EmptyStmts in AST
 */
class EmptyStmts: public Stmts {
 public:
  EmptyStmts();
  std::string unparse();
  std::string CppCode();
};

/** class StmtStmts
 * brief abstract class for StmtStmts in AST
 */
class StmtStmts: public Stmts {
 public:
  StmtStmts(Stmt* stmt_left, Stmts* stmts_right);
  std::string unparse();
  std::string CppCode();
 private:
  Stmt* stmt;
  Stmts* stmts;
};

/** class AssignStmt
 * brief abstract class for AssignStmt in AST
 */
class AssignStmt: public Stmt {
 public:
  AssignStmt(Token* id, Expr* expr);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
  Expr* expr_;
};

/** class BodyStmt
 * brief abstract class for BodyStmt in AST
 */
class BodyStmt: public Stmt {
 public:
  explicit BodyStmt(Stmts* stmts);
  std::string unparse();
  std::string CppCode();
 private:
  Stmts* stmts_;
};

/** class IfStmt
 * brief abstract class for BlockStmt in AST
 */
class IfStmt: public Stmt {
 public:
  IfStmt(Expr* predicate, Stmt* then_stmt);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* predicate_;
  Stmt* then_stmt_;
};

/** class IfStmt
 * brief abstract class for BlockStmt in AST
 */
class IfElseStmt: public Stmt {
 public:
  IfElseStmt(Expr* predicate, Stmt* then_stmt, Stmt* else_stmt);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* predicate_;
  Stmt* then_stmt_;
  Stmt* else_stmt_;
};

/** class DeclStmt
 * brief abstract class for DeclStmt in AST
 */
class DeclStmt: public Stmt {
 public:
  explicit DeclStmt(Decl* decl);
  std::string unparse();
  std::string CppCode();
 private:
  Decl* decl;
};

/** class PrintStmt
 * brief abstract class for PrintStmt in AST
 */
class PrintStmt: public Stmt {
 public:
  explicit PrintStmt(Expr* expr);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* expr_;
};

/** class RepeatStmt
 * brief abstract class for RepeatStmt in AST
 */
class RepeatStmt: public Stmt {
 public:
  RepeatStmt(Token* id, Expr* begin, Expr* end, Stmt* stmt);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
  Expr* begin_;
  Expr* end_;
  Stmt* stmt_;
};

/** class WhileStmt
 * brief abstract class for RepeatStmt in AST
 */
class WhileStmt: public Stmt {
 public:
  WhileStmt(Expr* expr, Stmt* body);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* predicate_;
  Stmt* body_;
};

/** class SemiColonStmt
 * brief abstract class for SemiColonStmt in AST
 */
class SemiColonStmt: public Stmt {
 public:
  SemiColonStmt();
  std::string unparse();
  std::string CppCode();
};

/** class VarExpr
 * brief abstract class for Simple Expressions in AST
 */
class VarExpr: public Expr {
 public:
  explicit VarExpr(Token* id);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
};

/** class IntConstExpr
 * brief abstract class for IntConst Expressions in AST
 */
class IntConstExpr: public Expr {
 public:
  explicit IntConstExpr(Token* id);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
};

/** class FloatConstExpr
 * brief abstract class for FloatConstExpr in AST
 */
class FloatConstExpr: public Expr {
 public:
  explicit FloatConstExpr(Token* id);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
};

/** class stringConstExpr
 * brief abstract class for SringConstExpr in AST
 */
class StringConstExpr: public Expr {
 public:
  explicit StringConstExpr(Token* id);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
};

/** class TrueExpr
 * brief abstract class for TrueExpr in AST
 */
class TrueExpr: public Expr {
 public:
  TrueExpr();
  std::string unparse();
  std::string CppCode();
};

/** class FalseExpr
 * brief abstract class for FalseExpr in AST
 */
class FalseExpr: public Expr {
 public:
  FalseExpr();
  std::string unparse();
  std::string CppCode();
};

/** class BinaryOperators
 * brief abstract class for BinaryOperators in AST
 */
class BinaryOperators: public Expr {
 public:
  BinaryOperators(Expr* left, Expr* right, TokenType _op);
  std::string unparse();
  std::string CppCode();
 protected:
  TokenType op_;
  Expr* left_;
  Expr* right_;
};

/** class BinaryOperators
 * brief abstract class for BinaryOperators in AST
 */
// class BasicOp: public BinaryOperators {
// public:
//  BasicOp(Expr* left, Expr* right, TokenType op);
//  std::string unparse();
// protected:
//  TokenType op;
//};

/** class funcExpr
 * brief abstract class for BinaryOperators in AST
 */
class FuncExpr: public Expr {
 public:
  FuncExpr(Token* name, Expr* body);
  std::string unparse();
  std::string CppCode();
 private:
  Token* name_;
  Expr* body_;
};

/** class ParenExpr
 * brief abstract class for ParenExpr in AST
 */
class ParenExpr: public Expr {
 public:
  explicit ParenExpr(Expr* expr);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* expr_;
};

/** class LetExpr
 * brief abstract class for LetExpr in AST
 */
class LetExpr: public Expr {
 public:
  LetExpr(Stmts* stmts, Expr* expr);
  std::string unparse();
  std::string CppCode();
 private:
  Stmts* stmts_;
  Expr* expr_;
};

/** class IfElseExpr
 * brief abstract class for IfElseExpr in AST
 */
class IfElseExpr: public Expr {
 public:
  IfElseExpr(Expr* predicate, Expr* then_stmt, Expr* else_stmt);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* predicate_;
  Expr* then_stmt_;
  Expr* else_stmt_;
};

/** class NotExpr
 * brief abstract class for LetExpr in AST
 */
class NotExpr: public Expr {
 public:
  explicit NotExpr(Expr* not_expr);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* not_expr_;
};

// matrix related classes
class MatrixDecl: public Decl {
 public:
  MatrixDecl(Token* matrix_name, Expr* matrix_expr);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* matrix_expr_;
};

/** class MatrixBodyDecl
 * brief abstract class for MatrixBodyDecl in AST
 */
class MatrixBodyDecl: public Decl {
 public:
  MatrixBodyDecl(Token* name, Expr* column, Expr* row, Expr* row_iter,
    Expr* col_iter, Expr* expr);
  std::string unparse();
  std::string CppCode();
 private:
  Expr* row_;
  Expr* column_;
  Expr* row_iter_;
  Expr* col_iter_;
  Expr* expr_;
};

/** class MatrixStmt
 * brief abstract class for MatrixStmt in AST
 */
class MatrixStmt: public Stmt {
 public:
  MatrixStmt(Token* name, Expr* column, Expr* row, Expr* expr);
  std::string unparse();
  std::string CppCode();
 private:
  Token* name_;
  Expr* row_;
  Expr* column_;
  Expr* expr_;
};

/** class MatrixStmt
 * brief abstract class for MatrixStmt in AST
 */
class MatrixRefExpr: public Expr {
 public:
  MatrixRefExpr(Token* name, Expr* row,  Expr* column);
  std::string unparse();
  std::string CppCode();
 private:
  Token* id_;
  Expr* row_;
  Expr* column_;
};

} /* namespace ast */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_AST_H_
