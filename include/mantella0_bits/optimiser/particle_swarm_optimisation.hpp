/**

*/
template <typename T1, unsigned N, template <class, unsigned> class T2>
struct particle_swarm_optimisation : optimiser<T1, N, T2> {
  T1 initial_velocity;
  T1 maximal_acceleration;
  T1 maximal_local_attraction;
  T1 maximal_global_attraction;
  
  particle_swarm_optimisation() noexcept;
};

//
// Implementation
//

template <typename T1, unsigned N, template <class, unsigned> class T2>
particle_swarm_optimisation<T1, N, T2>::particle_swarm_optimisation() noexcept 
    : optimiser<T1, N, T2>(),
      initial_velocity(T1(0.5)),
      maximal_acceleration(T1(1.0) / (T1(2.0) * std::log(T1(2.0)))),
      maximal_local_attraction(T1(0.5) + std::log(T1(2.0))),
      maximal_global_attraction(maximal_local_attraction) {
  this->optimisation_function = [this](const T2<T1, N>& problem, std::vector<std::array<T1, N>> parameters) {
    assert(initial_velocity >= T1(0.0));
    assert(maximal_acceleration >= T1(0.0));
    assert(maximal_local_attraction >= T1(0.0));
    assert(maximal_global_attraction >= T1(0.0));
    
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T1, N> result;
    
    std::vector<std::array<T1, N>> velocities(parameters.size());
    for (auto& velocity : velocities) {
      std::generate(
        velocity.begin(), std::next(velocity.begin(), this->active_dimensions.size()),
        std::bind(std::uniform_real_distribution<T1>(-initial_velocity, initial_velocity), std::ref(random_number_generator()))
      );
    }
    
    std::vector<std::array<T1, N>> local_best_parameters = parameters;
    std::vector<T1> local_best_objective_values(local_best_parameters.size());

    for (std::size_t n = 0; n < parameters.size(); ++n) {
      const auto& parameter = parameters.at(n);
      const auto objective_value = problem.objective_function(parameter);
      ++result.number_of_evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      local_best_objective_values.at(n) = objective_value;
      
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
      const auto n = result.number_of_evaluations % parameters.size();
      auto& parameter = parameters.at(n);
      const auto& local_best_parameter = local_best_parameters.at(n);
      
      std::array<T1, N> attraction_center;
      const auto weigthed_local_attraction = maximal_local_attraction * std::uniform_real_distribution<T1>(0, 1)(random_number_generator());
      const auto weigthed_global_attraction = maximal_global_attraction * std::uniform_real_distribution<T1>(0, 1)(random_number_generator());
      for (unsigned k = 0; k < this->active_dimensions.size(); ++k) {
        attraction_center.at(k) = (
            weigthed_local_attraction * (local_best_parameter.at(k) - parameter.at(k)) + 
            weigthed_global_attraction * (result.best_parameter.at(k) - parameter.at(k))) / 
          T1(3.0);
      }
      
      const auto&& random_velocity = random_neighbour(
        attraction_center,
        T1(0.0),
        std::sqrt(std::inner_product(
          attraction_center.cbegin(), attraction_center.cend(),
          attraction_center.cbegin(),
          T1(0.0))),
        this->active_dimensions.size());
      
      auto& velocity = velocities.at(n);
      const auto weigthed_acceleration = maximal_acceleration * std::uniform_real_distribution<T1>(0, 1)(random_number_generator());
      for (unsigned k = 0; k < this->active_dimensions.size(); ++k) {
        auto& parameter_element = parameter.at(k);
        auto& velocity_element = velocity.at(k);
        
        velocity_element = weigthed_acceleration * velocity_element + random_velocity.at(k);
        parameter_element += velocity_element;
        
        if (parameter_element < T1(0.0)) {
          parameter_element = T1(0.0);
          velocity_element *= -T1(0.5);
        } else if(parameter_element > T1(1.0)) {
          parameter_element = T1(1.0);
          velocity_element *= -T1(0.5);
        }
      }
        
      const auto objective_value = problem.objective_function(parameter);
      ++result.number_of_evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
        local_best_parameters.at(n) = parameter;
        local_best_objective_values.at(n) = objective_value;
      } else if (objective_value < local_best_objective_values.at(n)) {
        local_best_parameters.at(n) = parameter;
        local_best_objective_values.at(n) = objective_value;
      }
    }
      
    return result;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("particle_swarm_optimisation", "[particle_swarm_optimisation]") {
  constexpr unsigned number_of_dimensions = 3;
  mant::particle_swarm_optimisation<double, number_of_dimensions, mant::problem> optimiser;
  
  SECTION("Default configuration") {
    CHECK(optimiser.initial_velocity == Approx(0.5));
    CHECK(optimiser.maximal_acceleration == Approx(1.0 / (2.0 * std::log(2.0))));
    CHECK(optimiser.maximal_local_attraction == Approx(0.5 + std::log(2.0)));
    CHECK(optimiser.maximal_global_attraction == Approx(0.5 + std::log(2.0)));
  }
  
  SECTION("Benchmarking") {
    const std::array<std::unique_ptr<mant::problem<double, number_of_dimensions>>, 5> problems = {
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::ackley_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rastrigin_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rosenbrock_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sphere_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sum_of_different_powers_function<double, number_of_dimensions>)
    };
    
    std::vector<std::array<double, number_of_dimensions>> initial_parameters(10);
    for (auto& parameter : initial_parameters) {
      std::generate(
        parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<double>(0.0, 1.0),
          std::ref(random_number_generator())));
    }
    
    std::array<mant::optimise_result<double, number_of_dimensions>, problems.size()> results;
    optimiser.maximal_duration = std::chrono::seconds(10);
    optimiser.maximal_number_of_evaluations = 10'000'000;
    std::transform(
      problems.cbegin(), problems.cend(),
      results.begin(),
      [&optimiser, &initial_parameters](auto&& problem) {
        return optimiser.optimisation_function(*problem, initial_parameters);
      }
    );
    
    std::cout << "Particle swarm optimisation" << std::endl;
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
