#ifndef COMMON_BENCHMARK_H
#define COMMON_BENCHMARK_H

#include "mmio.h"

#ifdef __cplusplus
extern "C" {
#endif

void print_array(const char* message, double *values, int size);
void print_array_int(const char* message, int *values, int size);
void print_matrix(const char* message, double *values, int size);

/** Reads the dimensions of the matrix (n - number of rows, m - number
    of columns, nnzs - number of non-zeros) from the given Matrix
    Market file.  If the given file contains an array rather than a
    COO matrix, nnzs will be set to n;
*/
void read_mm_matrix_size(FILE *f, int *n, int *m, int *nnzs, MM_typecode* mcode);

/** Reads a matrix market file for a symmetric real valued sparse
    matrix and returns the matrix in 0-indexed CSR form. */
void read_mm_sym_matrix(FILE* f, MM_typecode mcode,
                        int n, int nnzs,
                        double *values, int* col_ind, int *row_ptr
                        );

/** Reads a matrix market file for a symmetric real valued sparse
    matrix and returns the matrix in 1-indexed CSR form, with ALL
    VALUES INCLUDED (i.e. when setting A(i, j), we also set A(j, i)).
*/
void read_mm_unsym_matrix(FILE* f, MM_typecode mcode,
                          int n, int *nnzs,
                          double *values, int* col_ind, int *row_ptr);

/** Returns the zero indexed array of values. */
void read_mm_array(FILE *f, MM_typecode code, int nnzs, double *values);
double* read_rhs(FILE* g, int* n, int *nnzs);
void read_system_matrix_sym_csr(FILE* f, int* n, int *nnzs, int** col_ind, int** row_ptr, double** values);

void read_system_matrix_unsym_csr(FILE* f,
                                  int* n, int *nnzs,
                                  int** col_ind, int** row_ptr, double** values);

/** Reads a generic sparse matrix in Matrix Market format and converts it to CSR */
void read_ge_mm_csr(char* fname,
                    int* n, int *nnzs,
                    int** col_ind, int** row_ptr, double** values);

void write_vector_to_file(const char* filename, double* vector, int size);

/**
    Implements element-wise multiplication of two vectors (treats first vector as
    diagonal matrix and multiplies it to second vector): z = x (*) y.
*/
void elementwise_xty(const int n, const double *x, const double *y, double *z);

#ifdef __cplusplus
}
#endif

#endif
