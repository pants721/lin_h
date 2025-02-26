#ifndef LIN_H
#define LIN_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// If you want to define your own decimal type (i.e. float instead of
// double) make sure to define this before including `lin.h`:
//
// #define lin_decimal_t float
// #include "lin.h"
#ifndef lin_decimal_t
typedef double lin_decimal_t;
#endif

#define LIN_LOG_ERROR(fmt, ...) \
    fprintf(stderr, "[%s:%d] ERROR: " fmt "\n", __FILE__, __LINE__, \
            ##__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
//
// VECTOR DECLARATION
//
///////////////////////////////////////////////////////////////////////////////

#ifndef LIN_VEC_MAX_SIZE
#define LIN_VEC_MAX_SIZE 1024
#endif

typedef enum {
    DEGREES,
    RADIANS,
} AngleType;

typedef struct {
    size_t dim;
    lin_decimal_t *elements;
} lin_vec_t;

lin_vec_t *lin_vec_create(size_t dim);
lin_vec_t *lin_vec_create_from_array(size_t dim, lin_decimal_t arr[LIN_VEC_MAX_SIZE]);
lin_vec_t *lin_vec_scalar_mult(lin_vec_t *v, lin_decimal_t k);
lin_vec_t *lin_vec_add(lin_vec_t *a, lin_vec_t *b);
lin_vec_t *lin_vec_sub(lin_vec_t *a, lin_vec_t *b);
lin_decimal_t lin_vec_dot(lin_vec_t *a, lin_vec_t *b);
lin_decimal_t lin_vec_len(lin_vec_t *v);
lin_decimal_t lin_vec_angle(lin_vec_t *a, lin_vec_t *b, AngleType angle_type);
lin_vec_t *lin_vec_cross(lin_vec_t *a, lin_vec_t *b);
void _lin_vec_print(lin_vec_t *v);

///////////////////////////////////////////////////////////////////////////////
//
// VECTOR IMPLEMENTATION
//
///////////////////////////////////////////////////////////////////////////////

lin_vec_t *lin_vec_create(size_t dim) {
    lin_vec_t *vec = (lin_vec_t *)malloc(sizeof(lin_vec_t));
    if (vec == NULL) {
        LIN_LOG_ERROR("Failed to allocate memory for lin_vec_t");
        return NULL;
    }

    vec->elements = (lin_decimal_t *)malloc(dim * sizeof(lin_decimal_t));
    if (vec->elements == NULL) {
        LIN_LOG_ERROR("Failed to allocate memory for vector elements");
        free(vec);
        return NULL;
    }

    vec->dim = dim;
    return vec;
}
lin_vec_t *lin_vec_create_from_array(size_t dim, lin_decimal_t arr[LIN_VEC_MAX_SIZE]) {
    lin_vec_t *vec = lin_vec_create(dim);
    
    for (size_t i = 0; i < dim; i++) {
        lin_decimal_t *n = &arr[i];
        if (n == NULL) {
            LIN_LOG_ERROR(
                "Array passed to lin_vec_create_from_array is incompatible with given dimension"
            );
            free(vec);
            return NULL;
        }

        vec->elements[i] = *n;
    }
    
    return vec;
}

lin_vec_t *lin_vec_scalar_mult(lin_vec_t *v, lin_decimal_t k) {
    lin_vec_t *res = lin_vec_create(v->dim);
    for (size_t i = 0; i < v->dim; i++) {
        res->elements[i] = v->elements[i] * k;
    }

    return res;
}

lin_vec_t *lin_vec_add(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch during vector addition (%zu and %zu)",
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    lin_vec_t *res = lin_vec_create(a->dim);
    for (size_t i = 0; i < a->dim; i++) {
        res->elements[i] = a->elements[i] + b->elements[i];
    }

    return res;
}

lin_vec_t *lin_vec_sub(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR(
            "Length mistmatch during vector subtraction (%zu and %zu)",
            a->dim, b->dim
        );
        exit(EXIT_FAILURE);
    }

    lin_vec_t *res = lin_vec_create(a->dim);
    for (size_t i = 0; i < a->dim; i++) {
        res->elements[i] = a->elements[i] - b->elements[i];
    }

    return res;
}

lin_decimal_t lin_vec_dot(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch while taking dot product (%zu and %zu)",
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    lin_decimal_t sum = 0;
    for (size_t i = 0; i < a->dim; i++) {
        sum += a->elements[i] * b->elements[i];
    }

    return sum;
}

lin_decimal_t lin_vec_len(lin_vec_t *v) {
    lin_decimal_t sum = 0;

    for (size_t i = 0; i < v->dim; i++) {
        sum += v->elements[i] * v->elements[i];
    }

    return (lin_decimal_t)sqrt((double)sum);
}

