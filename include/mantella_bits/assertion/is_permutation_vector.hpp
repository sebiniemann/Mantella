#pragma once

/**
is_permutation_vector
=====================

.. cpp:function:: template <T, N> constexpr is_permutation_vector(vector)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T** - The vector value type **(must be an integer type)**.
    * **N** (``std::size_t``) - The vector size.

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
template <typename T, std::size_t N>
constexpr bool is_permutation_vector(
    std::array<T, N> vector);

//
// Implementation
//

template <typename T, std::size_t N>
constexpr bool is_permutation_vector(
    std::array<T, N> vector) {
  // A permutation over elements from [0, `number_of_elements` - 1] must be non-empty, ...
  if (vector.empty()) {
    return false;
  }
  
  // ... has only unique elements, ...
  std::sort(vector.begin(), vector.end());
  if (std::adjacent_find(vector.cbegin(), vector.cend()) != vector.cend()) {
    return false;
  }
  
  // ... all within [0, `numberOfElements` - 1].
  const auto& minmax = std::minmax_element(vector.cbegin(), vector.cend());
  if (*minmax.first < 0 || *minmax.second > N - 1) {
    return false;
  }

  return true;
}

#if defined(MANTELLA_BUILD_TESTS) 
TEST_CASE("is_permutation_vector", "[assertion][is_permutation_vector]") {
  bool is_permutation_vector = mant::is_permutation_vector<unsigned int, 5>({4, 0, 1, 3, 2});
  CHECK(is_permutation_vector == true);
  is_permutation_vector = mant::is_permutation_vector<unsigned int, 5>({5, 1, 2, 3, 4});
  CHECK(is_permutation_vector == false);
  is_permutation_vector = mant::is_permutation_vector<unsigned int, 5>({0, 1, 2, 3, 0});
  CHECK(is_permutation_vector == false);
}
#endif