#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define lin_decimal_t float
#include "lin.h"


int main(int argc, char **argv) {
    float els[LIN_MAX_ROWS][LIN_MAX_COLS] = {
        {3, 1, 6},
        {5, 2, -1},
        {-4, 3, 0}
    };
    lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 3}, els);
    lin_mat_t *b = lin_mat_scalar_mult(lin_mat_col(mat, 0), 2);
    _lin_mat_print(b);
    return 0;
}
