/*******************************************************************************
 * Name            : ast.cc
 * Project         : 3081
 * Module          : ast
 * Description     : scan function
 * Copyright       : group faraday All rights reserved.
 * Original Author : Alish Akhmetzyanov, Cyrus Chen
 ******************************************************************************/

#include <iostream>
#include <string>
#include "../include/ast.h"
#include "../include/regex.h"
#include "../include/read_input.h"

namespace fcal {
namespace ast {

/*******************************************************************************
 * Abstract classes
 ******************************************************************************/
/*!
 * @brief main Program
 */
Program::Program(std::string var_name, Stmts* stmts) :
    var_name_(var_name), stmts_(stmts) {}

/*!
 * @brief unparsing parogram
 * @param string var_name, Stmts* stmts
 * @return string (name + unparsed )
 */
std::string Program::unparse() {
  return var_name_ + "()" + "{" + stmts_->unparse() +  "}";
}

std::string Program::CppCode() {
  std::string includes =
    "#include <iostream>\n#include <math.h>\n#include \"include/Matrix.h\"\n";
  std::string namespaces = "using namespace std;\n";
  std::string f_name = "int " + var_name_ + "() {\n" +
    stmts_->CppCode() + "}\n";
  return includes + namespaces + f_name;
}

/*!
 * @brief Declaration
 */
Decl::Decl(Token* id) :
    id_(id) {}

/*!
 * @brief Polymorphic class for various declarations
 */
Decls::Decls(Token* id): Decl(id) {}

/*!
 * @brief unparsing declarations
 * @param none
 * @return string (name + unparsed)
 */
std::string Decls::unparse() {
  switch (id_->terminal()) {
    case kIntKwd: return "int " + id_->lexeme() + " ;";
    case kFloatKwd: return "float " + id_->lexeme() + " ;";
    case kStringKwd: return "string " + id_->lexeme() + " ;";
    case kBoolKwd: return "boolean " + id_->lexeme() + " ;";
    default: return "Error";
  }
}

std::string Decls::CppCode() {
  switch (id_->terminal()) {
    case kIntKwd: return "int " + id_->lexeme() + ";\n";
    case kFloatKwd: return "float " + id_->lexeme() + ";\n";
    case kStringKwd: return "string " + id_->lexeme() + ";\n";
    case kBoolKwd: return "bool " + id_->lexeme() + ";\n";
    default: return "Error";
  }
}

/*******************************************************************************
 * Matrices
 ******************************************************************************/


MatrixBodyDecl::MatrixBodyDecl(Token* name, Expr* row, Expr* column,
    Expr* row_iter, Expr* col_iter, Expr* expr) :
    Decl(name), row_(row), column_(column), row_iter_(row_iter),
        col_iter_(col_iter), expr_(expr) {}

std::string MatrixBodyDecl::unparse() {
  return "matrix " + id_->lexeme() + "[" + row_->unparse() + ": "
      + column_->unparse() + "]" + row_iter_->unparse() + ":" +
      col_iter_->unparse() + " = " + expr_->unparse() + " ;";
}

std::string MatrixBodyDecl::CppCode() {
  std::string rt =
    "matrix " + id_->lexeme() + "(" + row_->CppCode() + ", " +
       column_->CppCode() + ");\n" +"for (int " + row_iter_->CppCode() +
       " = 0; " + row_iter_->CppCode() + " < " + row_->CppCode() + "; " +
       row_iter_->CppCode() + "++) {\n" + "\t" + "for (int " +
       col_iter_->CppCode() + " = 0; " + col_iter_->CppCode() + " < " +
       column_->CppCode() + "; " + col_iter_->CppCode() + "++) {\n" +"\t\t" +
       "*(" + id_->lexeme() +".access(" + row_iter_->CppCode() +
       "," + col_iter_->CppCode() + ")) = " + expr_->CppCode() + ";" +
       "\n\t}" + "\n}\n";

  return rt;
}


MatrixRefExpr::MatrixRefExpr(Token* id, Expr* row, Expr* column) :
    id_(id), row_(row), column_(column)  {}

std::string MatrixRefExpr::unparse() {
  return id_->lexeme() + " [" + row_->unparse() +
    ":" + column_->unparse() + "]";
}

std::string MatrixRefExpr::CppCode() {
  std::string rt = "*(" + id_->lexeme() +
    ".access(" + row_->CppCode() + "," + column_->CppCode() + "))";

  return rt;
}

// matrixAssignmentStmt
MatrixStmt::MatrixStmt(Token* name, Expr* row, Expr* column, Expr* expr):
    name_(name), row_(row), column_(column), expr_(expr) {}

std::string MatrixStmt::unparse() {
  return name_->lexeme() + " [" + row_->unparse() + ": " +
    column_->unparse() + "] =" + expr_->unparse() + " ; \n";
}

std::string MatrixStmt::CppCode() {
  return "*(" + name_->lexeme() + ".access(" +
    row_->CppCode() + "," + column_->CppCode() + ")) = " + expr_->CppCode();
}

MatrixDecl::MatrixDecl(Token *matrix_name,
    Expr *matrix_expr): Decl(matrix_name), matrix_expr_(matrix_expr) {}

std::string MatrixDecl::unparse() {
  return "matrix " + id_->lexeme() + " = " + matrix_expr_->unparse() + ";\n";
}

std::string MatrixDecl::CppCode() {
    regex_t * readm = fcal::scanner::make_regex("matrix_read");
    int findf =
        fcal::scanner::match_regex(readm, matrix_expr_->CppCode().c_str());

    if (findf == 11) {
        return "matrix " + id_->lexeme() +
            " ( matrix::" + matrix_expr_->CppCode() + " );\n";
    } else {
        return "matrix " + id_->lexeme() +
            " = " + matrix_expr_->CppCode() + " ;\n";
    }
}

/*******************************************************************************
 * Expressions
 ******************************************************************************/
/*!
 * @brief Var Expression Handling
 */
VarExpr::VarExpr(Token* id):
    id_(id) {}

/*!
 * @brief Var Expression Handling
 * @param none
 * @return string
 */
std::string VarExpr::unparse() {
  return id_->lexeme();
}

std::string VarExpr::CppCode() {
  return id_->lexeme();
}

/*!
 * @brief IntConstExpr
 */
IntConstExpr::IntConstExpr(Token* id):
    id_(id) {}

/*!
 * @brief unparse int constand expression
 * @param none
 * @return string
 */
std::string IntConstExpr::unparse() {
  return id_->lexeme();
}

std::string IntConstExpr::CppCode() {
  return id_->lexeme();
}

/*!
 * @brief FloatConstExpr
 */
FloatConstExpr::FloatConstExpr(Token* id):
    id_(id) {}

/*!
 * @brief unparse floating constand expression
 * @param none
 * @return string
 */
std::string FloatConstExpr::unparse() {
  return id_->lexeme();
}

std::string FloatConstExpr::CppCode() {
  return id_->lexeme();
}

/*!
 StringConstExpr
*/
StringConstExpr::StringConstExpr(Token* id):
    id_(id) {}

/*!
 * @brief unparse string constand expression
 * @param none
 * @return string
 */
std::string StringConstExpr::unparse() {
  return id_->lexeme();
}

std::string StringConstExpr::CppCode() {
  return id_->lexeme();
}

/*!
 TrueExpr
*/
TrueExpr::TrueExpr() {}

/*!
 * @brief unparse true constand expression
 * @param none
 * @return string
 */
std::string TrueExpr::unparse() {
  return "true";
}

std::string TrueExpr::CppCode() {
  return "true";
}

/*!
 FalseExpr
*/
FalseExpr::FalseExpr() {}

/*!
 * @brief unparse false constand expression
 * @param none
 * @return string
 */
std::string FalseExpr::unparse() {
  return "false";
}

std::string FalseExpr::CppCode() {
  return "false";
}

/*!
 Function Expr
*/
FuncExpr::FuncExpr(Token* name, Expr* expr) :
    name_(name), body_(expr) {}

/*!
 * @brief unparse function expression
 * @param none
 * @return string
 */
std::string FuncExpr::unparse() {
  return name_->lexeme() + "(" + body_->unparse() + ")";
}

std::string FuncExpr::CppCode() {
//  return body_->CppCode() + "." + name_->lexeme();

    if (name_->lexeme() == "n_rows" || name_->lexeme() == "n_cols") {
        return body_->CppCode() + "." + name_->lexeme() + "()";
    }

  return name_->lexeme() + "(" + body_->CppCode() + ")";
}

/*!
  Parenthesis Expr
*/
ParenExpr::ParenExpr(Expr* expr) :
    expr_(expr) {}

/*!
 * @brief unparse parenthesis expression
 * @param none
 * @return string
 */
std::string ParenExpr::unparse() {
  return "(" + expr_->unparse() + ")";
}

std::string ParenExpr::CppCode() {
  return "(" + expr_->CppCode() + ")";
}

/*!
  Let Expr
*/
LetExpr::LetExpr(Stmts* stmts, Expr* expr):
    stmts_(stmts), expr_(expr)  {}

/*!
 * @brief unparse let expression
 * @param none
 * @return string
 */
std::string LetExpr::unparse() {
  return "let " + stmts_->unparse() + " in " + expr_-> unparse() + " end";
}

std::string LetExpr::CppCode() {
  return "({ "+stmts_->CppCode() + " " + expr_-> CppCode() + ";})";
}

/*!
  If Else Expr
*/ 
IfElseExpr::IfElseExpr(Expr* predicate, Expr* then_stmt, Expr* else_stmt):
    predicate_(predicate), then_stmt_(then_stmt), else_stmt_(else_stmt) {}

/*!
 * @brief unparse if expression
 * @param none
 * @return string
 */
std::string IfElseExpr::unparse() {
  return "if " + predicate_->unparse() + " then " + then_stmt_->unparse() +
  " else " + else_stmt_->unparse();
}

std::string IfElseExpr::CppCode() {
  return predicate_->CppCode() + " ? " +
    then_stmt_->CppCode() + " : " + else_stmt_->CppCode();
}

/*!
  Not Expr
*/ 
NotExpr::NotExpr(Expr* expr): not_expr_(expr) {}

/*!
 * @brief unparse not expression
 * @param none
 * @return string
 */
std::string NotExpr::unparse() {
  return "!" + not_expr_->unparse();
}

std::string NotExpr::CppCode() {
  return "!" + not_expr_->CppCode();
}

/****************************************************************************
 * Binary Operators
  ***************************************************************************/
/*! Binary Operators */
BinaryOperators::BinaryOperators(Expr* left, Expr* right, TokenType _op):
    left_(left), right_(right), op_(_op) {}

// BasicOp::BasicOp(Expr* left, Expr* right, TokenType op):
//    BasicOp(left, right, op) {}

/*!
 * @brief All the basic binary operators
 * @param none
 * @return string
 */
std::string BinaryOperators::unparse() {
  switch (op_) {
    case kStar:  return left_->unparse() + "*" + right_->unparse();
    case kForwardSlash:  return left_->unparse() + "/" + right_->unparse();
    case kPlusSign: return left_->unparse() + "+" + right_->unparse();
    case kDash: return left_->unparse() + "-" + right_->unparse();
    case kGreaterThan: return left_->unparse() + ">" + right_->unparse();
    case kGreaterThanEqual: return left_->unparse() + ">=" + right_->unparse();
    case kLessThan: return left_->unparse() + "<" + right_->unparse();
    case kLessThanEqual: return left_->unparse() + "<=" + right_->unparse();
    case kEqualsEquals: return left_->unparse() + "==" + right_->unparse();
    case kNotEquals: return left_->unparse() + "!=" + right_->unparse();
    case kAndOp: return left_->unparse() + "&&" + right_->unparse();
    case kOrOp: return left_->unparse() + "||" + right_->unparse();
    default:return "Error";
  }
}

std::string BinaryOperators::CppCode() {
  switch (op_) {
    case kStar:
        return "(" + left_->CppCode() + " * " + right_->CppCode() + ")";
    case kForwardSlash:
        return "(" + left_->CppCode() + "/" + right_->CppCode() + ")";
    case kPlusSign:
        return "(" + left_->CppCode() + "+" + right_->CppCode() + ")";
    case kDash:
        return "(" + left_->CppCode() + "-" + right_->CppCode() + ")";
    case kGreaterThan:
        return "(" + left_->CppCode() + ">" + right_->CppCode() + ")";
    case kGreaterThanEqual:
        return "(" + left_->CppCode() + ">=" + right_->CppCode() + ")";
    case kLessThan:
        return "(" + left_->CppCode() + "<" + right_->CppCode() + ")";
    case kLessThanEqual:
        return "(" + left_->CppCode() + "<=" + right_->CppCode() + ")";
    case kEqualsEquals:
        return "(" + left_->CppCode() + "==" + right_->CppCode() + ")";
    case kNotEquals:
        return "(" + left_->CppCode() + "!=" + right_->CppCode() + ")";
    case kAndOp:
        return "(" + left_->CppCode() + "&&" + right_->CppCode() + ")";
    case kOrOp:
        return "(" + left_->CppCode() + "||" + right_->CppCode() + ")";
    default: return "(" + left_->CppCode() + ">=" + right_->CppCode() + ")";
  }
}

/*******************************************************************************
 * Statements
 ******************************************************************************/

/*! Empty Statements */
EmptyStmts::EmptyStmts() {}

/*!
 * @brief Empty statements
 * @param none
 * @return string
 */
std::string EmptyStmts::unparse() {
  return "\n";
}

std::string EmptyStmts::CppCode() {
  return "\n";
}


/*! Stmt + Stmts */
StmtStmts::StmtStmts(Stmt* stmt_left, Stmts* stmts_right) :
    stmt(stmt_left), stmts(stmts_right) {}

/*!
 * @brief unparse stmt and stmts
 * @param none
 * @return string
 */
std::string StmtStmts::unparse() {
  return stmt->unparse() + stmts->unparse();
}

std::string StmtStmts::CppCode() {
  return stmt->CppCode() + stmts->CppCode();
}

/*! Assignment Stmt */
AssignStmt::AssignStmt(Token* id, Expr* expr) :
    id_(id), expr_(expr) {}

/*!
 * @brief unparse assignment statement
 * @param none
 * @return string
 */
std::string AssignStmt::unparse() {
  return id_->lexeme() + " = " + expr_->unparse() + " ;";
}

std::string AssignStmt::CppCode() {
  return id_->lexeme() + " = " + expr_->CppCode() + ";\n";
}

/*! Statements in {} */
BodyStmt::BodyStmt(Stmts* stmts) :
    stmts_(stmts) {}

/*!
 * @brief unparse statements
 * @param none
 * @return string
 */
std::string BodyStmt::unparse() {
  return "{ " + stmts_->unparse()  + " }";
}

std::string BodyStmt::CppCode() {
  return "{ " + stmts_->CppCode() + "}\n";
}

/*! If Statement */
IfStmt::IfStmt(Expr* predicate, Stmt* then_stmt) :
    predicate_(predicate), then_stmt_(then_stmt) {}

/*!
 * @brief unparse if statement
 * @param none
 * @return string
 */
std::string IfStmt::unparse() {
  return "if (" + predicate_->unparse() + ") " + then_stmt_->unparse();
}

std::string IfStmt::CppCode() {
  return "if ("  + predicate_->CppCode() + ") {" +
    then_stmt_->CppCode() + "}\n";
}

/*! If Else Statement */
IfElseStmt::IfElseStmt(Expr* predicate, Stmt* then_stmt, Stmt* else_stmt) :
    predicate_(predicate), then_stmt_(then_stmt), else_stmt_(else_stmt) {}

/*!
 * @brief unparse If Else statement
 * @param none
 * @return string
 */
std::string IfElseStmt::unparse() {
  return "if ( " + predicate_->unparse() + " ) " +
  then_stmt_->unparse() + " else " + else_stmt_->unparse();
}

std::string IfElseStmt::CppCode() {
  return "if (" + predicate_->unparse() + ") " +
    "{" + then_stmt_->unparse() + "\n}\n" +
    "else " + "{\n" + else_stmt_->unparse() + "\n}\n";
}

/*! Decl Statement */
DeclStmt::DeclStmt(Decl* decl) :
    decl(decl) {}

/*!
 * @brief unparse decl
 * @param none
 * @return string
 */
std::string DeclStmt::unparse() {
  return decl->unparse();
}

std::string DeclStmt::CppCode() {
  return decl->CppCode();
}


/*! Print Statement */
PrintStmt::PrintStmt(Expr* expr): expr_(expr) {}

/*!
 * @brief unparse print statement
 * @param none
 * @return string
 */
std::string PrintStmt::unparse() {
  return "print (" + expr_->unparse() + ") ; \n";
}

std::string PrintStmt::CppCode() {
  return "cout<<" + expr_->CppCode() + ";\n";
}

/*! Repeat Statement */
RepeatStmt::RepeatStmt(Token* id, Expr* begin, Expr* end, Stmt* stmt):
    id_(id), begin_(begin), end_(end), stmt_(stmt) {}

/*!
 * @brief unparse repeat statement
 * @param none
 * @return string
 */
std::string RepeatStmt::unparse() {
  return "repeat (" + id_->lexeme() + " = " + begin_->unparse() + " to "
      + end_->unparse() + ") " + stmt_->unparse();
}

std::string RepeatStmt::CppCode() {
  return "for (" + id_->lexeme() + " = " + begin_->CppCode() + ";"
      + id_->lexeme() + " <= " + end_->unparse() + "; " +
        id_->lexeme() + "++)\n" +
      stmt_->CppCode();
}

/*! While Statement */
WhileStmt::WhileStmt(Expr* predicate, Stmt* body):
    predicate_(predicate), body_(body) {}

/*!
 * @brief unparse while statement
 * @param none
 * @return string
 */
std::string WhileStmt::unparse() {
  return "while (" + predicate_->unparse() + ") { \n" +
  body_->unparse() + "\n } \n";
}

std::string WhileStmt::CppCode() {
  return "while (" + predicate_->CppCode() + ")\n" + body_->CppCode() + "\n";
}

/*! Semi-Colon Statement */
SemiColonStmt::SemiColonStmt() {}

/*!
 * @brief unparse semi-colon statement
 * @param none
 * @return string
 */
std::string SemiColonStmt::unparse() {
  return " ; \n";
}

std::string SemiColonStmt::CppCode() {
  return "; \n";
}

}  // namespace ast
}  // namespace fcal
