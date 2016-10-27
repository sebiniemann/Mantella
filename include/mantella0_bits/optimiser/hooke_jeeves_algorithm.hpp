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
    
    T1 stepsize = initial_stepsize;
    
    
    while (result.duration < this->maximal_duration && result.number_of_evaluations < this->maximal_number_of_evaluations && result.best_objective_value > this->acceptable_objective_value) {
      bool is_improving = false;

      for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
        auto parameter = result.best_parameter;
        parameter.at(n) += stepsize;
        
        std::transform(
          parameter.cbegin(), parameter.cend(),
          parameter.begin(),
          [](const auto element) {
            if (element < T1(0.0)) {
              return T1(0.0);
            } else if(element > T1(1.0)) {
              return T1(1.0);
            }
          
            return element;
          });
          
        auto objective_value = problem.objective_function(parameter);
        ++result.number_of_evaluations;
        result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
        
        if (objective_value < result.best_objective_value) {
          result.best_parameter = parameter;
          result.best_objective_value = objective_value;
          
          if (result.best_objective_value <= this->acceptable_objective_value) {
            return result;
          }
          
          is_improving = true;
        }
        
        if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
          return result;
        } else if (result.duration >= this->maximal_duration) {
          return result;
        }
        
        parameter.at(n) -= T1(2.0) * stepsize;
        
        std::transform(
          parameter.cbegin(), parameter.cend(),
          parameter.begin(),
          [](const auto element) {
            if (element < T1(0.0)) {
              return T1(0.0);
            } else if(element > T1(1.0)) {
              return T1(1.0);
            }
          
            return element;
          });
        
        objective_value = problem.objective_function(parameter);
        ++result.number_of_evaluations;
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
  constexpr std::size_t number_of_dimensions = 3;
  const mant::hooke_jeeves_algorithm<double, number_of_dimensions, mant::problem> optimiser;
  
  SECTION("Default configuration") {
    CHECK(optimiser.initial_stepsize == 1.0);
    CHECK(optimiser.stepsize_decrease == 2.0);
  }
  
  SECTION("Benchmarking") {
    const std::array<std::unique_ptr<mant::problem<double, number_of_dimensions>>, 5> problems = {
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::ackley_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rastrigin_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rosenbrock_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sphere_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sum_of_different_powers_function<double, number_of_dimensions>)
    };
    
    std::array<mant::optimise_result<double, number_of_dimensions>, problems.size()> results;
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
                << ", number_of_evaluations: " << result.number_of_evaluations
                << ", duration: " << result.duration.count() << "ns" << std::endl;
    }
  }
  
  SECTION("Boundary handling") {
    mant::sphere_function<double, number_of_dimensions> problem;
    problem.lower_bounds.fill(0.5);
    
    const auto&& result = optimiser.optimisation_function(problem, {problem.lower_bounds});
    CHECK(std::all_of(
      result.best_parameter.cbegin(), std::next(result.best_parameter.cbegin(), optimiser.active_dimensions.size()),
      [](const auto elem){ 
        return elem >= 0.5;
      }
    ) == true);
  }
}
#endif
