#pragma once

/**
number_of_coefficients
======================

.. cpp:function:: template <number_of_elements, largest_degree> constexpr number_of_coefficients()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **number_of_elements** (``std::size_t``) - A number of indeterminates.
    * **largest_degree** (``std::size_t``) - A largest degree.

  **Returns**
   
    ``std::size_t`` - The number of coefficients.
  
  Calculates the number of coefficients in a polynomial with ``number_of_elements`` indeterminates and largest degree ``largest_degree``.
  
  For ``number_of_elements = 2`` and ``largest_degree = 3``, such a polynomial can be written as
  
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
    
    1 + \sum_{d = 1}^\text{largest_degree} \binom{\text{number_of_elements}}{d}
  
  In consequence, ``1`` will be returned if either ``number_of_elements`` or ``largest_degree`` is ``0``. 
  
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
template <std::size_t number_of_elements, std::size_t largest_degree>
constexpr std::size_t number_of_coefficients();

//
// Implementation
//

template <std::size_t number_of_elements, std::size_t largest_degree>
constexpr std::size_t number_of_coefficients() {
  if (number_of_elements == 0 || largest_degree == 0) {
    return 1;
  }
  
  // Checks if adding the constant term will result in an overflow.
  if (number_of_elements == std::numeric_limits<std::size_t>::max()) {
    return 0;
  }
  
  // Initialises the polynomial size for the linear and constant term.
  // Due to an earlier check, the polynomial will be at least linear from here on.
  std::size_t polynomial_size = number_of_elements + 1;
  
  // Sums up the number of parameter combinations for each degree > 0.
  for (decltype(largest_degree) degree = 2; degree <= largest_degree; ++degree) {
    const auto number_of_combinations = n_choose_k(number_of_elements + degree - 1, degree);
    // Checks if the number of combinations overflowed.
    if (number_of_combinations == 0) {
      return 0;
    }

    // Checks if adding the number of combinations will result in an overflow.
    if (number_of_combinations > std::numeric_limits<std::size_t>::max() - polynomial_size) {
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
    CHECK((number_of_coefficients<0, 0>() == 1));
    CHECK((number_of_coefficients<2, 0>() == 1));
    CHECK((number_of_coefficients<0, 2>() == 1));
    CHECK((number_of_coefficients<2, 1>() == 3));
    CHECK((number_of_coefficients<2, 3>() == 10));
    CHECK((number_of_coefficients<10, 1000>() == 0));
  }
#endif