lin_decimal_t lin_vec_angle(lin_vec_t *a, lin_vec_t *b, AngleType angle_type) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch while taking dot product (%zu and %zu)",
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    lin_decimal_t rads = (lin_decimal_t)acos(
        (double)(lin_vec_dot(a, b) /
        (lin_vec_len(a) * lin_vec_len(b)))
    );

    if (angle_type == DEGREES) {
        return (lin_decimal_t)(rads * (180.0 / M_PI));
    }

    return rads;
}

lin_vec_t *lin_vec_cross(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR(
            "Length mistmatch while taking cross product (%zu and %zu)",
            a->dim, b->dim
        );
        exit(EXIT_FAILURE);
    }

    if (a->dim != 3) {
        LIN_LOG_ERROR(
            "Cross product is not defined for vectors of %zu dimensions",
            a->dim
        );
        exit(EXIT_FAILURE);
    }

    lin_decimal_t res_elements[3] = {
        a->elements[1] * a->elements[2] - a->elements[2] * b->elements[1],
        a->elements[2] * b->elements[0] - a->elements[0] * b->elements[2],
        a->elements[0] * b->elements[1] - a->elements[1] * b->elements[0],
    };

    return lin_vec_create_from_array(3, res_elements);
}

void _lin_vec_print(lin_vec_t *v) {
    printf("[ ");
    for (size_t i = 0; i < v->dim; i++) {
        printf("%f ", v->elements[i]);
    }
    printf("]\n");
}

///////////////////////////////////////////////////////////////////////////////
//
// MATRIX DECLARATION
//
///////////////////////////////////////////////////////////////////////////////

#ifndef LIN_MAT_MAX_ROWS
#define LIN_MAT_MAX_ROWS 1024
#endif

#ifndef LIN_MAT_MAX_COLS
#define LIN_MAT_MAX_COLS 1024
#endif

typedef struct {
    size_t rows;
    size_t columns;
} lin_mat_shape_t;

typedef struct {
    lin_mat_shape_t shape;
    lin_decimal_t **elements;
} lin_mat_t;

lin_mat_t *lin_mat_create(lin_mat_shape_t shape);
lin_mat_t *lin_mat_create_from_array(lin_mat_shape_t shape, lin_decimal_t arr[LIN_MAT_MAX_ROWS][LIN_MAT_MAX_COLS]);
lin_mat_t *lin_mat_mult(lin_mat_t *a, lin_mat_t *b);
lin_mat_t *lin_mat_add(lin_mat_t *a, lin_mat_t *b);
lin_mat_t *lin_mat_sub(lin_mat_t *a, lin_mat_t *b);
lin_mat_t *lin_mat_scalar_mult(lin_mat_t *a, lin_decimal_t k);
lin_mat_t *lin_mat_transpose(lin_mat_t *a);
lin_decimal_t lin_mat_det(lin_mat_t *a);
lin_mat_t *lin_mat_identity(size_t n);
lin_mat_t *lin_mat_row(lin_mat_t *a, size_t n);
lin_mat_t *lin_mat_col(lin_mat_t *a, size_t n);
lin_vec_t *lin_mat_row_vec(lin_mat_t *a, size_t n);
lin_vec_t *lin_mat_col_vec(lin_mat_t *a, size_t n);
lin_decimal_t lin_mat_minor_of_element(lin_mat_t *a, size_t row, size_t col);
lin_mat_t *lin_mat_minor(lin_mat_t *a);
lin_decimal_t lin_mat_cofactor_of_element(lin_mat_t *a, size_t row, size_t col);
lin_mat_t *lin_mat_cofactor(lin_mat_t *a);
lin_mat_t *lin_mat_adj(lin_mat_t *a);
lin_mat_t *lin_mat_inv(lin_mat_t *a);
void _lin_mat_print(lin_mat_t *a);

///////////////////////////////////////////////////////////////////////////////
//
// MATRIX IMPLEMENTATION
//
///////////////////////////////////////////////////////////////////////////////

lin_mat_t *lin_mat_create(lin_mat_shape_t shape) {
    lin_mat_t *mat = (lin_mat_t *)malloc(sizeof(lin_mat_t));

    if (mat == NULL) {
        return NULL;
    }

    mat->shape = shape;
    mat->elements = (lin_decimal_t **)malloc(shape.rows * sizeof(lin_decimal_t *));
    if (mat->elements == NULL) {
        LIN_LOG_ERROR(
            "Failed to allocate memory for rows in matrix of dimensions [%zu x %zu]", 
            shape.rows, shape.columns
        );
        return NULL;
    }

    for (size_t i = 0; i < shape.rows; i++) {
        mat->elements[i] = (lin_decimal_t *)malloc(shape.columns * sizeof(lin_decimal_t));
        if (mat->elements[i] == NULL) {
            LIN_LOG_ERROR(
                "Failed to allocate memory for columns in matrix of dimensions [%zu x %zu]", 
                shape.rows, shape.columns
            );

            for (size_t j = 0; j < i; j++) {
                free(mat->elements[j]);
            }
            free(mat->elements);
            free(mat);
            return NULL;
        }
    }

    return mat;
}

