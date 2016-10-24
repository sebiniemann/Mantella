/**

*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2>
struct hooke_jeeves_algorithm : optimiser<T1, N, T2> {
  T1 initial_stepsize;
  T1 stepsize_decrease;
  
  hooke_jeeves_algorithm() noexcept;
};

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
hooke_jeeves_algorithm<T1, N, T2>::hooke_jeeves_algorithm() noexcept 
    : optimiser<T1, N, T2>(),
      initial_stepsize(T1(1.0)), 
      stepsize_decrease(T1(2.0)) {
  this->optimisation_function = [this](const T2<T1, N>& problem, const std::vector<std::array<T1, N>>& initial_parameters) {
    assert(stepsize_decrease > T1(1.0));
    
    optimise_result<T1, N> result;
    
    for (const auto& parameter : initial_parameters) {
      const auto objective_value = problem.objective_function(parameter);
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
      }
    }
    
    T1 stepsize = initial_stepsize;
    
    
    for (; result.number_of_evaluations < this->maximal_number_of_evaluations && result.best_objective_value > this->acceptable_objective_value; ++result.number_of_evaluations) {
      bool is_improving = false;

      for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
        auto parameter = result.best_parameter;
        parameter.at(n) += stepsize;
        auto objective_value = problem.objective_function(parameter);
        
        if (objective_value < result.best_objective_value) {
          result.best_parameter = parameter;
          result.best_objective_value = objective_value;
          is_improving = true;
        }
        
        parameter.at(n) -= T1(2.0) * stepsize;
        objective_value = problem.objective_function(parameter);
        
        if (objective_value < result.best_objective_value) {
          result.best_parameter = parameter;
          result.best_objective_value = objective_value;
          is_improving = true;
        }
      }
      
      if (!is_improving) {
        stepsize /= stepsize_decrease;
      }
    }
  
    return result;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("hooke_jeeves_algorithm", "[hooke_jeeves_algorithm]") {
  const mant::hooke_jeeves_algorithm<double, 3, mant::problem> optimiser;
  
  CHECK(optimiser.initial_stepsize == 1.0);
  CHECK(optimiser.stepsize_decrease == 2.0);
  
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
  
  std::cout << "Hooke-Jeeves algorithm" << std::endl;
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
