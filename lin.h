#ifndef LIN_H
#define LIN_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LIN_LOG_ERROR(fmt, ...) \
    fprintf(stderr, "[%s:%d] ERROR: " fmt "\n", __FILE__, __LINE__, \
            ##__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
//
// VECTOR IMPLEMENTATION
//
///////////////////////////////////////////////////////////////////////////////

typedef enum {
    DEGREES,
    RADIANS,
} AngleType;

typedef struct {
    /// `len` is the number of components, not the magnitude. 
    /// For the vector length, use `lin_vec_len`.
    size_t dim;
    float *elements;
} lin_vec_t;

lin_vec_t lin_vec_scalar_mult(lin_vec_t *v, float k) {
    lin_vec_t res = *v;
    for (size_t i = 0; i < v->dim; i++) {
        res.elements[i] *= k;
    }

    return res;
}

lin_vec_t lin_vec_add(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch during vector addition (%zu and %zu)", 
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    lin_vec_t res = *a;
    for (size_t i = 0; i < a->dim; i++) {
        res.elements[i] += b->elements[i];
    }

    return res;
}

lin_vec_t lin_vec_sub(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch during vector subtraction (%zu and %zu)", 
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    lin_vec_t res = *a;
    for (size_t i = 0; i < a->dim; i++) {
        res.elements[i] -= b->elements[i];
    }

    return res;
}

float lin_vec_dot(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch while taking dot product (%zu and %zu)", 
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    float sum = 0;  
    for (size_t i = 0; i < a->dim; i++) {
        sum += a->elements[i] * b->elements[i];
    }

    return sum;
}

float lin_vec_len(lin_vec_t *v) {
    float sum = 0;

    for (size_t i = 0; i < v->dim; i++) {
        sum += v->elements[i] * v->elements[i];
    }

    return sqrtf(sum);
}

float lin_vec_angle(lin_vec_t *a, lin_vec_t *b, AngleType angle_type) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch while taking dot product (%zu and %zu)", 
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    float rads = acosf(lin_vec_dot(a, b) / (lin_vec_len(a) * lin_vec_len(b)));

    if (angle_type == DEGREES) {
        return rads * (180.0 / M_PI);
    }

    return rads;
}

lin_vec_t lin_vec_cross(lin_vec_t *a, lin_vec_t *b) {
    if (a->dim != b->dim) {
        LIN_LOG_ERROR("Length mistmatch while taking cross product (%zu and %zu)", 
                      a->dim, b->dim);
        exit(EXIT_FAILURE);
    }

    if (a->dim != 3) {
        LIN_LOG_ERROR("Cross product is not defined for vectors of %zu dimensions",
                      a->dim);
        exit(EXIT_FAILURE);
    }

    float res_elements[3] = {
        a->elements[1] * a->elements[2] - a->elements[2] * b->elements[1],
        a->elements[2] * b->elements[0] - a->elements[0] * b->elements[2],
        a->elements[0] * b->elements[1] - a->elements[1] * b->elements[0],
    };

    return (lin_vec_t){3, res_elements};
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
    size_t rows;
    size_t columns;
} lin_mat_shape_t;

typedef struct {
    lin_mat_shape_t shape;
    float elements[LIN_MAX_ROWS][LIN_MAX_COLS];
} lin_mat_t;

lin_mat_t lin_mat_mult(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.columns != b->shape.rows) {
        LIN_LOG_ERROR("Dimension mismatch during matrix multiplication \
                      [%zu x %zu] [%zu x %zu]",
                      a->shape.rows, a->shape.columns, 
                      b->shape.rows, b->shape.columns);
        exit(EXIT_FAILURE);
    }

    lin_mat_t res = {{a->shape.rows, b->shape.columns}, {{0}}};
    for (size_t a_row = 0; a_row < a->shape.rows; a_row++) {
        for (size_t b_col = 0; b_col < b->shape.columns; b_col++) {
            float column[b->shape.rows];
            for (size_t i = 0; i < b->shape.rows; i ++) {
                column[i] = b->elements[i][b_col];
            }
            float d = lin_vec_dot(
                &(lin_vec_t){a->shape.columns, a->elements[a_row]}, 
                &(lin_vec_t){b->shape.rows, column}
            );
            res.elements[a_row][b_col] = d;
        }
    }


    return res;
}

lin_mat_t lin_mat_add(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.rows != b->shape.rows 
        || a->shape.columns != b->shape.columns) {
        LIN_LOG_ERROR(
            "Dimension mismatch during matrix addition [%zu x %zu] [%zu x %zu]",
            a->shape.rows, a->shape.columns, b->shape.rows, b->shape.columns
        );
        exit(EXIT_FAILURE);
    }

    lin_mat_t res = *a;
    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res.elements[row][col] += b->elements[row][col];
        }
    }

    return res;
}


lin_mat_t lin_mat_sub(lin_mat_t *a, lin_mat_t *b) {
    if (a->shape.rows != b->shape.rows 
        || a->shape.columns != b->shape.columns) {
        LIN_LOG_ERROR(
            "Dimension mismatch during matrix subtraction [%zu x %zu] [%zu x %zu]",
            a->shape.rows, a->shape.columns, b->shape.rows, b->shape.columns
        );
        exit(EXIT_FAILURE);
    }

    lin_mat_t res = *a;
    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res.elements[row][col] -= b->elements[row][col];
        }
    }

    return res;
}

lin_mat_t lin_mat_scalar_mult(lin_mat_t *a, float k) {
    lin_mat_t res = *a;
    for (size_t row = 0; row < a->shape.rows; row++) {
        for (size_t col = 0; col < a->shape.columns; col++) {
            res.elements[row][col] *= k;
        }
    }

    return res;
}

lin_mat_t lin_mat_transpose(lin_mat_t *a) {
    lin_mat_t res = *a;
    for (size_t col = 0; col < a->shape.columns; col++) {
        for (size_t row = 0; row < a->shape.rows; row++) {
            res.elements[col][row] = a->elements[row][col];
        }
    }

    return res;
}

float lin_mat_det(lin_mat_t *a) {
    if (a->shape.rows != a->shape.columns) {
        LIN_LOG_ERROR("Cannot take determinant of non-square matrix [%zu x %zu]",
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
        (a is [%zu x %zu])", 
        a->shape.rows, a->shape.columns
    );
    exit(EXIT_FAILURE);
}

/// Where `n` is the dimension [n x n] of the output matrix
lin_mat_t lin_mat_identity(size_t n) {
    lin_mat_t res = {{n, n}, {{0}}};
    
    for (int i = 0; i < (int)n; i++) {
        res.elements[i][i] = 1;
    }

    return res;
}

void _lin_mat_print(lin_mat_t *m) {
    for (size_t row = 0; row < m->shape.rows; row++) {
        printf("[ ");
        for (size_t col = 0; col < m->shape.columns; col++) {
            printf("%.1f ", m->elements[row][col]);
        }
        printf("]\n");
    }
}

#endif // LIN_H
