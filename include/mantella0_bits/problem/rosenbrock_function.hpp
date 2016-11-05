/**
Rosenbrock function
-------------------

.. cpp:class:: rosenbrock_function : public problem

  .. versionadded:: 1.0.0 

  The Ackley function is a common *toy* problem with a very small computational cost, used for testing and benchmarking algorithms. It is named after Howard H. Rosenbrock and was first published 1960 in *An automatic method for finding the greatest or least value of a function. The Computer Journal*.
  
  Its analytic form can be denotes as
  
  .. math::

    \sum_{i = 1}^{N} 100 \cdot \big(\text{parameter}_{i + 1} - \text{parameter}_i^2\big)^2 + \big(\text{parameter}_i - 1\big)^2
    
  The problem's default search space is bounded to ``[-10, 10]``, with optimal parameter ``(1, 1, ..., 1)`` and optimal function value ``0``.

  .. code-block:: image
    :name: rosenbrock_function.png
    
    #include <mantella0>
    #include <fstream> // Used for std::ofstream
    
    int main() {
      mant::rosenbrock_function<double, 2> problem;
      
      std::ofstream output;
      output.open("data.mat");
      for (double y = std::get<1>(problem.lower_bounds); y <= std::get<1>(problem.upper_bounds); y += (std::get<1>(problem.upper_bounds) - std::get<1>(problem.lower_bounds)) / 100.0) {
        for (double x = std::get<0>(problem.lower_bounds); x <= std::get<0>(problem.upper_bounds); x += (std::get<0>(problem.upper_bounds) - std::get<0>(problem.lower_bounds)) / 100.0) {
          output << problem.objective_function({x, y}) << "  ";
        }
        output << "\n";
      }
      output.close();
      
      return 0;
    }
  
    :octave:
    
    data = dlmread('data.mat');
    [X, Y] = meshgrid(linspace(-10, 10, size(data, 1)), linspace(-10, 10, size(data, 2)));
    surfc(X, Y, data)
    xlabel('x_1')
    ylabel('x_2')
    zlabel('f(x_1, x_2)')
    set(gcf, 'Color', [0.99, 0.99, 0.99]) % Matches to the documentation's background colour
    box off % Hide box outline
    axis tight % Fits the axis
    set(findall(gcf, 'Type', 'patch'), 'LineWidth', 2) % Thicker contours
    saveas(gcf, name)

  .. list-table:: Template parameters
    :widths: 27 73

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``std::size_t``
      - The number of dimensions.
        
        Must be within ``[2, std::numeric_limits<std::size_t>::max()]``.
      
  .. list-table:: Member functions
    :widths: 27 73
    
    * - rosenbrock_function()
    
        Constructor
      - Initialises all member variables to their default value.
      
        This will especially set `objective_function` and fill the lower and bounds.
      
        Will never throw an exception.
*/
template <typename T, std::size_t N>
struct rosenbrock_function : problem<T, N> {
  rosenbrock_function() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
rosenbrock_function<T, N>::rosenbrock_function() noexcept 
    : problem<T, N>() {
  static_assert(N > 1, "");

  /* @see Howard H. Rosenbrock (1960). An automatic method for finding the greatest or least value of a function. The 
   * Computer Journal, 3(3), pp. 175–184.
   *
   *       /                                                                          \
   *   n   |       /                                     \^2   /                  \^2 |
   *  sum  | 100 * | parameter(i + 1) - (parameter(i))^2 |   + | parameter(i) - 1 |   |
   * i = 1 |       \                                     /     \                  /   |
   *       \                                                                          /
   */
  this->objective_function = [](auto parameter) {
    return 
      std::inner_product(
        parameter.cbegin(), std::prev(parameter.cend(), 1),
        std::next(parameter.cbegin(), 1),
        T(0.0),
        std::plus<T>(),
        [](const auto element, const auto other_element) {
          return 
            T(100.0) * std::pow(
              other_element - std::pow(element, T(2.0)),
              T(2.0)
            ) + 
            std::pow(element - T(1.0), T(2.0));
        });
  };
  
  this->lower_bounds.fill(T(-10.0));
  this->upper_bounds.fill(T(10.0));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rosenbrock_function", "[problem][rosenbrock_function]") {
  const mant::rosenbrock_function<double, 3> rosenbrock_function;
  
  CHECK(rosenbrock_function.objective_function({1.0, -2.0, 3.0}) == Approx(1009.0));
}
#endif
