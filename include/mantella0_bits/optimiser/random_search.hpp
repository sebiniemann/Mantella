/**

*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2>
struct random_search : optimiser<T1, N, T2> {
  random_search() noexcept;
};

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
random_search<T1, N, T2>::random_search() noexcept 
    : optimiser<T1, N, T2>() {
  this->optimisation_function = [this](const T2<T1, N>& problem, const std::vector<std::array<T1, N>>& initial_parameters) {
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T1, N> result;
    
    for (const auto& parameter : initial_parameters) {
      const auto objective_value = problem.objective_function(parameter);
      ++result.number_of_evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
        
        if (result.best_objective_value <= this->acceptable_objective_value) {
          return result;
        }
      }
      
      if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
        return result;
      } else if (result.duration >= this->maximal_duration) {
        return result;
      }
    }
    
    while (result.duration < this->maximal_duration && result.number_of_evaluations < this->maximal_number_of_evaluations && result.best_objective_value > this->acceptable_objective_value) {
      std::array<T1, N> parameter;
      std::generate(
        parameter.begin(), std::next(parameter.begin(), this->active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<T1>(0.0, 1.0),
          std::ref(random_number_generator())));
          
      const auto objective_value = problem.objective_function(parameter);
      ++result.number_of_evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
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
TEST_CASE("random_search", "[random_search]") {
  const mant::random_search<double, 3, mant::problem> optimiser;

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
    });
  
  std::cout << "Random search" << std::endl;
  for (auto&& result : results) {
    std::cout << "best_parameter: [ ";
    for (auto&& element : result.best_parameter) {
      std::cout << element << " ";
    }
    std::cout << "], best_objective_value: " << result.best_objective_value
              << ", number_of_evaluations: " << result.number_of_evaluations
              << ", duration: " << result.duration.count() << "ns" << std::endl;
  }
}
#endif
