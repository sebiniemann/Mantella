#pragma once

/**
number_of_coefficients
======================

.. cpp:function:: template <N1, N2> constexpr number_of_coefficients()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N1** (``std::size_t``) - A number of indeterminates.
    * **N2** (``std::size_t``) - A largest degree.

  **Returns**
   
    ``std::size_t`` - The number of coefficients.
  
  Calculates the number of coefficients in a polynomial with ``N1`` indeterminates and largest degree ``N2``.
  
  For ``N1 = 2`` and ``N2 = 3``, such a polynomial can be written as
  
  .. math::
  
    a_1 x_1^3 +
    a_2 x_1^2 x_2 +
    a_3 x_1 x_2^2 +
    a_4 x_2^3 +
    a_5 x_1^2 +
    a_6 x_1 x_2 +
    a_7 x_2^2 +
    a_8 x_1 +
    a_9 x_2 +
    a_10
  
  The number of coefficients will be equal to:

  .. math::
    
    1 + \sum_{d = 1}^\text{N2} \binom{\text{N1}}{d}
  
  In consequence, ``1`` will be returned if either ``N1`` or ``N2`` is ``0``. 
  
  This is typically used together with :cpp:func:`polynomial`, to calculate the number of coefficients to be stored at compile-time.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      std::cout << mant::number_of_coefficients<0, 0>() << "\n";
      std::cout << mant::number_of_coefficients<0, 2>() << "\n";
      std::cout << mant::number_of_coefficients<2, 0>() << "\n";
      std::cout << mant::number_of_coefficients<2, 1>() << "\n";
      std::cout << mant::number_of_coefficients<2, 3>() << "\n";
      std::cout << mant::number_of_coefficients<100, 10>() << "\n";
      std::cout << std::endl;
      
      return 0;
    }
*/
template <std::size_t N1, std::size_t N2>
constexpr std::size_t number_of_coefficients();

//
// Implementation
//

template <std::size_t N1, std::size_t N2>
constexpr std::size_t number_of_coefficients() {
  if (N1 == 0 || N2 == 0) {
    return 1;
  }
  
  // Checks if adding the constant term will result in an overflow.
  if (N1 == std::numeric_limits<std::size_t>::max()) {
    return 0;
  }
  
  // Initialises the polynomial size for the linear and constant term.
  // Due to an earlier check, the polynomial will be at least linear from here on.
  std::size_t polynomial_size = N1 + 1;
  
  // Sums up the number of parameter combinations for each degree > 0.
  for (std::size_t degree = 2; degree <= N2; ++degree) {
    const std::size_t number_of_combinations = n_choose_k(N1 + degree - 1, degree);
    // Checks if the number of combinations overflowed.
    if (number_of_combinations == 0) {
      return 0;
    }

    // Checks if adding the number of combinations will result in an overflow.
    if (std::numeric_limits<std::size_t>::max() - polynomial_size < number_of_combinations) {
      return 0;
    }

    polynomial_size += number_of_combinations;
  }

  return polynomial_size;
}

//
// Tests
//

#if defined(MANTELLA_BUILD_TESTS)
  TEST_CASE("number_of_coefficients", "[number_of_coefficients]") {
    CEHCK(number_of_coefficients<0, 0> == 1);
    CEHCK(number_of_coefficients<2, 0> == 1);
    CEHCK(number_of_coefficients<0, 2> == 1);
    CEHCK(number_of_coefficients<2, 1> == 3);
    CEHCK(number_of_coefficients<2, 3> == 10);
    CEHCK(number_of_coefficients<100, 10> == 0);
  }
#endif