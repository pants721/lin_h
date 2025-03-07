#include "unity.h"
#include "unity_internals.h"
#include "lin.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void create(void) {
    lin_mat_t *mat = lin_mat_create((lin_mat_shape_t){3, 3});
    TEST_ASSERT_NOT_NULL(mat);
    TEST_ASSERT_NOT_NULL(mat->elements);
}

void create_from_array(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(els, mat->elements, 9);
}

void mult(void) {
    float a_el[2 * 4] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
    };

    lin_mat_t *a = lin_mat_create_from_array((lin_mat_shape_t){2, 4}, a_el);

    float b_el[4 * 2] = {
        1, 2,
        3, 4,
        5, 6,
        7, 8,
    };

    lin_mat_t *b = lin_mat_create_from_array((lin_mat_shape_t){4, 2}, b_el);

    lin_mat_t *res = lin_mat_mult(a, b);

    float exp[2 * 2] = {
        50, 60,
        114, 140
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 4);
}

void add(void) {
    float els_a[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *a = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els_a);

    float els_b[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *b = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els_b);

    lin_mat_t *res = lin_mat_add(a, b);

    float exp[3 * 3] = {
        2, 4, 6,
        8, 10, 12,
        14, 16, 18
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 9);
}

void sub(void) {
    float els_a[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *a = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els_a);

    float els_b[3 * 3] = {
        3, 6, 9,
        12, 15, 18,
        21, 24,27 
    };
    lin_mat_t *b = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els_b);

    lin_mat_t *res = lin_mat_sub(a, b);

    float exp[3 * 3] = {
        -2, -4, -6,
        -8, -10, -12,
        -14, -16, -18
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 9);
}

void scalar_mult(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_mat_t *res = lin_mat_scalar_mult(mat, 2.0);

    float exp[3 * 3] = {
        2, 4, 6,
        8, 10, 12,
        14, 16, 18
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 9);


    float els2[2 * 4] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
    };
    lin_mat_t *mat2 = lin_mat_create_from_array((lin_mat_shape_t){2, 4}, els2);

    lin_mat_t *res2 = lin_mat_scalar_mult(mat2, 2.0);

    float exp2[2 * 4] = {
        2, 4, 6, 8,
        10, 12, 14, 16,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp2, res2->elements, 8);
}

void transpose(void) {
    float els[2 * 4] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){2, 4}, els);

    lin_mat_t *res = lin_mat_transpose(mat);

    float exp[4 * 2] = {
        1, 5,
        2, 6,
        3, 7,
        4, 8,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 8);
}

void det1x1(void) {
    float els[1] = {
        9.2
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){1, 1}, els);

    float res = lin_mat_det(mat);

    TEST_ASSERT_EQUAL(9.2, res);
}

void det2x2(void) {
    float els[2 * 2] = {
        4, 2,
        6, 9,
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){2, 2}, els);

    float res = lin_mat_det(mat);

    TEST_ASSERT_EQUAL(24, res);
}

void det3x3(void) {
    float els[3 * 3] = {
        4, 2, 0,
        6, 9, 1,
        5, 120, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    float res = lin_mat_det(mat);

    TEST_ASSERT_EQUAL(-254, res);
}

void det4x4(void) {
    float els[4 * 4] = {
        4, 2, 0, 8,
        6, 9, 1, 90,
        5, 120, 9, 1,
        0, 700, 2, 5,
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){4, 4}, els);

    float res = lin_mat_det(mat);

    TEST_ASSERT_EQUAL(-1918318, res);
}

void identity(void) {
    lin_mat_t *res = lin_mat_identity(5);
    float exp[5 * 5] = {
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 1,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 5 * 5);
}

void row(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_mat_t *row1 = lin_mat_row(mat, 0);
    float exp1[3] = {1, 2, 3};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp1, row1->elements, 3);


    lin_mat_t *row2 = lin_mat_row(mat, 1);
    float exp2[3] = {4, 5, 6};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp2, row2->elements, 3);

    lin_mat_t *row3 = lin_mat_row(mat, 2);
    float exp3[3] = {7, 8, 9};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp3, row3->elements, 3);
}

void col(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_mat_t *col1 = lin_mat_col(mat, 0);
    float exp1[3] = {
        1,
        4,
        7,
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp1, col1->elements, 3);

    lin_mat_t *col2 = lin_mat_col(mat, 1);
    float exp2[3] = {
        2, 
        5, 
        8
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp2, col2->elements, 3);

    lin_mat_t *col3 = lin_mat_col(mat, 2);
    float exp3[3] = {
        3, 
        6, 
        9
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp3, col3->elements, 3);
}

void row_vec(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_vec_t *row1 = lin_mat_row_vec(mat, 0);
    float exp1[3] = {1, 2, 3};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp1, row1->elements, 3);


    lin_vec_t *row2 = lin_mat_row_vec(mat, 1);
    float exp2[3] = {4, 5, 6};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp2, row2->elements, 3);

    lin_vec_t *row3 = lin_mat_row_vec(mat, 2);
    float exp3[3] = {7, 8, 9};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp3, row3->elements, 3);
}