lin_mat_t *lin_mat_create_from_array(lin_mat_shape_t shape, lin_decimal_t arr[LIN_MAT_MAX_ROWS][LIN_MAT_MAX_COLS]) {
    lin_mat_t *mat = lin_mat_create(shape);
    
    for (size_t i = 0; i < shape.rows; i++) {
        for (size_t j = 0; j < shape.columns; j++) {
            lin_decimal_t *n = &arr[i][j];
            if (n == NULL) {
                LIN_LOG_ERROR(
                    "Array passed to lin_mat_create_from_array is of incompatible shape with matrix [%zu x %zu]",
                    shape.rows, shape.columns
                );
             
                return NULL;
            }

            mat->elements[i][j] = *n;
        }
    }

    return mat;
}

lin_mat_t *lin_mat_mult(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.columns != b->shape.rows) {
        LIN_LOG_ERROR("Dimension mismatch during matrix multiplication \
                      [%zu x %zu] [%zu x %zu]",
                      a->shape.rows, a->shape.columns,
                      b->shape.rows, b->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t *res = lin_mat_create(a->shape);
    for (size_t a_row = 0; a_row < a->shape.rows; a_row++) {
        for (size_t b_col = 0; b_col < b->shape.columns; b_col++) {
            lin_decimal_t column[b->shape.rows];
            for (size_t i = 0; i < b->shape.rows; i ++) {
                column[i] = b->elements[i][b_col];
            }
            lin_decimal_t d = lin_vec_dot(
                &(lin_vec_t){a->shape.columns, a->elements[a_row]},
                &(lin_vec_t){b->shape.rows, column}
            );
            res->elements[a_row][b_col] = d;
        }
    }


    return res;
}

lin_mat_t *lin_mat_add(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.rows != b->shape.rows
        || a->shape.columns != b->shape.columns) {
        LIN_LOG_ERROR(
            "Dimension mismatch during matrix addition [%zu x %zu] [%zu x %zu]",
            a->shape.rows, a->shape.columns, b->shape.rows, b->shape.columns
        );
        exit(EXIT_FAILURE);
    }

    lin_mat_t *res = lin_mat_create(a->shape);
    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res->elements[row][col] = a->elements[row][col] + b->elements[row][col];
        }
    }

    return res;
}


lin_mat_t *lin_mat_sub(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.rows != b->shape.rows
        || a->shape.columns != b->shape.columns) {
        LIN_LOG_ERROR(
            "Dimension mismatch during matrix addition [%zu x %zu] [%zu x %zu]",
            a->shape.rows, a->shape.columns, b->shape.rows, b->shape.columns
        );
        exit(EXIT_FAILURE);
    }

    lin_mat_t *res = lin_mat_create(a->shape);
    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res->elements[row][col] = a->elements[row][col] - b->elements[row][col];
        }
    }

    return res;
}

lin_mat_t *lin_mat_scalar_mult(lin_mat_t *a, lin_decimal_t k) {
    lin_mat_t *res = lin_mat_create(a->shape);
    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res->elements[row][col] = a->elements[row][col] * k;
        }
    }

    return res;
}

lin_mat_t *lin_mat_transpose(lin_mat_t *a) {
    lin_mat_t *res = lin_mat_create((lin_mat_shape_t){a->shape.columns, a->shape.rows});
    for (size_t col = 0; col < a->shape.columns; col++) {
        for (size_t row = 0; row < a->shape.rows; row++) {
            res->elements[col][row] = a->elements[row][col];
        }
    }

    return res;
}

lin_decimal_t lin_mat_det(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot take determinant of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    size_t n = a->shape.rows;

    if (n == 1) {
        return a->elements[0][0];
    }

    if (n == 2) {
        return a->elements[0][0] * a->elements[1][1] -
            a->elements[0][1] * a->elements[1][0];
    }

    if (n == 3) {
        return (a->elements[0][0] * a->elements[1][1] * a->elements[2][2]) +
        (a->elements[0][1] * a->elements[1][2] * a->elements[2][0]) +
        (a->elements[0][2] * a->elements[1][0] * a->elements[2][1]) -
        (a->elements[0][2] * a->elements[1][1] * a->elements[2][0]) -
        (a->elements[0][1] * a->elements[1][0] * a->elements[2][2]) -
        (a->elements[0][0] * a->elements[1][2] * a->elements[2][1]);
    }

    lin_decimal_t res = 0;
    for (size_t col = 0; col < n; ++col) {
        lin_mat_t *sub = lin_mat_create((lin_mat_shape_t){n - 1, n - 1});
        for (size_t i = 1; i < n; ++i) {
            int subcol = 0;
            for (size_t j = 0; j < n; j++) {
                if (j == col) continue;

                sub->elements[i - 1][subcol++] = a->elements[i][j];
            }
        }
        int sign = (col % 2 == 0) ? 1: -1;
        res += sign * a->elements[0][col] * lin_mat_det(sub);
    }

    return res;

}

