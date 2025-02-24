#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "lin.h"

int main(int argc, char **argv) {
    lin_mat_t a = { {2, 4}, {
        {1, 2, 3, 4}, 
        {5, 6, 7, 8}
    } };
    lin_mat_t b = { {2, 4}, {
        {1, 2, 3, 4}, 
        {5, 6, 7, 8}
    } };
    lin_mat_t n = lin_mat_transpose(&a);

    _lin_mat_print(&n);

    return 0;
}
