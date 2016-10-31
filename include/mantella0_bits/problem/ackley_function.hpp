/**
Ackley function
===============

.. cpp:class:: ackley_function : public problem

  .. versionadded:: 1.0.0 

  .. list-table:: Template parameters
    :widths: 27 73

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``unsigned``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<unsigned>::max()]``.
      
  .. list-table:: Member functions
    :widths: 27 73
    
    * - ackley_function
    
        (Constructor)
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
*/
template <typename T, unsigned N>
struct ackley_function : problem<T, N> {
  ackley_function() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
ackley_function<T, N>::ackley_function() noexcept 
    : problem<T, N>() {
  /* @see David H. Ackley (1987). A Connectionist Machine for Genetic Hillclimbing. Kluwer Academic Publishers.
   *
   *      /                                     \       /   N                              \
   *      |      / -0.2 * norm(parameter)     \ |       |  sum  cos(2 * pi * parameter(i)) | 
   * 20 * | -exp | ---------------------- + 1 | | - exp | i = 1                            | + exp(1)
   *      |      \      sqrt(N)               / |       | -------------------------------- |
   *      \                                     /       \                 N                /
   */
  this->objective_function = [](auto parameter) {
    std::transform(
      parameter.cbegin(), parameter.cend(),
      parameter.begin(),
      [](const auto element) {
        return element * T(65.536) - T(32.768);
      });
    
    return
      T(20.0) * (
        -std::exp(
          T(-0.2) * std::sqrt(std::inner_product(
            parameter.cbegin(), parameter.cend(),
            parameter.cbegin(),
            T(0.0))
          ) /
          std::sqrt(N)
        ) + 
        T(1.0)
      ) -
      std::exp(
        std::accumulate(
          parameter.cbegin(), parameter.cend(),
          T(0.0),
          [](const auto sum, const auto element) {
            return 
              sum +
              std::cos(T(2.0) * std::acos(T(-1.0)) * element);
          }
        ) / 
        static_cast<T>(N)
      ) + 
      std::exp(T(1.0));
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("ackley_function", "[problem][ackley_function]") {
  const mant::ackley_function<double, 3> ackley_function;
  
  CHECK(ackley_function.objective_function({1.0, -2.0, 3.0}) == Approx(21.2341400796));
}
#endif
