/*******************************************************************************
 * Name            : Matrix.h
 * Project         : fcal
 * Module          : matrix
 * Copyright       : group-faraday
 * Original Author : Alish Akhmetzyanov, Cyrus Chen
 *
 ******************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../include/Matrix.h"

// constructor
matrix::matrix(int i, int j)  : rows(i), cols(j) {
    data = new float *[rows];
    for (int k = 0; k < rows; k++) {
        data[k] = new float[cols];
        for (int s = 0; s < rows; s++)
            data[k][s] = 0;
    }
}


// copy constructor
matrix::matrix(const matrix& m) : rows(m.rows), cols(m.cols) {
    data = new float *[rows];
    for (int k = 0; k < rows; k++) {
        data[k] = new float[cols];

        for (int s = 0; s < rows; s++)
            data[k][s] = *m.access(k, s);
        }
}

/*
// Copy assignment

matrix &matrix::operator=(const matrix &m) 
{
    this->rows = m.rows;
    this->cols = m.cols;
    this->data = m.data;
    return *this;
}

*/

// destructor

matrix::~matrix() {
    for (int i = 0; i < rows; i++) {
        delete[] data[i];
    }
    delete[] data;
}




int matrix::n_rows () {return rows;}
int matrix::n_cols () {return cols;}


float* matrix::access(const int i, const int j) const {
    int m = i;
    int n = j;
    return &data[m][n];
}


matrix operator*(const matrix &mleft_, const matrix &mright_) {
    matrix mleft = mleft_;
    matrix mright = mright_;
    if (mleft.n_cols() != mright.n_rows()) {
        std::cerr << "Error, input matrices cannot be multiplied together";
        exit(2);
    }

    matrix m(mleft.n_rows(), mright.n_cols());
    for (int i = 0; i != m.n_rows(); i++) {
        for (int j = 0; j != m.n_cols(); j++) {
            for (int k = 0; k != mleft.n_cols(); k++) {
                (*m.access(i, j)) +=
                    (*mleft.access(j, k)) * (*mright.access(k, j));
            }
        }
    }
    return m;
}




std::ostream& operator<<(std::ostream &os, matrix &m) {
    os << m.n_rows() << " " << m.n_cols() << std::endl;

    for (int i=0; i < m.n_rows(); i++) {
        for (int j=0; j < m.n_cols(); j++) {
            os << *m.access(i, j) << "  ";
        }
        os << std::endl;
    }
    return os;
}



matrix matrix::matrix_read(std::string filename) {
    const char * cfilename = filename.c_str();

    std::ifstream myfile(cfilename, std::ifstream::in);
    int countrows, countcols;
    myfile >> countrows >> countcols;

    // initiate matrix
    matrix m(countrows, countcols);

    for (int x = 0; x < countrows; x++) {
        for (int y = 0; y < countcols; y++) {
            myfile >> *m.access(x, y);
        }
    }
    return m;
}








