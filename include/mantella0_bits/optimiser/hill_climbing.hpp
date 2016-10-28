/**

*/
template <typename T1, unsigned N, template <class, unsigned> class T2>
struct hill_climbing : optimiser<T1, N, T2> {
  T1 minimal_stepsize;
  T1 maximal_stepsize;
  
  hill_climbing() noexcept;
};

//
// Implementation
//

template <typename T1, unsigned N, template <class, unsigned> class T2>
hill_climbing<T1, N, T2>::hill_climbing() noexcept 
    : optimiser<T1, N, T2>(),
      minimal_stepsize(T1(0.0)),
      maximal_stepsize(T1(0.1)) {
  this->optimisation_function = [this](const T2<T1, N>& problem, const std::vector<std::array<T1, N>>& initial_parameters) {
    assert(T1(0.0) <= minimal_stepsize && minimal_stepsize <= maximal_stepsize && minimal_stepsize <= T1(1.0));
    assert(maximal_stepsize > T1(0.0));
    
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T1, N> result;
    
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
      auto&& parameter = random_neighbour(result.best_parameter, minimal_stepsize, maximal_stepsize, this->active_dimensions.size());
      
      std::transform(
        parameter.cbegin(), parameter.cend(),
        parameter.begin(),
        [](const auto element) {
          return std::fmin(std::fmax(element, T1(0.0)), T1(1.0));
        });
        
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
TEST_CASE("hill_climbing", "[hill_climbing]") {
  constexpr unsigned dimensions = 3;
  mant::hill_climbing<double, dimensions, mant::problem> optimiser;
  
  SECTION("Default configuration") {
    CHECK(optimiser.minimal_stepsize == 0.0);
    CHECK(optimiser.maximal_stepsize == 0.1);
  }
  
  SECTION("Benchmarking") {
    const std::array<std::unique_ptr<mant::problem<double, dimensions>>, 5> problems = {
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::ackley_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::rastrigin_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::rosenbrock_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::sphere_function<double, dimensions>),
      std::unique_ptr<mant::problem<double, dimensions>>(new mant::sum_of_different_powers_function<double, dimensions>)
    };
    
    std::vector<std::array<double, dimensions>> initial_parameters(1);
    for (auto& parameter : initial_parameters) {
      std::generate(
        parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<double>(0.0, 1.0),
          std::ref(random_number_generator())));
    }
    
    std::array<mant::optimise_result<double, dimensions>, problems.size()> results;
    optimiser.maximal_duration = std::chrono::seconds(10);
    optimiser.maximal_evaluations = 10'000'000;
    std::transform(
      problems.cbegin(), problems.cend(),
      results.begin(),
      [&optimiser, &initial_parameters](auto&& problem) {
        return optimiser.optimisation_function(*problem, initial_parameters);
      }
    );
    
    std::cout << "Hill climbing" << std::endl;
    for (auto&& result : results) {
      std::cout << "best_parameter: [ ";
      std::copy(result.best_parameter.cbegin(), result.best_parameter.cend(), std::ostream_iterator<double>(std::cout, " "));
      std::cout << "], best_objective_value: " << result.best_objective_value
                << ", evaluations: " << result.evaluations
                << ", duration: " << result.duration.count() << "ns" << std::endl;
    }
  }
  
  SECTION("Boundary handling") {
    mant::problem<double, dimensions> problem;
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





