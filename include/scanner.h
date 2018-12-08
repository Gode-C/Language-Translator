/*******************************************************************************
 * Name            : scanner.h
 * Project         : fcal
 * Module          : scanner
 * Description     : Header file for scanner module
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell Alish, Akhmetzyanov, Cyrus Chen
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_SCANNER_H_
#define PROJECT_INCLUDE_SCANNER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <string>
#include "../include/token.h"
#include "../include/regex.h"



/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Class Definitions
 *
 * You will need to define a Token class (remember, a pointer of type Token *
 * is returned by the scan method of the scanner class.)
 * Also, Classes are typically declared (and sometimes also defined) in
 * their own header files
 *
 * You will also need to define a Scanner class with a scan and other methods
 ******************************************************************************/

class Scanner {
 private:
  regex_t* white_space_;
  regex_t* line_comment_;
  regex_t* block_comment_;
  regex_t* regexes[44];

 public:
  Scanner();
  void Initialize_Regex();
  Token* Scan(const char*);
  int consume_whitespace_and_comments(regex_t *white_space,
                                    regex_t *block_comment,
                                    regex_t* line_comment,
                                    const char *text);
};  // end Scanner



} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_SCANNER_H_
