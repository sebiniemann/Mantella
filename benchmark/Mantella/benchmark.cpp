// C++ standard library
#include <iostream>

// Mantella
#include <mantella0>

int main() {  
  const std::array<std::unique_ptr<mant::problem<double, 3>>, 5> problems = {
    std::unique_ptr<mant::problem<double, 3>>(new mant::ackley_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::rastrigin_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::rosenbrock_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::sphere_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::sum_of_different_powers_function<double, 3>)
  };
  
  const std::array<std::unique_ptr<mant::optimiser<double, 3, mant::problem>>, 5> optimisers = {
    std::unique_ptr<mant::optimiser<double, 3, mant::problem>>(new mant::hill_climbing<double, 3, mant::problem>),
    std::unique_ptr<mant::optimiser<double, 3, mant::problem>>(new mant::hooke_jeeves_algorithm<double, 3, mant::problem>),
    std::unique_ptr<mant::optimiser<double, 3, mant::problem>>(new mant::particle_swarm_optimisation<double, 3, mant::problem>),
    std::unique_ptr<mant::optimiser<double, 3, mant::problem>>(new mant::random_search<double, 3, mant::problem>)
  };
  
  // TODO Explain the statistic reason behind this magic looking number (effect power, test power, ...)
  constexpr std::size_t number_of_repetitions = 2600;
  std::array<std::array<std::array<mant::optimiser_result, 2 * problems.size()>, number_of_repetitions>, optimisers.size()> benchmark;
  for (std::size_t n = 0; n < results.size(); ++n) {
    const auto& optimiser = optimisers.at(n);
    auto& repetitions = benchmark.at(n);
    
    for (std::size_t k = 0; k < repetitions.size(); ++k) {
      auto& results = repetitions.at(k);
      
      // Why 10 parameters, should this be the same number for each algorithm?
      std::vector<std::array<double, 3>> parameters(10);
      for (auto& parameter : parameters) {
        std::generate(
          parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
          std::bind(
            std::uniform_real_distribution<double>(0.0, 1.0),
            std::ref(random_number_generator())));
      }
          
      for (std::size_t l = 0; l < results.size(); ++l) {
        // Why 1e-5? Should this be the same value for each problem?
        optimiser.acceptable_objective_value(1e-5);
        results.at(2 * l) = optimiser.optimisation_function(problems.at(l), parameters);
        optimiser.acceptable_objective_value(-std::numeric_limits<double>::infinity());
        // Why 1000? Should this be the same number for each problem?
        optimiser.maximal_number_of_evaluations(1000);
        results.at(2 * l + 1) = optimiser.optimisation_function(problems.at(l), parameters);
      }
    }
  }

  // Save everything (hdf5?, cube?) 
  // Analyse everything using Matlab or Octave
  
  return 0;
}
