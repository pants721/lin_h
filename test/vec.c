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
    lin_vec_t *vec = lin_vec_create(3);
    TEST_ASSERT_NOT_NULL(vec);
    TEST_ASSERT_NOT_NULL(vec->elements);
}

void create_from_array(void) {
    float els[3] = {1, 2, 3};
    lin_vec_t *vec = lin_vec_create_from_array(3, els);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(els, vec->elements, 3);
}

void scalar_mult(void) {
    float els[3] = {1, 2, 3};
    lin_vec_t *vec = lin_vec_create_from_array(3, els);
    float exp[3] = {2, 4, 6};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, lin_vec_scalar_mult(vec, 2)->elements, 3);
}

void add(void) {
    float els1[3] = {1, 2, 3};
    lin_vec_t *vec1 = lin_vec_create_from_array(3, els1);
    float els2[3] = {4, 5, 6};
    lin_vec_t *vec2 = lin_vec_create_from_array(3, els2);

    lin_vec_t *res = lin_vec_add(vec1, vec2);

    float exp[3] = {5, 7, 9};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 3);
}

void sub(void) {
    float els1[3] = {1, 2, 3};
    lin_vec_t *vec1 = lin_vec_create_from_array(3, els1);
    float els2[3] = {4, 5, 6};
    lin_vec_t *vec2 = lin_vec_create_from_array(3, els2);

    lin_vec_t *res = lin_vec_sub(vec1, vec2);

    float exp[3] = {-3, -3, -3};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 3);
}

void dot(void) {
    float els1[3] = {1, 2, 3};
    lin_vec_t *vec1 = lin_vec_create_from_array(3, els1);
    float els2[3] = {4, 5, 6};
    lin_vec_t *vec2 = lin_vec_create_from_array(3, els2);

    float res = lin_vec_dot(vec1, vec2);

    TEST_ASSERT_EQUAL(32, res);
}

void len(void) {
    float els[3] = {3, 4, 12};
    lin_vec_t *vec = lin_vec_create_from_array(3, els);
    TEST_ASSERT_EQUAL(13, lin_vec_len(vec));
}

void angle_deg(void) {
    float els1[3] = {1, 2, 3};
    lin_vec_t *vec1 = lin_vec_create_from_array(3, els1);
    float els2[3] = {4, 5, 6};
    lin_vec_t *vec2 = lin_vec_create_from_array(3, els2);

    float res = lin_vec_angle(vec1, vec2, DEGREES);

    TEST_ASSERT_EQUAL_FLOAT(12.933154491899119, res);
}

void angle_rad(void) {
    float els1[3] = {1, 2, 3};
    lin_vec_t *vec1 = lin_vec_create_from_array(3, els1);
    float els2[3] = {4, 5, 6};
    lin_vec_t *vec2 = lin_vec_create_from_array(3, els2);

    float res = lin_vec_angle(vec1, vec2, RADIANS);

    TEST_ASSERT_EQUAL_FLOAT(0.225726128552734, res);
}

void cross(void) {
    float els1[3] = {1, 2, 3};
    lin_vec_t *vec1 = lin_vec_create_from_array(3, els1);
    float els2[3] = {3, 4, 5};
    lin_vec_t *vec2 = lin_vec_create_from_array(3, els2);

    lin_vec_t *res = lin_vec_cross(vec1, vec2);

    float exp[3] = {-2, 4, -2};

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(exp, res->elements, 3);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(create);
    RUN_TEST(create_from_array);
    RUN_TEST(scalar_mult);
    RUN_TEST(add);
    RUN_TEST(sub);
    RUN_TEST(dot);
    RUN_TEST(len);
    RUN_TEST(angle_deg);
    RUN_TEST(angle_rad);
    RUN_TEST(cross);
    return UNITY_END();
}
