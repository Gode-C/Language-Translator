/*******************************************************************************
 * Name            : scanner.cc
 * Project         : 3081
 * Module          : scanner
 * Description     : scan function
 * Copyright       : group faraday All rights reserved.
 * Original Author : Alish Akhmetzyanov, Cyrus Chen
 ******************************************************************************/


#include "../include/scanner.h"
#include <stdio.h>
#include "../include/token.h"

namespace fcal {
namespace scanner {

/*
regex_t* int_kwd = make_regex("^int");
regex_t* float_kwd = make_regex("^float");
regex_t* bool_kwd = make_regex("^bool");
regex_t* true_kwd = make_regex("^true");
regex_t* false_kwd = make_regex("^false");
regex_t* while_kwd = make_regex("^while");
regex_t* string_kwd = make_regex("^string");
regex_t* matrix_kwd = make_regex("^matrix");
regex_t* let_kwd = make_regex("^let");
regex_t* in_kwd = make_regex("^in");
regex_t* end_kwd = make_regex("^end");
regex_t* if_kwd = make_regex("^if");
regex_t* then_kwd = make_regex("^then");
regex_t* else_kwd = make_regex("^else");
regex_t* repeat_kwd = make_regex("^repeat");
regex_t* print_kwd = make_regex("^print");
regex_t* to_kwd = make_regex("^to");

regex_t* int_const = make_regex("^[0-9]+");
regex_t* float_const = make_regex("^[-+]?([0-9]*\\.[0-9]+|[0-9]+)");
regex_t* string_const = make_regex("^\"[^\n\"]*\"");   // Not sure

regex_t* variable_name = make_regex("^[a-zA-Z][_a-zA-Z0-9]*");

regex_t* left_paren = make_regex("^\\(");
regex_t* right_paren = make_regex("^\\)");
regex_t* left_curly = make_regex("^\\{");
regex_t* right_curly = make_regex("^\\}");
regex_t* left_square = make_regex("^\\[");
regex_t* right_square = make_regex("^\\]");

regex_t* semi_colon = make_regex("^;");
regex_t* colon = make_regex("^:");

regex_t* assign = make_regex("^=");
regex_t* plus = make_regex("^\\+");
regex_t* star = make_regex("^\\*");
regex_t* dash = make_regex("^\\-");
regex_t* forward_slash = make_regex("^\\/");

regex_t* less = make_regex("^<");
regex_t* less_equal = make_regex("^<=");
regex_t* greater = make_regex("^>");
regex_t* greater_equal = make_regex("^>=");
regex_t* equals_equals = make_regex("^==");
regex_t* not_equals = make_regex("^!=");
regex_t* and_op = make_regex("^&&");
regex_t* or_op = make_regex("^\\|\\|");
regex_t* not_op = make_regex("^!");

regex_t* eof = make_regex("^\\z");

regex_t* white_space = make_regex("^[\n\t\r ]+");
*/

// regex_t* block_comment = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");



/*! 
  initialize scanner that all regular expressions are
  putted in an array.
*/
void Scanner::Initialize_Regex() {
  white_space_ = make_regex("^[\n\t\r ]+");
  block_comment_ = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
  line_comment_ = make_regex("^//[^\n]*\n");

  regexes[0] = make_regex("^int");
  regexes[1]  = make_regex("^float");
  regexes[2]  = make_regex("^boolean");
  regexes[3]  = make_regex("^true");
  regexes[4]  = make_regex("^false");
  regexes[5]  = make_regex("^while");
  regexes[6]  = make_regex("^string");
  regexes[7]  = make_regex("^matrix");
  regexes[8]  = make_regex("^let");
  regexes[9]  = make_regex("^in");
  regexes[10]  = make_regex("^end");
  regexes[11]  = make_regex("^if");
  regexes[12]  = make_regex("^then");
  regexes[13]  = make_regex("^else");
  regexes[14]  = make_regex("^repeat");
  regexes[15]  = make_regex("^print");
  regexes[16]  = make_regex("^to");

  regexes[17]  = make_regex("^[0-9]+");
  regexes[18]  = make_regex("^[0-9]+\\.[0-9]+");
  regexes[19]  = make_regex("^\"[^\n\"]*\"");   // Not sure

  regexes[20]  = make_regex("^[a-zA-Z][_a-zA-Z0-9]*");

  regexes[21]  = make_regex("^\\(");
  regexes[22]  = make_regex("^\\)");
  regexes[23]  = make_regex("^\\{");
  regexes[24]  = make_regex("^\\}");
  regexes[25]  = make_regex("^\\[");
  regexes[26]  = make_regex("^\\]");

  regexes[27]  = make_regex("^;");
  regexes[28]  = make_regex("^:");

  regexes[29]  = make_regex("^=");
  regexes[30]  = make_regex("^\\+");
  regexes[31]  = make_regex("^\\*");
  regexes[32]  = make_regex("^\\-");
  regexes[33]  = make_regex("^\\/");

  regexes[34]  = make_regex("^<");
  regexes[35]  = make_regex("^<=");
  regexes[36]  = make_regex("^>");
  regexes[37]  = make_regex("^>=");
  regexes[38]  = make_regex("^==");
  regexes[39]  = make_regex("^!=");
  regexes[40]  = make_regex("^&&");
  regexes[41]  = make_regex("^\\|\\|");
  regexes[42]  = make_regex("^!");

  regexes[43] = make_regex("^\\z");
}

Scanner::Scanner() {
  Initialize_Regex();
}

/*! 
  clean up the input so it doesn't contain whitespace and comment
*/
int Scanner::consume_whitespace_and_comments(regex_t *white_space,
                                             regex_t *block_comment,
                                             regex_t *line_comment,
                                             const char *text) {
  int num_matched_chars = 0;
  int total_num_matched_chars = 0;
  int still_consuming_white_space;

  do {
    still_consuming_white_space = 0;  // exit loop if not reset by a match

/*! Try to match white space */
    num_matched_chars = match_regex(white_space, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }

/*!  Try to match block comments */
    num_matched_chars = match_regex(block_comment, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }

    num_matched_chars = match_regex(line_comment, text);
    total_num_matched_chars += num_matched_chars;
    if (num_matched_chars > 0) {
      text = text + num_matched_chars;
      still_consuming_white_space = 1;
    }
  } while (still_consuming_white_space);

  return total_num_matched_chars;
} /* consume_whitespace_and_comments() */

/*! 
  scan the target text and match with the regular expressions
  for different token type.
*/
Token* Scanner::Scan(const char* text) {
  Token* head = new Token();
  head->SetNext(NULL);
  Token* temp1;
  Token* tail = head;
  temp1 = head;


  int num_matched_chars = 0;

/*! Consume leading white space and comments */
  num_matched_chars =
      consume_whitespace_and_comments(white_space_, block_comment_,
      line_comment_, text);
  /*
   * text is a character pointer that points to the current beginning of the
   * array of characters in the input. Adding an integer value to it advances
   * the pointer that many elements in the array. Thus, text is increased so
   * that it points to the current location in the input.
   */
  text = text + num_matched_chars;

  int max_num_matched_chars = 0;
  TokenType match_type = kLexicalError;


  while (text[0] != '\0') {
    max_num_matched_chars = 0;
    match_type = kLexicalError;

    while (temp1->next() != NULL)
      temp1 = temp1->next();
    int i = 0;
    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kIntKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kFloatKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kBoolKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kTrueKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kFalseKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kWhileKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kStringKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kMatrixKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kLetKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kInKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kEndKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kIfKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kThenKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kElseKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kRepeatKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kPrintKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kToKwd;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kIntConst;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kFloatConst;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kStringConst;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kVariableName;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kLeftParen;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kRightParen;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kLeftCurly;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kRightCurly;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kLeftSquare;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kRightSquare;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kSemiColon;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kColon;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kAssign;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kPlusSign;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kStar;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kDash;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kForwardSlash;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kLessThan;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kLessThanEqual;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kGreaterThan;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kGreaterThanEqual;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kEqualsEquals;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kNotEquals;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kAndOp;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kOrOp;
    }

    num_matched_chars = match_regex(regexes[i++], text);
    if (num_matched_chars > max_num_matched_chars) {
      max_num_matched_chars = num_matched_chars;
      match_type = kNotOp;
    }



    // num_matched_chars = match_regex(eof, text);
    // if (num_matched_chars > max_num_matched_chars) {
    //   max_num_matched_chars = num_matched_chars;
    //   match_type = kEndOfFile;
    // }

    Token* temp = new Token();

    if (match_type == kLexicalError) {
      temp->SetTerminal(kLexicalError);
      temp->SetLexeme(std::string(text, 1));
      temp->SetNext(NULL);
      temp1->SetNext(temp);

      text++;
    } else {
      temp->SetTerminal(match_type);
      temp->SetLexeme(std::string(text, max_num_matched_chars));
      temp->SetNext(NULL);
      temp1->SetNext(temp);

      text += max_num_matched_chars;
    }

    num_matched_chars = consume_whitespace_and_comments
        (white_space_, block_comment_, line_comment_, text);
    text = text + num_matched_chars;
    tail = temp;
  }  // end while

  Token* eof_token = new Token();
  eof_token->SetTerminal(kEndOfFile);
  eof_token->SetLexeme("\0");
  eof_token->SetNext(NULL);

  tail->SetNext(eof_token);


  return head->next();
}

} /* namespace scanner */
} /* namespace fcal */
