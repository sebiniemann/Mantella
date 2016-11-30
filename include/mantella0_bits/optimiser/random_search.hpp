/**
Random search
-------------

.. cpp:class:: random_search : public optimiser

  .. versionadded:: 1.0.0

  Random search is one of the most simplest approaches as it uniformly draws a parameter within the problem's bounds, independently from previous evaluations.

  Note that despite it is simplistic nature, it computational cost is still higher compared to deterministic approaches like :cpp:any:`hooke_jeeves`.

  .. code-block:: animation
    :name: random_search.mp4

    #include <mantella0>
    #include <fstream> // Used for std::ofstream

    int main () {
      mant::ackley_function<double, 2> problem;
      mant::random_search<double, 2> optimiser;
      optimiser.maximal_evaluations = 10;

      // Since all *optimiser* run from start to end within a single call, we cannot peek into the sampled parameters.
      // However, we can still track them by observing the parameter given to the objective function instead.
      std::vector<std::array<double, 2>> samples;
      samples.reserve(optimiser.maximal_evaluations);
      mant::problem<double, 2> tracking_problem;
      tracking_problem.objective_function = [&problem, &optimiser, &samples](auto parameter) {
        const auto objective_value = problem.objective_function(parameter);

        // Remaps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*] and permutes the parameter to match
        // the active dimensions.
        for (std::size_t n = optimiser.active_dimensions.size(); n > 0; --n) {
          parameter.at(optimiser.active_dimensions.at(n - 1)) =
            problem.lower_bounds.at(n - 1) +
            parameter.at(n - 1) * (
              problem.upper_bounds.at(n - 1) - problem.lower_bounds.at(n - 1)
            );
        }
        samples.push_back(parameter);

        return objective_value;
      };

      mant::optimise(tracking_problem, optimiser);

      std::ofstream samples_output;
      samples_output.open("samples.mat");
      for (const auto& sample : samples) {
        for (const auto element : sample) {
          samples_output << element << "  ";
        }
        samples_output << "\n";
      }
      samples_output.close();

      std::ofstream problem_output;
      problem_output.open("problem.mat");
      for (double y = std::get<1>(problem.lower_bounds); y <= std::get<1>(problem.upper_bounds); y += (std::get<1>(problem.upper_bounds) - std::get<1>(problem.lower_bounds)) / 100.0) {
        for (double x = std::get<0>(problem.lower_bounds); x <= std::get<0>(problem.upper_bounds); x += (std::get<0>(problem.upper_bounds) - std::get<0>(problem.lower_bounds)) / 100.0) {
          problem_output << problem.objective_function({x, y}) << "  ";
        }
        problem_output << "\n";
      }
      problem_output.close();

      return 0;
    }

    :octave:

    problem = dlmread('problem.mat');
    [X, Y] = meshgrid(linspace(-32.768, 32.768, size(problem, 1)), linspace(-32.768, 32.768, size(problem, 2)));
    surfc(X, Y, problem)
    xlabel('x_1')
    ylabel('x_2')
    zlabel('f(x_1, x_2)')
    set(gcf, 'Color', [0.99, 0.99, 0.99]) % Matches to the documentation's background colour
    box off % Hide box outline
    axis tight % Fits the axis
    set(findall(gcf, 'Type', 'patch'), 'LineWidth', 2) % Thicker contours

    samples = dlmread('samples.mat');

    hold on;
    for n = 1:size(samples, 1)
      sample = samples(n, :);

      scatter3(sample(1), sample(2), 0)
      saveas(gcf, [name, int2str(n - 1), '.png'])
    end

  .. list-table:: Template parameters

    * - T

        Any floating point type
      - The value type of the parameter and objective value.
    * - N

        ``std::size_t``
      - The number of dimensions.

        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.

  .. list-table:: Member functions

    * - random_search

        Constructor
      - Initialises all member variables to their default value.

        This will especially set `optimisation_function`.

        Will never throw an exception.
*/
template <typename T, std::size_t N>
struct random_search : optimiser<T, N> {
  random_search() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
random_search<T, N>::random_search() noexcept
    : optimiser<T, N>() {
  this->optimisation_function = [this](const mant::problem<T, N>& problem, const std::vector<std::array<T, N>>& initial_parameters) {
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T, N> result;

    for (const auto& parameter : initial_parameters) {
      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);

      if (objective_value < result.objective_value) {
        result.parameter = parameter;
        result.objective_value = objective_value;

        if (result.objective_value <= this->acceptable_objective_value) {
          return result;
        }
      }

      if (result.evaluations >= this->maximal_evaluations) {
        return result;
      } else if (result.duration >= this->maximal_duration) {
        return result;
      }
    }

    while (result.duration < this->maximal_duration && result.evaluations < this->maximal_evaluations && result.objective_value > this->acceptable_objective_value) {
      std::array<T, N> parameter;
      std::generate(
        parameter.begin(), std::next(parameter.begin(), this->active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<T>(0.0, 1.0),
          std::ref(random_number_generator())));

      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);

      if (objective_value < result.objective_value) {
        result.parameter = parameter;
        result.objective_value = objective_value;
      }

    }

    return result;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_search", "[optimser][random_search]") {
  constexpr std::size_t dimensions = 3;
  mant::random_search<double, dimensions> optimiser;

  SECTION("Boundary handling") {
    mant::problem<double, dimensions> problem;
    problem.objective_function = [](const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), 0.0);
    };

    const auto&& result = optimiser.optimisation_function(problem, {{0.0, 0.0, 0.0}});
    CHECK(std::all_of(
      result.parameter.cbegin(), std::next(result.parameter.cbegin(), optimiser.active_dimensions.size()),
      std::bind(std::greater_equal<double>{}, std::placeholders::_1, 0.0)
    ) == true);
  }

  SECTION("Stopping criteria") {
    optimiser.maximal_duration = std::chrono::seconds(10);
    optimiser.maximal_evaluations = 1000;
    auto&& result = optimiser.optimisation_function(mant::sphere_function<double, dimensions>(), {{0.0, 0.0, 0.0}});
    CHECK(result.evaluations == 1000);
    optimiser.maximal_duration = std::chrono::microseconds(1);
    result = optimiser.optimisation_function(mant::sphere_function<double, dimensions>(), {{0.0, 0.0, 0.0}});
    CHECK(result.duration > std::chrono::microseconds(1));
    CHECK(result.duration < std::chrono::milliseconds(1));
  }
}
#endif
