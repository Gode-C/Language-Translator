/*******************************************************************************
 * Name            : Matrix.CC
 * Project         : fcal
 * Module          : matrix
 * Copyright       : group-faraday
 * Original Author : Alish Akhmetzyanov, Cyrus Chen
 *
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

class matrix {
 public:
    matrix(int i, int j);
    matrix(const matrix& m);
    ~matrix();

    int n_rows();
    int n_cols();

    float *access(const int i, const int j) const;
    friend std::ostream& operator<<(std::ostream &os, matrix &m);

    friend matrix operator*(const matrix &mleft_, const matrix &mright_);

    static matrix matrix_read(std::string filename);

 private:
    matrix() {}
    int rows;
    int cols;

    /* Your implementation of "data" may vary.  There are ways in
       which data can be an array of arrays and thus simplify the
       access method, at the cost of complicating the process of
       allocating space for data.  The choice is entirely up to
       you. */
    float **data;
};

#endif  // PROJECT_INCLUDE_MATRIX_H_



