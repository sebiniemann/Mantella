#pragma once

/**
is_permutation_vector
=====================

.. cpp:function:: template <T, N1, N2> constexpr is_permutation_vector(vector)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T** - The parameter value type **(must be an integer type)**.
    * **N1** (``std::size_t``) - The parameter size.
    * **N2** (``std::size_t``) - The number of parameters.

  **Function parameters**
   
    * **vector** (``std::array<T, N>``) - The vector.

  **Returns**
   
    ``bool`` - True if the vector is a permutation vector and false otherwise.
    
  Checks whether ``vector`` is a `permutation vector <https://de.wikipedia.org/wiki/Permutationsmatrix>`_ or not. It is accepted if and only if all elements are unique and within :math:`(0, ..., \text{N}-1)` and ``N`` is greater than ``0``.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      std::cout << mant::is_permutation_vector({4, 0, 1, 3, 2}) << "\n";
      // Indices outside of [0, 4].
      std::cout << mant::is_permutation_vector({5, 1, 2, 3, 4}) << "\n";
      // Non-unique indices.
      std::cout << mant::is_permutation_vector({0, 1, 2, 3, 0}) << "\n";
      std::cout << std::endl;
      
      return 0;
    }
*/

/**
.. cpp:function:: template <T, N> constexpr is_permutation_vector(vector)

  .. versionadded:: 1.x
  
  Short-cut for ``is_permutation_vector<T, N, 1>(vector)``.
*/
template <typename T, std::size_t N>
constexpr bool is_permutation_vector(
    const std::array<T, N>& vector);

//
// Implementation
//

template <typename T1, typename T2, std::size_t N>
bool is_permutation_vector(
    const typename std::array<T1, N>::const_iterator vector,
    const T2 number_of_elements) {
  // A permutation over elements from [0, `number_of_elements` - 1] must be non-empty, ...
  if (vector.is_empty()) {
    return false;
  }
  
  // ... have only unique elements ...
  if (std::adjacent_find(vector, std::next(vector, N)) != std::next(vector, N)) {
    return false;
  }
  
  // ...  from [0, `numberOfElements` - 1].
  const auto& minmax = std::minmax_element(vector, std::next(vector, N));
  if (minmax.first < 0 || minmax.second > N - 1) {
    return false;
  }

  return true;
}

template <typename T, std::size_t N>
bool is_permutation_vector(
    const typename std::array<T, N>::const_iterator vector) {
  return is_permutation_vector(vector, N);
}