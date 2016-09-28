#pragma once

/**
is_rotation_matrix
------------------

.. cpp:function:: template <T, N> is_rotation_matrix(matrix)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T** - The parameter value type **(must be a floating point type)**.
    * **N** (``std::size_t``) - The root of the matrix size.
  
  **Function parameters**
   
    * **matrix** (``std::array<T, N*N>``) - The matrix.

  **Returns**
   
    ``bool`` - True if the matrix is a rotation matrix and false otherwise.
    
  Checks whether ``matrix`` is a `rotation matrix <https://en.wikipedia.org/wiki/Rotation_matrix>`_ or not. It is accepted if and only if its inverse is (approximately) equal to its transpose, its determinate is (approximately) either ``1.0`` or ``-1.0`` and ``N`` is greater than ``1``.
  
  The allowed deviation must be within ``std::numeric_limits<T>::digits10 - 3`` digits for each requirement. This means, that invocations with rotation matrices containing very large or very small values might return in a false negative, as the rounding errors will be to large to distinguish these matrices from non-rotation matrices.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      // Identity matrix
      std::cout << mant::is_rotation_matrix({1.0, 0.0, 0.0, 1.0}) << "\n";
      // 3-dimensional rotation matrix along the x-axis
      const double angle = mant::pi<double> / 2.0;
      std::cout << mant::is_rotation_matrix({1.0, 0.0, 0.0, 0.0, std::cos(angle), std::sin(angle), 0.0, -std::sin(angle), std::cos(angle)}) << "\n";
      // Matrix of ones
      std::cout << mant::is_rotation_matrix({1.0, 1.0, 1.0, 1.0}) << "\n";
      std::cout << std::endl;
      
      return 0;
    }
*/
template <typename T, std::size_t number_of_rows>
bool is_rotation_matrix(
    const std::array<T, number_of_rows*number_of_rows>& matrix);
    
//
// Implementation
//

template <typename T, std::size_t number_of_rows>
bool is_rotation_matrix(
    const std::array<T, number_of_rows*number_of_rows>& matrix) {
  static_assert(std::is_same<T, double>::value, "The type for the matrix's elements must be *double*.");
  // *Note:* The matrix is guaranteed to be squared by compile time checks, as it must hold exactly *number_of_rows*^2 elements.
  
  if (number_of_rows < 2) {
    //  A rotation matrix must be have at least 2 dimensions, ...
    return false;
  }
  
  //  ... be finite and ...
  for (const auto element : matrix) {
    if(!std::isfinite(element)) {
      return false;
    }
  }
  
  // ... its transpose must be equal to its inverse.
  // *Note:* Since A^-1 must be equal to A^T, this also means that A*A^T = I must hold true (which is faster and less error prone to compute, as no matrix inversion is needed).
  std::array<T, number_of_rows*number_of_rows> identity_matrix;
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, number_of_rows, number_of_rows, number_of_rows, 1.0, matrix.data(), number_of_rows, matrix.data(), number_of_rows, 0.0, identity_matrix.data(), number_of_rows);
  
  // *Note:* The result of A*A^T is always symmetric.
  for (std::size_t column_index = 0; column_index < number_of_rows; ++column_index) {
    if (std::fabs(identity_matrix[column_index * (number_of_rows + 1)] - 1) > std::pow(10.0, -std::numeric_limits<T>::digits10 - 3)) {
      return false; 
    }
    for (std::size_t row_index = column_index + 1; row_index < number_of_rows; ++row_index) {
      if (std::fabs(identity_matrix[column_index * number_of_rows + row_index]) > std::pow(10.0, -std::numeric_limits<T>::digits10 - 3)) {
        return false; 
      }
    }
  }

  return true;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("is_rotation_matrix", "[assertion][is_rotation_matrix]") {
  CHECK((mant::is_rotation_matrix<double, 0>({}) == false));
  CHECK((mant::is_rotation_matrix<double, 2>({1.0, 0.0, 0.0, std::numeric_limits<double>::quiet_NaN()}) == false));
  CHECK((mant::is_rotation_matrix<double, 1>({1.0}) == false));
  CHECK((mant::is_rotation_matrix<double, 2>({1.0, 0.0, 0.0, -2.0}) == false));
  CHECK((mant::is_rotation_matrix<double, 4>({1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0}) == true));
  CHECK((mant::is_rotation_matrix<double, 4>({1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0}) == true));
}
#endif