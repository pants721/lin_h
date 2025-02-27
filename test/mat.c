#include "unity.h"
#define lin_decimal_t float
#include "lin.h"

#define LIN_TEST_ASSERT_EQUAL_FLOAT_MAT(exp, acc, w, h)

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_mat_scalar_mult() {
    lin_mat_t *mat = lin_mat_create((lin_mat_shape_t){3, 3});
    float els[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            mat->elements[i][j] = els[i][j];
        }
    }

    float exp[3][3] = {
        {2, 4, 6},
        {8, 10, 12},
        {14, 16, 18}
    };

    lin_mat_t *acc = lin_mat_scalar_mult(mat, 2.0);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            TEST_ASSERT_EQUAL_FLOAT(exp[i][j], acc->elements[i][j]);
        }
    }
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_mat_scalar_mult);
    return UNITY_END();
}