void col_vec(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_vec_t *col1 = lin_mat_col_vec(mat, 0);
    float exp1[3] = {
        1,
        4,
        7,
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp1, col1->elements, 3);

    lin_vec_t *col2 = lin_mat_col_vec(mat, 1);
    float exp2[3] = {
        2, 
        5, 
        8
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp2, col2->elements, 3);

    lin_vec_t *col3 = lin_mat_col_vec(mat, 2);
    float exp3[3] = {
        3, 
        6, 
        9
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp3, col3->elements, 3);
}

void minor_of_element(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    TEST_ASSERT_EQUAL(-3, lin_mat_minor_of_element(mat, 0, 0));
    TEST_ASSERT_EQUAL(-6, lin_mat_minor_of_element(mat, 0, 1));
    TEST_ASSERT_EQUAL(-3, lin_mat_minor_of_element(mat, 0, 2));
    TEST_ASSERT_EQUAL(-6, lin_mat_minor_of_element(mat, 1, 0));
    TEST_ASSERT_EQUAL(-12, lin_mat_minor_of_element(mat, 1, 1));
    TEST_ASSERT_EQUAL(-6, lin_mat_minor_of_element(mat, 1, 2));
    TEST_ASSERT_EQUAL(-3, lin_mat_minor_of_element(mat, 2, 0));
    TEST_ASSERT_EQUAL(-6, lin_mat_minor_of_element(mat, 2, 1));
    TEST_ASSERT_EQUAL(-3, lin_mat_minor_of_element(mat, 2, 2));
}

void minor(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_mat_t *res = lin_mat_minor(mat);

    float exp[3 * 3] = {
        -3, -6, -3,
        -6, -12, -6,
        -3, -6, -3,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 9);
}

void cofactor_of_element(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    TEST_ASSERT_EQUAL(-3, lin_mat_cofactor_of_element(mat, 0, 0));
    TEST_ASSERT_EQUAL(6, lin_mat_cofactor_of_element(mat, 0, 1));
    TEST_ASSERT_EQUAL(-3, lin_mat_cofactor_of_element(mat, 0, 2));
    TEST_ASSERT_EQUAL(6, lin_mat_cofactor_of_element(mat, 1, 0));
    TEST_ASSERT_EQUAL(-12, lin_mat_cofactor_of_element(mat, 1, 1));
    TEST_ASSERT_EQUAL(6, lin_mat_cofactor_of_element(mat, 1, 2));
    TEST_ASSERT_EQUAL(-3, lin_mat_cofactor_of_element(mat, 2, 0));
    TEST_ASSERT_EQUAL(6, lin_mat_cofactor_of_element(mat, 2, 1));
    TEST_ASSERT_EQUAL(-3, lin_mat_cofactor_of_element(mat, 2, 2));
}

void cofactor(void) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_mat_t *res = lin_mat_cofactor(mat);

    float exp[3 * 3] = {
        -3, 6, -3,
        6, -12, 6,
        -3, 6, -3,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 9);
}

void adj(void) {
    float els[3 * 3] = {
        1, 3, 2,
        4, 6, 5,
        7, 9, 8
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    lin_mat_t *res = lin_mat_adj(mat);

    float exp[3 * 3] = {
        3, -6, 3,
        3, -6, 3,
        -6, 12, -6,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 9);
}

void inv(void) {
    float els[2 * 2] = {
        4, 3,
        3, 2,
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){2, 2}, els);

    lin_mat_t *res = lin_mat_inv(mat);

    float exp[2 * 2] = {
        -2, 3,
        3, -4,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 4);
}

lin_decimal_t sq(lin_decimal_t n) {
    return n * n;
}

void map(void) {
    float els[2 * 2] = {
        4, 3,
        3, 2,
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){2, 2}, els);
    lin_mat_t *res = lin_mat_map(mat, sq);
    float exp[2 * 2] = {
        16, 9,
        9, 4,
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 4);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(create);
    RUN_TEST(create_from_array);
    RUN_TEST(mult);
    RUN_TEST(add);
    RUN_TEST(sub);
    RUN_TEST(scalar_mult);
    RUN_TEST(transpose);
    RUN_TEST(det1x1);
    RUN_TEST(det2x2);
    RUN_TEST(det3x3);
    RUN_TEST(det4x4);
    RUN_TEST(identity);
    RUN_TEST(row);
    RUN_TEST(col);
    RUN_TEST(row_vec);
    RUN_TEST(col_vec);
    RUN_TEST(minor_of_element);
    RUN_TEST(minor);
    RUN_TEST(cofactor_of_element);
    RUN_TEST(cofactor);
    RUN_TEST(adj);
    RUN_TEST(inv);
    RUN_TEST(map);
    return UNITY_END();
}
