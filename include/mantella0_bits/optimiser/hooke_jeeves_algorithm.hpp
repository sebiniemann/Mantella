/**

*/
template <typename T, unsigned N>
struct hooke_jeeves_algorithm : optimiser<T, N> {
  T initial_stepsize;
  T stepsize_decrease;
  
  hooke_jeeves_algorithm() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
hooke_jeeves_algorithm<T, N>::hooke_jeeves_algorithm() noexcept 
    : optimiser<T, N>(),
      initial_stepsize(T(1.0)), 
      stepsize_decrease(T(2.0)) {
  this->optimisation_function = [this](const mant::problem<T, N>& problem, const std::vector<std::array<T, N>>& initial_parameters) {
    assert(stepsize_decrease > T(1.0));
    
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
    
    T stepsize = initial_stepsize;
    
    
    while (result.duration < this->maximal_duration && result.evaluations < this->maximal_evaluations && result.best_objective_value > this->acceptable_objective_value) {
      bool is_improving = false;

      for (unsigned n = 0; n < this->active_dimensions.size(); ++n) {
        auto parameter = result.best_parameter;
        parameter.at(n) += stepsize;
        
        std::transform(
          parameter.cbegin(), parameter.cend(),
          parameter.begin(),
          [](const auto element) {
            if (element < T(0.0)) {
              return T(0.0);
            } else if(element > T(1.0)) {
              return T(1.0);
            }
          
            return element;
          });
          
        auto objective_value = problem.objective_function(parameter);
        ++result.evaluations;
        result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
        
        if (objective_value < result.best_objective_value) {
          result.best_parameter = parameter;
          result.best_objective_value = objective_value;
          
          if (result.best_objective_value <= this->acceptable_objective_value) {
            return result;
          }
          
          is_improving = true;
        }
        
        if (result.evaluations >= this->maximal_evaluations) {
          return result;
        } else if (result.duration >= this->maximal_duration) {
          return result;
        }
        
        parameter.at(n) -= T(2.0) * stepsize;
        
        std::transform(
          parameter.cbegin(), parameter.cend(),
          parameter.begin(),
          [](const auto element) {
            return std::fmin(std::fmax(element, T(0.0)), T(1.0));
          });
        
        objective_value = problem.objective_function(parameter);
        ++result.evaluations;
        result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
        
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
  constexpr unsigned dimensions = 3;
  mant::hooke_jeeves_algorithm<double, dimensions> optimiser;
  
  SECTION("Default configuration") {
    CHECK(optimiser.initial_stepsize == 1.0);
    CHECK(optimiser.stepsize_decrease == 2.0);
  }
  
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
