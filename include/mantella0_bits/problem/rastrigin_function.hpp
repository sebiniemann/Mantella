/**
Rastrigin function
==================

.. cpp:class:: rastrigin_function : public problem

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
    
    * - rastrigin_function
    
        (Constructor)
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
*/
template <typename T, unsigned N>
struct rastrigin_function : problem<T, N> {
  rastrigin_function() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
rastrigin_function<T, N>::rastrigin_function() noexcept 
    : problem<T, N>() {
  /* Original, 2-dimensional: @see L. A. Rastrigin (1974). Systems of Extremal Control.
   * Generalised, n-dimensional: @see H. Mühlenbein, D. Schomisch and J. Born (1991). The Parallel Genetic Algorithm as 
   * Function Optimizer. Parallel Computing, 17(6-7), pp. 619–632.
   *
   *            n   /                                                  \
   * 10 * N +  sum  | parameter(i)^2 - 10 * cos(2 * pi * parameter(i)) |
   *          i = 1 \                                                  /
   */
  this->objective_function = [](const auto& parameter) {
    return 
      T(10.0) *
      static_cast<T>(N) +
      std::accumulate(
        parameter.cbegin(), parameter.cend(),
        T(0.0),
        [](const T sum, T element) {
          element = element * T(10.24) - T(5.12);
          
          return 
            sum +
            std::pow(element, T(2.0)) - 
            T(10.0) * std::cos(T(2.0) * std::acos(T(-1.0)) * element);
        });
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rastrigin_function", "[problem][rastrigin_function]") {
  const mant::rastrigin_function<double, 3> rastrigin_function;
  
  CHECK(rastrigin_function.objective_function({1.0, -2.0, 3.0}) == Approx(1375.8250536133));
}
#endif
