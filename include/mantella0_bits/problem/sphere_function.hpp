/**
Sphere function
===============

.. cpp:class:: sphere_function : public problem

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
    
    * - sphere_function
    
        (Constructor)
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
*/
template <typename T, unsigned N>
struct sphere_function : problem<T, N> {
  sphere_function() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
sphere_function<T, N>::sphere_function() noexcept 
    : problem<T, N>() {
  /*   n   /                \
   *  sum  | parameter(i)^2 |
   * i = 1 \                /
   */
  this->objective_function = [](const auto& parameter) {
    return 
      std::accumulate(
        parameter.cbegin(), parameter.cend(),
        T(0.0),
        [](const double length, const double element) {
          return length + std::pow(element * T(10.24) - T(5.12), T(2.0));
        });
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sphere_function", "[problem][sphere_function]") {
  const mant::sphere_function<double, 3> sphere_function;
  
  CHECK(sphere_function.objective_function({1.0, -2.0, 3.0}) == Approx(1336.9344));
}
#endif