/// Where `n` is the dimension [n x n] of the output matrix
lin_mat_t *lin_mat_identity(size_t n) {
    lin_mat_t *res = lin_mat_create((lin_mat_shape_t){n, n});

    for (int i = 0; i < (int)n; i++) {
        res->elements[i][i] = 1;
    }

    return res;
}

lin_mat_t *lin_mat_row(lin_mat_t *a, size_t n) {
    lin_mat_t *row = lin_mat_create((lin_mat_shape_t){1, a->shape.columns});
    for (size_t i = 0; i < a->shape.columns; i++) {
        row->elements[0][i] = a->elements[n][i];
    }
    return row;
}

lin_mat_t *lin_mat_col(lin_mat_t *a, size_t n) {
    lin_mat_t *col = lin_mat_create((lin_mat_shape_t){a->shape.rows, 1});
    for (size_t i = 0; i < a->shape.rows; i++) {
        col->elements[i][0] = a->elements[i][n];
    }
    return col;
}

lin_vec_t *lin_mat_row_vec(lin_mat_t *a, size_t n) {
    lin_vec_t *row = (lin_vec_t *)malloc(sizeof(lin_vec_t));
    row->dim = a->shape.columns;
    row->elements = (lin_decimal_t *)malloc(sizeof(lin_decimal_t) * a->shape.columns);
    for (size_t i = 0; i < a->shape.columns; i++) {
        row->elements[i] = a->elements[n][i];
    }
    return row;
}

lin_vec_t *lin_mat_col_vec(lin_mat_t *a, size_t n) {
    lin_vec_t *col = (lin_vec_t *)malloc(sizeof(lin_vec_t));
    col->dim = a->shape.columns;
    col->elements = (lin_decimal_t *)malloc(sizeof(lin_decimal_t) * a->shape.columns);
    for (size_t i = 0; i < a->shape.rows; i++) {
        col->elements[i] = a->elements[i][n];
    }
    return col;
}

lin_decimal_t lin_mat_minor_of_element(lin_mat_t *a, size_t row, size_t col) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot take minor of element of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    size_t n = a->shape.rows;
    lin_mat_t *sub = lin_mat_create((lin_mat_shape_t){n - 1, n - 1});

    bool passed_row = false;
    for (size_t i = 0; i < a->shape.rows; i++) {
        if (i == row) {
            passed_row = true;
            continue;
        }

        bool passed_col = false;
        for (size_t j = 0; j < a->shape.columns; j++) {
            if (j == col) {
                passed_col = true;
                continue;
            }

            size_t sub_row = passed_row ? i - 1 : i;
            size_t sub_col = passed_col ? j - 1 : j;
            sub->elements[sub_row][sub_col] = a->elements[i][j];
        }
    }

    return lin_mat_det(sub);
}

lin_mat_t *lin_mat_minor(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot find minor matrix of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t *res = lin_mat_create(a->shape);

    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res->elements[row][col] = lin_mat_minor_of_element(a, row, col);
        }
    }

    return res;
}

lin_decimal_t lin_mat_cofactor_of_element(lin_mat_t *a, size_t row, size_t col) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot find cofactor of element of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_decimal_t min = lin_mat_minor_of_element(a, row, col);
    lin_decimal_t mul  = (lin_decimal_t)pow(-1.0, (double)(row + col));

    return mul * min;
}

lin_mat_t *lin_mat_cofactor(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot find cofactor matrix of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t *res = lin_mat_create(a->shape);

    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res->elements[row][col] = lin_mat_cofactor_of_element(a, row, col);
        }
    }

    return res;
}

lin_mat_t *lin_mat_adj(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot find adjoint matrix of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t *min = lin_mat_cofactor(a);
    return lin_mat_transpose(min);
}

lin_mat_t *lin_mat_inv(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot find inverse of non-square matrix [%zu x %zu]",
                      a->shape.rows, a->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t *adj = lin_mat_adj(a);
    lin_decimal_t det = lin_mat_det(a);

    return lin_mat_scalar_mult(adj, (1.0 / det));
}

void _lin_mat_print(lin_mat_t *a) {
    for (size_t row = 0; row < a->shape.rows; row++) {
        printf("[ ");
        for (size_t col = 0; col < a->shape.columns; col++) {
            printf("%f ", a->elements[row][col]);
        }
        printf("]\n");
    }
}

#endif // LIN_H
