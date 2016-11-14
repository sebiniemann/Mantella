/**
Sum of different powers
-----------------------

.. cpp:class:: sum_of_different_powers : public problem

  .. versionadded:: 1.0.0 

  The sum of different powers function is a common *toy* problem with a very small computational cost, used for testing and benchmarking algorithms.
  
  Its original, analytic form can be denotes as
  
  .. math::
  
    f(\text{parameter}) = \sum_{i = 1}^{n} \text{parameter}_i^{i + 1}
    
  The problem's default search space is bounded to ``[-1, 1]``, with optimal parameter ``(0, 0, ..., 0)`` and optimal function value ``0``.

  .. code-block:: image
    :name: sum_of_different_powers.png
    
    #include <mantella0>
    #include <fstream> // Used for std::ofstream
    
    int main() {
      mant::sum_of_different_powers<double, 2> problem;
      
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
    [X, Y] = meshgrid(linspace(-1, 1, size(data, 1)), linspace(-1, 1, size(data, 2)));
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

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``std::size_t``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.
      
  .. list-table:: Member functions

    
    * - sum_of_different_powers()
    
        Constructor
      - Initialises all member variables to their default value.
      
        This will especially set `objective_function` and fill the lower and bounds.
      
        Will never throw an exception.
*/
template <typename T,  std::size_t N>
struct sum_of_different_powers : problem<T, N> {
  sum_of_different_powers() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
sum_of_different_powers<T, N>::sum_of_different_powers() noexcept 
    : problem<T, N>() {
  /*   n   /                           \
   *  sum  | abs(parameter(i))^(i + 1) |
   * i = 1 \                           /
   */
  this->objective_function = [](const auto& parameter) {
    T sum = T(0.0);
    for (std::size_t n = 0; n < N; ++n) {
      sum += std::pow(std::fabs(parameter.at(n)), static_cast<T>(n + 2));
    }
    
    return sum;
  };
  
  this->lower_bounds.fill(T(-1.0));
  this->upper_bounds.fill(T(1.0));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sum_of_different_powers", "[problem][sum_of_different_powers]") {
  const mant::sum_of_different_powers<double, 3> sum_of_different_powers;
  
  CHECK(sum_of_different_powers.objective_function({1.0, -2.0, 3.0}) == Approx(90.0));
}
#endif
