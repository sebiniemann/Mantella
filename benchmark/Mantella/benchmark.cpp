// C++ standard library
#include <iostream>
#include <memory>

// Mantella
#include <mantella0>

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

constexpr std::size_t number_of_repetitions = 2600;

static std::array<std::array<std::array<mant::optimise_result<double, number_of_dimensions>, problems.size()>, number_of_repetitions>, optimisers.size()> benchmark;

template <unsigned N1, unsigned N2>
loop_over_dimensions {
  for (std::size_t n = 0; n < benchmark.size(); ++n) {
    std::cout << "n: " << n << " of " << benchmark.size() << std::endl;
    const auto& optimiser = optimisers.at(n);
    auto& repetitions = benchmark.at(n);
    
    for (std::size_t k = 0; k < repetitions.size(); ++k) {
      auto& results = repetitions.at(k);
      
      std::vector<std::array<double, number_of_dimensions>> initial_parameters(number_of_dimensions + 1);
      for (auto& parameter : initial_parameters) {
        std::generate(
          parameter.begin(), std::next(parameter.begin(), optimiser->active_dimensions.size()),
          std::bind(
            std::uniform_real_distribution<double>(0.0, 1.0),
            std::ref(mant::random_number_generator())));
      }

      std::transform(
        problems.cbegin(), problems.cend(),
        results.begin(),
        [&optimiser, &initial_parameters](const auto& problem) {
          return optimiser->optimisation_function(*problem, initial_parameters);
        });
    }
  }
  loop_over_dimensions<N1-N2>();
};
template <unsigned N>
loop_over_dimensions<0> {};

int main() {
  loop_over_dimensions<10, 1>
  

  
  // TODO Explain the statistic reason behind this magic looking number (effect power, test power, ...)
  
  


  // TODO Save everything (hdf5?, cube?) 
  // TODO Analyse everything using Matlab or Octave
  
  return 0;
}
