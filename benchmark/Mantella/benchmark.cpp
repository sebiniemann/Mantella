// C++ standard library
#include <memory>
#include <iostream>
#include <fstream>

// Mantella
#include <mantella0>

static constexpr std::array<std::size_t, 15> dimensions = {2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 35, 40};
static constexpr std::size_t repetitions = 100;
static constexpr double acceptable_objective_value = 1e-5;
static constexpr double maximal_evaluations = std::numeric_limits<std::size_t>::max();
static constexpr std::chrono::nanoseconds maximal_duration = std::chrono::milliseconds(10);

/* +------------+------------+--------------+----------+-------------+
 * | Dimensions | Problem ID | optimiser ID | Duration | Successful? |
 * +------------+------------+--------------+----------+-------------+
 */
static std::array<std::tuple<std::size_t, std::size_t, std::size_t, std::chrono::nanoseconds, bool>, dimensions.size() * 5 * 5 * repetitions> results;
static std::size_t n = 0;

template <std::size_t N>
struct benchmark {
  static constexpr void loop() {
    static_assert(0 < N && N <= dimensions.size(), "");
    constexpr auto dimensions = std::get<::dimensions.size() - N>(::dimensions);
    
    const std::array<std::unique_ptr<mant::problem<double, dimensions>>, 5> problems = {
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::ackley_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::rastrigin_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::rosenbrock_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::sphere_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::sum_of_different_powers<double, dimensions>)
    };

    const std::array<std::unique_ptr<mant::optimiser<double, dimensions>>, 5> optimisers = {
      std::unique_ptr<mant::optimiser<double, dimensions>>(new mant::hill_climbing<double, dimensions>),
      std::unique_ptr<mant::optimiser<double, dimensions>>(new mant::hooke_jeeves_algorithm<double, dimensions>),
      std::unique_ptr<mant::optimiser<double, dimensions>>(new mant::nelder_mead_method<double, dimensions>),
      std::unique_ptr<mant::optimiser<double, dimensions>>(new mant::particle_swarm_optimisation<double, dimensions>),
      std::unique_ptr<mant::optimiser<double, dimensions>>(new mant::random_search<double, dimensions>)
    };
    
    for (std::size_t k = 0; k < problems.size(); ++k) {
      const auto& problem = *problems.at(k);
      
      for (std::size_t l = 0; l < optimisers.size(); ++l) {
        auto& optimiser = *optimisers.at(l);
        optimiser.acceptable_objective_value = acceptable_objective_value;
        optimiser.maximal_evaluations = maximal_evaluations;
        optimiser.maximal_duration = maximal_duration;
        
        std::vector<std::array<double, dimensions>> initial_parameters;
        switch(l) {
          case 2: {
            initial_parameters.resize(dimensions + 1);
          } break;
          case 3: {
            initial_parameters.resize(10 * dimensions);
          } break;
          default: {
            initial_parameters.resize(1);
          } break;
        }
        
        for (auto& parameter : initial_parameters) {
          std::generate(
            parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
            std::bind(
              std::uniform_real_distribution<double>(0.0, 1.0),
              std::ref(mant::random_number_generator())));
        }
        
        for (std::size_t m = 0; m < repetitions; ++m) {
          const auto&& result = mant::optimise(problem, optimiser, initial_parameters);
          results.at(n++) = std::make_tuple(dimensions, k, l, result.duration, result.objective_value <= acceptable_objective_value);
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

  std::ofstream output;
  output.open("benchmark.mat");
  for (auto&& result : results) {
    output << std::get<0>(result) << "  " << std::get<1>(result) << "  " << std::get<2>(result) << "  " << std::get<3>(result).count() << "  " << std::get<4>(result) << "\n";
  }
  output.close();
  
  return 0;
}
