#ifndef LIN_H
#define LIN_H

#include <stdlib.h>
#include <stdio.h>

#define LIN_LOG_ERROR(fmt, ...) \
    fprintf(stderr, "[%s:%d] ERROR: " fmt "\n", __FILE__, __LINE__, \
            ##__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
//
// MATRIX IMPLEMENTATION
//
///////////////////////////////////////////////////////////////////////////////

#ifndef LIN_MAX_ROWS
#define LIN_MAX_ROWS 512
#endif

#ifndef LIN_MAX_COLS
#define LIN_MAX_COLS 512
#endif

typedef struct {
    int rows;
    int columns;
} lin_mat_shape_t;

typedef struct {
    lin_mat_shape_t shape;
    float elements[LIN_MAX_ROWS][LIN_MAX_COLS];
} lin_mat_t;

// TODO: Define actual vec type
float lin_vec_dot(float *a, float *b, size_t len_a, size_t len_b) {
    if (len_a != len_b) {
        LIN_LOG_ERROR("Length mistmatch during dot product (%zu and %zu)", 
                      len_a, len_b);
        exit(EXIT_FAILURE);
    }

    float sum = 0;  
    for (int i = 0; i < (int)len_a; i++) {
        sum += a[i] * b[i];
    }

    return sum;
}

lin_mat_t lin_mat_mult(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.columns != b->shape.rows) {
        LIN_LOG_ERROR("Dimension mismatch during matrix multiplication \
                      [%d x %d] [%d x %d]",
                      a->shape.rows, a->shape.columns, 
                      b->shape.rows, b->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t res = { {a->shape.rows, b->shape.columns}, {{0}}};
    for (int a_row = 0; a_row < a->shape.rows; a_row++) {
        for (int b_col = 0; b_col < b->shape.columns; b_col++) {
            float column[b->shape.rows];
            for (int i = 0; i < b->shape.rows; i ++) {
                column[i] = b->elements[i][b_col];
            }
            float d = lin_vec_dot(a->elements[a_row], column, 
                          a->shape.columns, b->shape.rows);
            res.elements[a_row][b_col] = d;
        }
    }


    return res;
}

lin_mat_t lin_mat_add(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.rows != b->shape.rows 
        || a->shape.columns != b->shape.columns) {
        LIN_LOG_ERROR(
            "Dimension mismatch during matrix addition [%d x %d] [%d x %d]",
            a->shape.rows, a->shape.columns, b->shape.rows, b->shape.columns
        );
        exit(EXIT_FAILURE);
    }

    lin_mat_t res = { {a->shape.rows, a->shape.columns}, {{0}}};
    for (int row = 0; row < a->shape.rows; row++) {
        for (int col = 0; col < a->shape.columns; col++) {
            res.elements[row][col] = a->elements[row][col] + 
                b->elements[row][col];
        }
    }

    return res;
}


lin_mat_t lin_mat_sub(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.rows != b->shape.rows 
        || a->shape.columns != b->shape.columns) {
        LIN_LOG_ERROR(
            "Dimension mismatch during matrix subtraction [%d x %d] [%d x %d]",
            a->shape.rows, a->shape.columns, 
            b->shape.rows, b->shape.columns
        );
        exit(EXIT_FAILURE);
    }

    lin_mat_t res = { {a->shape.rows, a->shape.columns}, {{0}}};
    for (int row = 0; row < a->shape.rows; row++) {
        for (int col = 0; col < a->shape.columns; col++) {
            res.elements[row][col] = a->elements[row][col] - 
                b->elements[row][col];
        }
    }

    return res;
}

lin_mat_t lin_mat_scalar_mult(lin_mat_t *a, float k) {
    lin_mat_t res = { {a->shape.rows, a->shape.columns}, {{0}}};
    for (int row = 0; row < a->shape.rows; row++) {
        for (int col = 0; col < a->shape.columns; col++) {
            res.elements[row][col] = k * a->elements[row][col];
        }
    }

    return res;
}

lin_mat_t lin_mat_transpose(lin_mat_t *a) {
    lin_mat_t res = { {a->shape.columns, a->shape.rows}, {{0}}};
    for (int col = 0; col < a->shape.columns; col++) {
        for (int row = 0; row < a->shape.rows; row++) {
            res.elements[col][row] = a->elements[row][col];
        }
    }

    return res;
}

float lin_mat_det(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot take determinant of non-square matrix [%d x %d]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    int n = a->shape.rows;

    if (n == 1) {
        return a->elements[0][0];
    }

    if (n == 2) {
        return a->elements[0][0] * a->elements[1][1] - 
            a->elements[0][1] * a->elements[1][0];
    }

    LIN_LOG_ERROR(
        "Determinants are not implemented for matrices larger than [2 x 2] \
        (a is [%d x %d])", 
        a->shape.rows, a->shape.columns
    );
    exit(EXIT_FAILURE);
}

void _lin_mat_print(lin_mat_t *m) {
    for (int row = 0; row < m->shape.rows; row++) {
        printf("[ ");
        for (int col = 0; col < m->shape.columns; col++) {
            printf("%.1f ", m->elements[row][col]);
        }
        printf("]\n");
    }
}

#endif // LIN_H
