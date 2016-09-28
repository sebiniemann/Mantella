#pragma once

/**
polynomials
===========

.. cpp:function:: template <T, number_of_elements, N2, N3> constexpr polynomials(parameters)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T** - The parameters and polynomials value type **(must be a floating point type)**.
    * **number_of_elements** (``std::size_t``) - The number of indeterminates.
    * **N2** (``std::size_t``) - A largest degree.
    * **N3** (``std::size_t``) - The number of parameters.

  **Function parameters**
   
    * **parameters** (``std::array<T, N3*number_of_elements>``) - The parameters.

  **Returns**
   
    ``std::array<T, N3*mant::`` :cpp:any:`number_of_coefficients` ``<number_of_elements,N2>()>`` - The polynomials.
  
  Generates a polynomial with ``number_of_elements`` indeterminates and largest degree ``N2`` for each of the ``N3`` parameters (all with length ``number_of_elements``) in ``parameters``. Each polynomial is written as a tuple with one polynomial term per element.
  
  The indeterminates :math:`(x_1, \ldots, x_\text{number_of_elements})` of the ``n``-th polynomial are substituted by ``(parameters[n], ..., parameters[n + number_of_elements-1])`` and all coefficients are assumed to be ``1.0``.
  
  The result is filled by one polynomial after another, going from the first parameter's polynomial to the last parameter's. The terms within each tuple are going from the terms with degree ``N2`` (lexicographic ordering along the indeterminates indices) to degree 0. The last value (degree 0) will always be ``1.0``.
  
  In consequence, for ``number_of_elements = 2``, ``N2 = 3`` and ``N3 = 2``, the result will be ordered as
  
  .. math::
  
    \bigg((
      x_{1,1}^3,
      x_{1,1}^2 x_{1,2},
      x_{1,1} x_{1,2}^2,
      x_{1,2}^3,
      x_{1,1}^2,
      x_{1,1} x_{1,2},
      x_{1,2}^2,
      x_{1,1},
      x_{1,2},
      1.0
    ), (
      x_{2,1}^3,
      x_{2,1}^2 x_{2,2},
      x_{2,1} x_{2,2}^2,
      x_{2,2}^3,
      \ldots
    )\bigg)
  
  If either ``number_of_elements`` or ``N2`` is ``0``, the return value will be ``{1.0, ..., 1.0}`` (having the length ``N3``). If ``N3`` is ``0``, an empty array will be returned.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      constexpr std::size_t number_of_indeterminates = 2;
      constexpr std::size_t largest_degree = 2;
      constexpr std::size_t number_of_parameters = 3;
      
      constexpr std::array<double, number_of_indeterminates> parameters = {0.2, -9.0};
      constexpr auto polynomials = mant::polynomials<number_of_indeterminates, largest_degree, number_of_parameters>(parameters);
      
      auto begin_parameters = parameters.cbegin();
      auto begin_polynomials = polynomials.cbegin();
      const auto serialise_array = [](const std::string& serialised_array, const auto element) { return serialised_array + ", " + element; };
      for (std::size_t n = 0; n < number_of_parameters; ++n) {
        std::cout << std::accumulate(begin_parameters, std::next(begin_parameters + number_of_indeterminates, "", serialise_array) << " -> "<< std::accumulate(begin_polynomials, std::next(begin_polynomials + number_of_indeterminates, "", serialise_array) << "\n";
        
        std::advance(begin_parameters, number_of_indeterminates);
        std::advance(begin_polynomials, number_of_indeterminates);
      }
      std::cout << std::endl;
      
      return 0;
    }
*/
template <typename T, std::size_t number_of_elements, std::size_t largest_degree>
constexpr std::array<T, number_of_coefficients(number_of_elements, largest_degree)> polynomial(
    const std::array<T, number_of_elements>& parameter);

//
// Implementation
//

template <typename T, std::size_t number_of_elements, std::size_t largest_degree>
constexpr std::array<T, number_of_coefficients(number_of_elements, largest_degree)> polynomial(
    const std::array<T, number_of_elements>& parameter) {
  static_assert(std::is_floating_point<T>::value, "The type for the parameter's elements must be a floating point.");
      
  std::array<T, number_of_coefficients(number_of_elements, largest_degree)> polynomial;
  
  if (polynomial.empty()) {
    return polynomial;
  }
  
  if (number_of_elements == 0 || largest_degree == 0) {
    polynomial.fill(1.0);
    return polynomial;
  }

  // Generates all terms for degree > 1
  std::array<std::size_t, largest_degree> parameter_indicies;
  std::size_t polynomial_index = 0;
  for (auto d = largest_degree; d > 1; --d) {
    /* Iterates through all *number_of_elements*-adic numbers having *degree* digits, skipping all number whose values are not monotonically decreasing from the less to the most significant digit.
     * If *number_of_elements* was set to 3 and *degree* to 4, we would get:
     *
     * (0, 0, 0, 0)
     * (1, 0, 0, 0)
     * (2, 0, 0, 0)
     * (1, 1, 0, 0)
     * (2, 1, 0, 0)
     * (2, 2, 0, 0)
     * (1, 1, 1, 0)
     *     ...
     * (2, 2, 2, 2)
     */
    parameter_indicies.fill(0);
    for (std::size_t n = 0; n < n_choose_k(number_of_elements + d - 1, d); ++n) {
      polynomial[polynomial_index++] = std::accumulate(parameter_indicies.cbegin(), parameter_indicies.cbegin() + d, T(1.0), [&parameter](const T product, const std::size_t parameter_index) {return product * parameter[parameter_index];});
      
      ++parameter_indicies[0];

      // Increments the next (more significant) digit by 1 if we overshoot the maximal value for a *numberOfElements*-adic number.
      // All less significant digits are set to the same value, as we are skipping numbers whose values are not monotonically decreasing from the less to the most significant digit.
      for (std::size_t k = 0; k < d - 1; ++k) {
        if (parameter_indicies[k] == number_of_elements) {
          std::fill_n(parameter_indicies.begin(), k + 1, ++parameter_indicies[k + 1]);
        } else {
          break;
        }
      }
    }
  }
  // Linear term
  std::copy(parameter.cbegin(), parameter.cend(), polynomial.begin() + polynomial_index);
  // Constant term
  polynomial[polynomial.size() - 1] = T(1.0);
  
  return polynomial;
}

//
// Tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("polynomial", "[algebra][polynomial]") {
  CHECK((mant::polynomial<double, 0, 1>({}) == std::array<double, 1>({1.0})));
  CHECK((mant::polynomial<double, 1, 0>({2.0}) == std::array<double, 1>({1.0})));
  CHECK((mant::polynomial<double, 2, 1>({-2.0, 3.0}) == std::array<double, 3>({-2.0, 3.0, 1.0})));
  CHECK((mant::polynomial<double, 2, 2>({0.0, 0.0}) == std::array<double, 6>({0.0, 0.0, 0.0, 0.0, 0.0, 1.0})));
  CHECK((mant::polynomial<double, 2, 3>({-2.0, 3.0}) == std::array<double, 10>({-8.0, 12.0, -18.0, 27.0, 4.0, -6.0, 9.0, -2.0, 3.0, 1.0})));
  CHECK((mant::polynomial<double, 10, 1000>({0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}).empty() == true));
}
#endif