/**

*/
template <typename T, unsigned N>
struct random_search : optimiser<T, N> {
  random_search() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
random_search<T, N>::random_search() noexcept 
    : optimiser<T, N>() {
  this->optimisation_function = [this](const mant::problem<T, N>& problem, const std::vector<std::array<T, N>>& initial_parameters) {
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T, N> result;
    
    for (const auto& parameter : initial_parameters) {
      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
        
        if (result.best_objective_value <= this->acceptable_objective_value) {
          return result;
        }
      }
      
      if (result.evaluations >= this->maximal_evaluations) {
        return result;
      } else if (result.duration >= this->maximal_duration) {
        return result;
      }
    }
    
    while (result.duration < this->maximal_duration && result.evaluations < this->maximal_evaluations && result.best_objective_value > this->acceptable_objective_value) {
      std::array<T, N> parameter;
      std::generate(
        parameter.begin(), std::next(parameter.begin(), this->active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<T>(0.0, 1.0),
          std::ref(random_number_generator())));
          
      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
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
  constexpr unsigned dimensions = 3;
  mant::random_search<double, dimensions> optimiser;
  
  SECTION("Boundary handling") {
    mant::problem<double, dimensions> problem;
    problem.objective_function = [](const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), 0.0);
    };
    
    const auto&& result = optimiser.optimisation_function(problem, {{0.0, 0.0, 0.0}});
    CHECK(std::all_of(
      result.best_parameter.cbegin(), std::next(result.best_parameter.cbegin(), optimiser.active_dimensions.size()),
      [](const auto element) { 
        return element >= 0.0;
      }
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
