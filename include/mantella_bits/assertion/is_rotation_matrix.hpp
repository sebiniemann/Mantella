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
template <typename T, std::size_t N>
bool is_rotation_matrix(
    const typename std::array<T, N*N>::const_iterator begin_matrix);
    const typename std::array<T, N*N>::const_iterator end_matrix);
    
//
// Implementation
//