/**
Sphere function
---------------

.. cpp:class:: sphere_function : public problem

  .. versionadded:: 1.0.0

  The sphere function is a common *toy* problem with a very small computational cost, used for testing and benchmarking algorithms.

  Its analytic form can be denotes as

  .. math::

    f(\text{parameter}) = \sum_{i = 1}^{n} \text{parameter}_i^2

  The problem's default search space is bounded to ``[-5.12, 5.12]``, with optimal parameter ``(0, 0, ..., 0)`` and optimal function value ``0``.

  .. code-block:: image
    :name: sphere_function.png

    #include <mantella0>
    #include <fstream> // Used for std::ofstream

    int main() {
      mant::sphere_function<double, 2> problem;

      std::ofstream output;
      output.open("problem.mat");
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

    problem = dlmread('problem.mat');
    [X, Y] = meshgrid(linspace(-5.12, 5.12, size(problem, 1)), linspace(-5.12, 5.12, size(problem, 2)));
    surfc(X, Y, problem)
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

    * - sphere_function()

        Constructor
      - Initialises all member variables to their default value.

        This will especially set `objective_function` and fill the lower and bounds.

        Will never throw an exception.
*/
template <typename T, std::size_t N>
struct sphere_function : problem<T, N> {
  sphere_function() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
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
        [](const auto length, const auto element) {
          return length + std::pow(element, T(2.0));
        });
  };

  this->lower_bounds.fill(T(-5.12));
  this->upper_bounds.fill(T(5.12));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sphere_function", "[problem][sphere_function]") {
  const mant::sphere_function<double, 3> sphere_function;

  CHECK(sphere_function.objective_function({1.0, -2.0, 3.0}) == Approx(14.0));
}
#endif
