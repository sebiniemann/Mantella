/**
Ackley function
<<<<<<< HEAD
===============

.. cpp:class:: template<T, N> ackley_function

  **Template parameters**
  
    - **T** - A floating point type
    - **N** - The (``unsigned``) number of dimensions 
      
  .. cpp:function:: ackley_function()
  
    Lorem ipsum dolor sit amet
=======
---------------

.. cpp:class:: ackley_function : public problem

  .. versionadded:: 1.0.0 

  The Ackley function is a common *toy* problem with a very small computational cost, used for testing and benchmarking algorithms. It is named after David H. Ackley and was first published 1987 in *A Connectionist Machine for Genetic Hillclimbing. Kluwer Academic Publishers*.
  
  Its analytic form can be denoted as
  
  .. math::
  
    f(\text{parameter}) = 20 \cdot \exp\bigg(\frac{-0.2 \cdot \text{norm}_2(\text{parameter})}{\sqrt{N}} + 1\bigg) - \exp\Bigg(\frac{1}{N} \cdot \sum_{i = 1}^{N} \cos\big(2\pi \cdot \text{parameter}_i\big) \Bigg) + \exp(1)

  The problem's default search space is bounded to ``[-32.768, 32.768]``, with optimal parameter ``(0, 0, ..., 0)`` and optimal function value ``0``.
    
  .. code-block:: image
    :name: ackley_function.png
    
    #include <mantella0>
    #include <fstream> // Used for std::ofstream
    
    int main() {
      mant::ackley_function<double, 2> problem;
      
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
    [X, Y] = meshgrid(linspace(-32.768, 32.768, size(data, 1)), linspace(-32.768, 32.768, size(data, 2)));
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
        
        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.
      
  .. list-table:: Member functions
    :widths: 27 73
    
    * - ackley_function()
    
        Constructor
      - Initialises all member variables to their default value.
      
        This will especially set `objective_function` and fill the lower and bounds.
      
        Will never throw an exception.
>>>>>>> master
*/
template <typename T, std::size_t N>
struct ackley_function : problem<T, N> {
  ackley_function() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
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
  
  this->lower_bounds.fill(T(-32.768));
  this->upper_bounds.fill(T(32.768));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("ackley_function", "[problem][ackley_function]") {
  const mant::ackley_function<double, 3> ackley_function;
  
  CHECK(ackley_function.objective_function({1.0, -2.0, 3.0}) == Approx(7.0164536083));
}
#endif
