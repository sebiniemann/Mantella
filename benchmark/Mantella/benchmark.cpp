// C++ standard library
#include <iostream>
#include <memory>

// Mantella
#include <mantella0>

int main() {  
  constexpr std::size_t number_of_dimensions = 5;
  const std::array<std::unique_ptr<mant::problem<double, number_of_dimensions>>, 5> problems = {
    std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::ackley_function<double, number_of_dimensions>),
    std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rastrigin_function<double, number_of_dimensions>),
    std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rosenbrock_function<double, number_of_dimensions>),
    std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sphere_function<double, number_of_dimensions>),
    std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sum_of_different_powers_function<double, number_of_dimensions>)
  };
  
  const std::array<std::unique_ptr<mant::optimiser<double, number_of_dimensions, mant::problem>>, 5> optimisers = {
    std::unique_ptr<mant::optimiser<double, number_of_dimensions, mant::problem>>(new mant::hill_climbing<double, number_of_dimensions, mant::problem>),
    std::unique_ptr<mant::optimiser<double, number_of_dimensions, mant::problem>>(new mant::hooke_jeeves_algorithm<double, number_of_dimensions, mant::problem>),
    std::unique_ptr<mant::optimiser<double, number_of_dimensions, mant::problem>>(new mant::nelder_mead_method<double, number_of_dimensions, mant::problem>),
    std::unique_ptr<mant::optimiser<double, number_of_dimensions, mant::problem>>(new mant::particle_swarm_optimisation<double, number_of_dimensions, mant::problem>),
    std::unique_ptr<mant::optimiser<double, number_of_dimensions, mant::problem>>(new mant::random_search<double, number_of_dimensions, mant::problem>)
  };
  
  // TODO Explain the statistic reason behind this magic looking number (effect power, test power, ...)
  constexpr std::size_t number_of_repetitions = 2600;
  std::array<std::array<std::array<mant::optimise_result<double, number_of_dimensions>, 2 * problems.size()>, number_of_repetitions>, optimisers.size()> benchmark;
  for (std::size_t n = 0; n < benchmark.size(); ++n) {
    std::cout << "n: " << n << " of " << benchmark.size() << std::endl;
    const auto& optimiser = optimisers.at(n);
    auto& repetitions = benchmark.at(n);
    
    for (std::size_t k = 0; k < repetitions.size(); ++k) {
      auto& results = repetitions.at(k);
      
      // TODO Why 10 parameters, should this be the same number for each algorithm?
      std::vector<std::array<double, number_of_dimensions>> parameters(number_of_dimensions + 1);
      for (auto& parameter : parameters) {
        std::generate(
          parameter.begin(), std::next(parameter.begin(), optimiser->active_dimensions.size()),
          std::bind(
            std::uniform_real_distribution<double>(0.0, 1.0),
            std::ref(mant::random_number_generator())));
      }
          
      for (std::size_t l = 0; l < results.size() / 2; ++l) {
        // TODO Why 1e-5? Should this be the same value for each problem?
        optimiser->acceptable_objective_value = 1e-5;
        results.at(2 * l) = optimiser->optimisation_function(*problems.at(l), parameters);
        optimiser->acceptable_objective_value = -std::numeric_limits<double>::infinity();
        // TODO Why 1000? Should this be the same number for each problem?
        optimiser->maximal_number_of_evaluations = 1000;
        results.at(2 * l + 1) = optimiser->optimisation_function(*problems.at(l), parameters);
      }
    }
  }

  // TODO Save everything (hdf5?, cube?) 
  // TODO Analyse everything using Matlab or Octave
  
  return 0;
}
