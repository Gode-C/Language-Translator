/*******************************************************************************
 * Name            : read_input.h
 * Project         : fcal
 * Module          : scanner
 * Description     : A utility to read the contents of a file into a
 *                   character buffer
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell, Alish Akhmetzyanov, Cyrus Chen
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_READ_INPUT_H_
#define PROJECT_INCLUDE_READ_INPUT_H_

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Functions
 ******************************************************************************/

/*!
  Function reads input 
*/
char *ReadInput(int argc, char **argv);

/*!
  Function reads input from the target file
*/
char *ReadInputFromFile(const char *filename);

} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_READ_INPUT_H_
