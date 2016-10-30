/**
Rosenbrock function
===================

.. cpp:class:: template<T, N> rosenbrock_function

  **Template parameters**
  
    - **T** (``floating point``)
    - **N** (``unsigned``)
      
  .. cpp:function:: rosenbrock_function()
  
    Lorem ipsum dolor sit amet
*/
template <typename T, unsigned N>
struct rosenbrock_function : problem<T, N> {
  rosenbrock_function() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
rosenbrock_function<T, N>::rosenbrock_function() noexcept 
    : problem<T, N>() {
  static_assert(N > 1, "");

  /* @see Howard H. Rosenbrock (1960). An automatic method for finding the greatest or least value of a function. The 
   * Computer Journal, 3(3), pp. 175–184.
   *
   *       /                                                                          \
   *   n   |       /                                             \^2                  |
   *  sum  | 100 * | parameter(i + 1) + 1 - (parameter(i) + 1)^2 |   + parameter(i)^2 |
   * i = 1 |       \                                             /                    |
   *       \                                                                          /
   */
  this->objective_function = [](auto parameter) {
    std::transform(
      parameter.cbegin(), parameter.cend(),
      parameter.begin(),
      [](const auto element) {
        return element * T(15.0) - T(5.0);
      });
      
    return 
      std::inner_product(
        parameter.cbegin(), std::prev(parameter.cend(), 1),
        std::next(parameter.cbegin(), 1),
        T(0.0),
        std::plus<T>(),
        [](const auto element, const auto other_element) {
          return 
            T(100.0) * std::pow(
              other_element + 1 - std::pow(element + 1, T(2.0)),
              T(2.0)
            ) + 
            std::pow(element, T(2.0));
        });
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rosenbrock_function", "[problem][rosenbrock_function]") {
  const mant::rosenbrock_function<double, 3> rosenbrock_function;
  
  CHECK(rosenbrock_function.objective_function({1.0, -2.0, 3.0}) == Approx(126726325.0));
}
#endif
