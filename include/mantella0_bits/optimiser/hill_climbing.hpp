/**

*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2>
struct hill_climbing : optimiser<T1, N, T2> {
  T1 minimal_stepsize;
  T1 maximal_stepsize;
  
  hill_climbing() noexcept;
};

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
hill_climbing<T1, N, T2>::hill_climbing() noexcept 
    : optimiser<T1, N, T2>(),
      minimal_stepsize(T1(0.0)),
      maximal_stepsize(T1(0.1)) {
  this->optimisation_function = [this](const T2<T1, N>& problem, const std::vector<std::array<T1, N>>& initial_parameters) {
    assert(T1(0.0) <= minimal_stepsize && minimal_stepsize <= maximal_stepsize && minimal_stepsize <= T1(1.0));
    assert(maximal_stepsize > T1(0.0));
    
    optimise_result<T1, N> result;
    
    for (const auto& parameter : initial_parameters) {
      const auto objective_value = problem.objective_function(parameter);
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
      }
    }
    
    while (result.number_of_evaluations < this->maximal_number_of_evaluations && result.best_objective_value > this->acceptable_objective_value) {
      const auto& parameter = random_neighbour(result.best_parameter, minimal_stepsize, maximal_stepsize, this->active_dimensions.size());
      const auto objective_value = problem.objective_function(parameter);
       ++result.number_of_evaluations;
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
      }
    }
    
    return result;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("hill_climbing", "[hill_climbing]") {
  const mant::hill_climbing<double, 3, mant::problem> optimiser;
  
  CHECK(optimiser.minimal_stepsize == 0.0);
  CHECK(optimiser.maximal_stepsize == 0.1);
  
  const std::array<std::unique_ptr<mant::problem<double, 3>>, 5> problems = {
    std::unique_ptr<mant::problem<double, 3>>(new mant::ackley_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::rastrigin_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::rosenbrock_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::sphere_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::sum_of_different_powers_function<double, 3>)
  };
  
  std::array<mant::optimise_result<double, 3>, problems.size()> results;
  std::transform(
    problems.cbegin(), problems.cend(),
    results.begin(),
    [&optimiser](auto&& problem) {
      return optimiser.optimisation_function(*problem, {{5.0, 5.0, 5.0}});
    }
  );
  
  std::cout << "Hill climbing" << std::endl;
  for (auto&& result : results) {
    std::cout << "best_parameter: [ ";
    for (auto&& element : result.best_parameter) {
      std::cout << element << " ";
    }
    std::cout << "], best_objective_value: " << result.best_objective_value
              << ", number_of_evaluations: " << result.number_of_evaluations << std::endl;
  }
}
#endif





