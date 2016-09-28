#pragma once

/**
.. cpp:function:: template <N> constexpr factorial()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N** (``std::size_t``) - A number.

  **Returns**
   
    ``std::size_t`` - The `factorial <https://en.wikipedia.org/wiki/Factorial>`_ :math:`\text{N}!` or ``0`` if it overflows.
    
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      std::cout << "0! = " << factorial<0>() << "\n";
      std::cout << "1! = " << factorial<1>() << "\n";
      std::cout << "2! = " << factorial<2>() << "\n";
      std::cout << "3! = " << factorial<3>() << "\n";
      std::cout << "100! = " << factorial<3>() << "\n";
      std::cout << std::endl;
      
      return 0;
    }
*/
template <typename T>
constexpr std::size_t factorial(
    const T n);

//
// Implementation
//

template <typename T>
constexpr std::size_t factorial(
    const T n) {
  static_assert(std::is_integral<T>::value, "The type for 'n' must be an integer.");
  
  if (n < 2) {
    return 1;
  }
  
  std::size_t factorial = 1;
  for (T k = 2; k <= n; ++k) {
    if (n - 1 > std::numeric_limits<std::size_t>::max() / n) {
      return 0;
    }
    
    factorial *= k;
  }

  return factorial;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("factorial", "[combinatorics][factorial]") {
  CHECK(factorial(0) == 1);
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(3) == 6);
  CHECK(factorial(100) == 0);
}
#endif