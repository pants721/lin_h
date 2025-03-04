#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define lin_decimal_t float
#include "lin.h"


int main(int argc, char **argv) {
    float els[3 * 3] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);

    float res = lin_mat_minor_of_element(mat, 0, 0);
    printf("%f\n", res);


    return 0;
}
