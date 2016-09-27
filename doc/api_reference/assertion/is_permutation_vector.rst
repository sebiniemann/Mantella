
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