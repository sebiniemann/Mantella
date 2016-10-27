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
  constexpr std::size_t number_of_dimensions = 3;
  const mant::random_search<double, number_of_dimensions, mant::problem> optimiser; 

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
    
    std::cout << "Random search" << std::endl;
    for (auto&& result : results) {
      std::cout << "best_parameter: [ ";
      std::copy(result.best_parameter.cbegin(), result.best_parameter.cend(), std::ostream_iterator<double>(std::cout, " "));
      std::cout << "], best_objective_value: " << result.best_objective_value
                << ", number_of_evaluations: " << result.number_of_evaluations
                << ", duration: " << result.duration.count() << "ns" << std::endl;
    }
  }
  
  SECTION("Boundary handling") {
    mant::problem<double, number_of_dimensions> problem;
    problem.objective_function = [](const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), 0.0);
    };
    
    const auto&& result = optimiser.optimisation_function(problem, {problem.lower_bounds});
    CHECK(std::all_of(
      result.best_parameter.cbegin(), std::next(result.best_parameter.cbegin(), optimiser.active_dimensions.size()),
      [](const auto element) { 
        return element >= 0.0;
      }
    ) == true);
  }
}
#endif
