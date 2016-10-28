// C++ standard library
#include <memory>

// Mantella
#include <mantella0>

static constexpr std::array<unsigned, 15> dimensions = {2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 35, 40};
static constexpr std::size_t repetitions = 2600;

/* +------------+------------+--------------+------------+----------+
 * | Dimensions | Problem ID | Optimiser ID | Repetition | Duration |
 * +------------+------------+--------------+------------+----------+
 */
static std::array<std::tuple<unsigned, unsigned, unsigned, unsigned, std::chrono::nanoseconds>, dimensions.size() * 5 * 5 * repetitions> results;
static std::size_t n = 0;

template <unsigned N>
struct benchmark {
  static constexpr void loop() {
    static_assert(0 < N && N <= dimensions.size(), "");
    constexpr auto dimensions = std::get<::dimensions.size() - N>(::dimensions);
    
    const std::array<std::unique_ptr<mant::problem<double, dimensions>>, 5> problems = {
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::ackley_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::rastrigin_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::rosenbrock_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::sphere_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::sum_of_different_powers_function<double, dimensions>)
    };

    const std::array<std::unique_ptr<mant::optimiser<double, dimensions, mant::problem>>, 5> optimisers = {
      std::unique_ptr<mant::optimiser<double, dimensions, mant::problem>>(new mant::hill_climbing<double, dimensions, mant::problem>),
      std::unique_ptr<mant::optimiser<double, dimensions, mant::problem>>(new mant::hooke_jeeves_algorithm<double, dimensions, mant::problem>),
      std::unique_ptr<mant::optimiser<double, dimensions, mant::problem>>(new mant::nelder_mead_method<double, dimensions, mant::problem>),
      std::unique_ptr<mant::optimiser<double, dimensions, mant::problem>>(new mant::particle_swarm_optimisation<double, dimensions, mant::problem>),
      std::unique_ptr<mant::optimiser<double, dimensions, mant::problem>>(new mant::random_search<double, dimensions, mant::problem>)
    };
    
    for (unsigned k = 0; k < problems.size(); ++k) {
      const auto& problem = *problems.at(n);
      
      for (unsigned l = 0; l < optimisers.size(); ++l) {
        auto& optimiser = *optimisers.at(l);
        optimiser.acceptable_objective_value = 1e-5;
        optimiser.maximal_number_of_evaluations = std::numeric_limits<unsigned>::max();
        optimiser.maximal_duration = std::chrono::seconds(1);
        
        for (unsigned m = 0; m < repetitions; ++m) {
          results.at(n++) = std::make_tuple(N, k, l, m, mant::optimise(problem, optimiser).duration);
        }
      }
    }
    
    benchmark<N - 1>::loop();
  }
};

template <>
struct benchmark<0> {
  static constexpr void loop() {}
};

int main() {
  benchmark<dimensions.size()>::loop();

  return 0;
}