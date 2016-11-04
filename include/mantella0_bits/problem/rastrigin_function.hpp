/**
Rastrigin function
------------------

.. cpp:class:: rastrigin_function : public problem

  .. versionadded:: 1.0.0 

  The Rastrigin function is a common *toy* problem with a very small computational cost, used for testing and benchmarking algorithms. It is named after L. A. Rastrigin and was first published 1974 in *Systems of Extremal Control* as 2-dimensional problem and 1991 generalised for n dimensions by H. Mühlenbein, D. Schomisch and J. Born in *The Parallel Genetic Algorithm as Function Optimizer. Parallel Computing*.
  
  Its analytic form can be denotes as
  
  .. math::
   
    f(\text{parameter}) = 10 \cdot N + \sum_{i = 1}^{N} \bigg( \text{parameter}_i^2 - 10 \cdot \cos\big(2\pi \cdot \text{parameter}_i\big)\bigg)
    
  The problem's default search space is bounded to ``[-5.12, 5.12]``, with optimal parameter ``(0, 0, ..., 0)`` and optimal function value ``0``.

  .. code-block:: image
    :name: rastrigin_function.png
    
    #include <mantella0>
    #include <fstream> // Used for std::ofstream
    
    int main() {
      mant::rastrigin_function<double, 2> problem;
      
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
    [X, Y] = meshgrid(linspace(-5.12, 5.12, size(data, 1)), linspace(-5.12, 5.12, size(data, 2)));
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
        
        ``unsigned``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<unsigned>::max()]``.
      
  .. list-table:: Member functions
    :widths: 27 73
    
    * - rastrigin_function()
    
        Constructor
      - Initialises all member variables to their default value.
      
        This will especially set `objective_function` and fill the lower and bounds.
      
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
        [](const T sum, const T element) {
          return 
            sum +
            std::pow(element, T(2.0)) - 
            T(10.0) * std::cos(T(2.0) * std::acos(T(-1.0)) * element);
        });
  };
  
  this->lower_bounds.fill(T(-5.12));
  this->upper_bounds.fill(T(5.12));
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
