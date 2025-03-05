# lin

Simple linear algebra library for C

> [!WARNING]
> This library was made for educational purposes with my very poor C standards. Be wary of using it in anything actually important.

## Usage
To use lin in your project, just include `lin.h` in your compilation.

By default, lin uses floats as its decimal data type. To specify a custom decimal data type, you can define `lin_decimal_t`:
```c
#define lin_decimal_t double
```

Due to some limitations of C that I have yet to outsmart, elements for a matrix or a vector must be declared as a 1-dimensional array prior to creating a `lin_mat_t` or `lin_vec_t` object. For example, to declare a 3 x 2 matrix:
```c
float els[3 * 2] = {
  1, 2,
  3, 4,
  5, 6,
};

lin_mat_t *mat = lin_mat_create_from_array((lin_mat_shape_t){3, 2}, els);
```
Similarly, for a vector:
```c
float els[3] = {1, 2, 3};
lin_vec_t *vec = lin_vec_create_from_array(3, els);
```

### Matrices
The following functions are implemented for matrices:
+ Multiplication: `lin_mat_mult`
+ Addition: `lin_mat_add`
+ Subtraction: `lin_mat_sub`
+ Multiplication by a scalar: `lin_mat_scalar_mult`
+ Transposition: `lin_mat_transpose`
+ Determinants: `lin_mat_det`
+ Identity matrices: `lin_mat_identity`
+ Row matrix: `lin_mat_row`
+ Column matrix: `lin_mat_col`
+ Row vector: `lin_mat_row_vec`
+ Column vector: `lin_mat_col_vec`
+ Minor of matrix element: `lin_mat_minor_of_element`
+ Minor matrix: `lin_mat_minor`
+ Cofactor of matrix element: `lin_mat_cofactor_of_element`
+ Cofactor matrix: `lin_mat_cofactor`
+ Adjugate / classical adjoint: `lin_mat_adj`
+ Inverse: `lin_mat_inv`

### Vectors
The following functions are implemented for vectors:
+ Addition: `lin_vec_add`
+ Subtraction: `lin_vec_sub`
+ Multiplication by a scalar: `lin_vec_scalar_mult`
+ Dot product: `lin_vec_dot`
+ Length / Magnitude: `lin_vec_len`
+ Angle between two vectors: `lin_vec_angle`
+ Cross product: `lin_vec_cross`

## Testing
Lin uses [Unity](https://github.com/ThrowTheSwitch/Unity) and [Meson](https://mesonbuild.com/) for unit testing.
To run the tests, navigate to the root directory of the project and run `meson test -C build`.
Alternatively, navigate to the `build/` directory and run `meson test`.
