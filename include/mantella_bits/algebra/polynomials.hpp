#pragma once

/**
polynomial
==========

.. cpp:function:: template <T, N1, N2, N3> constexpr polynomial(parameters)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T** - The parameters and polynomials value type **(must be a floating point type)**.
    * **N1** (``std::size_t``) - The number of indeterminates.
    * **N2** (``std::size_t``) - A largest degree.
    * **N3** (``std::size_t``) - The number of parameters.

  **Function parameters**
   
    * **parameters** (``std::array<T, N3*N1>``) - The parameters.

  **Returns**
   
    ``std::array<T, N3*mant::`` :cpp:any:`number_of_coefficients` ``<N1,N2>()>`` - The polynomials.
  
  Generates a polynomial with ``N1`` indeterminates and largest degree ``N2`` for each of the ``N3`` parameters (all with length ``N1``) in ``parameters``. Each polynomial is written as a tuple with one polynomial term per element.
  
  The indeterminates :math:`(x_1, \ldots, x_\text{N1})` of the ``n``-th polynomial are substituted by ``(parameters[n], ..., parameters[n + N1-1])`` and all coefficients are replaced by ``1.0``.
  
  The result is filled by one polynomial after another, going from the first parameter's polynomial to the last parameter's. The terms within each tuple are going from the terms with degree ``N2`` (lexicographic ordering along the indeterminates indices) to degree 0. The last value (degree 0) will always be ``1.0``.
  
  In consequence, for ``N1 = 2``, ``N2 = 3`` and ``N3 = 2``, the result will be ordered as
  
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
  
  If either ``N1`` or ``N2`` is ``0``, the return value will be ``{1.0, ..., 1.0}`` (having the length ``N3``). If ``N3`` is ``0``, an empty array will be returned.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      constexpr std::size_t number_of_indeterminates = 2;
      constexpr std::size_t largest_degree = 2;
      constexpr std::size_t number_of_parameters = 3;
      
      constexpr std::array<double, number_of_indeterminates> parameters = {0.2, -9.0, 0.1, 3.0};
      constexpr auto polynomials = mant::polynomial<number_of_indeterminates, largest_degree, number_of_parameters>(parameters);
      
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
template <typename T, std::size_t N1, std::size_t N2, std::size_t N3>
constexpr std::array<T, N3*number_of_coefficients<N1, N2>()> polynomial(
    const std::array<T, N3*N1>& parameters);

/**
.. cpp:function:: template <T, N1, N2> constexpr polynomial(parameter)

  .. versionadded:: 1.x
  
  Short-cut for ``polynomial<T, N1, N2, 1>(parameter)``
*/
template <typename T, std::size_t N1, std::size_t N2>
constexpr std::array<T, number_of_coefficients<N1, N2>()> polynomial(
    const std::array<T, N1>& parameters);

//
// Implementation
//

template <typename T, std::size_t N1, std::size_t N2, std::size_t N3>
constexpr std::array<T, N3*number_of_coefficients<N1, N2>()> polynomial(
    const std::array<T, N3*N1>& parameters) {
  std::array<T, N3*number_of_coefficients<N1, N2>()> polynomials;
  
  if (N1 == 0 || N2 == 0) {
    polynomials.fill(1.0);
    return polynomials;
  }

  std::size_t offset = 0;
  for (std::size_t n = 0; n < N3; ++n) {
    // Generates all terms for degree > 1
    for (std::size_t degree = N2; degree >= 2; --degree) {
      const auto& combinations = multicombinations(parameter.n_elem, degree);
      for (std::size_t n = 0; n < indeterminates.n_cols; ++n) {
        polynomial(n) = arma::prod(parameter.elem(indeterminates.col(n)));
      }
    }
    // Linear term
    polynomial.subvec(polynomial.n_elem - parameter.n_elem - 1, polynomial.n_elem - 2) = parameter;
    // Constant term
    polynomial.tail(1) = 1;
    
    offset += number_of_coefficients<N1, N2>();
  }

  return polynomial;
}

template <typename T, std::size_t N1, std::size_t N2>
constexpr std::array<T, number_of_coefficients<N1, N2>()> polynomial(
    const std::array<T, N1>& parameter) {
  return polynomial<T, N1, N2, 1>(parameter);
}

//
// Tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("polynomial", "[polynomial]") {
  CHECK(arma::approx_equal(mant::polynomial({}, 1), arma::vec({1.0}), "absdiff", ::mant::machinePrecision) == true);
  CHECK(arma::approx_equal(mant::polynomial({2.0}, 0), arma::vec({1.0}), "absdiff", ::mant::machinePrecision) == true);
  CHECK(arma::approx_equal(mant::polynomial({0.0, 0.0}, 2), arma::vec({0.0, 0.0, 0.0, 0.0, 0.0, 1.0}), "absdiff", ::mant::machinePrecision) == true);
  CHECK(arma::approx_equal(mant::polynomial({-2.0, 3.0}, 3), arma::vec({-8.0, 12.0, -18.0, 27.0, 4.0, -6.0, 9.0, -2.0, 3.0, 1.0}), "absdiff", ::mant::machinePrecision) == true);
}
#endif