/**
particle_swarm_optimisation
===========================

.. cpp:class:: template<T, N> particle_swarm_optimisation

  **Template parameters**
  
    - **T** (``floating point``)

      - Lorem ipsum dolor sit amet
    
    - **N** (``unsigned``)

      - Lorem ipsum dolor sit amet
  
  .. cpp:member:: T initial_velocity
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: T maximal_acceleration
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: T maximal_local_attraction
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: T maximal_global_attraction
  
    Lorem ipsum dolor sit amet
      
  .. cpp:function:: particle_swarm_optimisation()
  
    Lorem ipsum dolor sit amet
*/
template <typename T, unsigned N>
struct particle_swarm_optimisation : optimiser<T, N> {
  T initial_velocity;
  T maximal_acceleration;
  T maximal_local_attraction;
  T maximal_global_attraction;
  
  particle_swarm_optimisation() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
particle_swarm_optimisation<T, N>::particle_swarm_optimisation() noexcept 
    : optimiser<T, N>(),
      initial_velocity(T(0.5)),
      maximal_acceleration(T(1.0) / (T(2.0) * std::log(T(2.0)))),
      maximal_local_attraction(T(0.5) + std::log(T(2.0))),
      maximal_global_attraction(maximal_local_attraction) {
  this->optimisation_function = [this](const mant::problem<T, N>& problem, std::vector<std::array<T, N>> parameters) {
    assert(initial_velocity >= T(0.0));
    assert(maximal_acceleration >= T(0.0));
    assert(maximal_local_attraction >= T(0.0));
    assert(maximal_global_attraction >= T(0.0));
    
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T, N> result;
    
    std::vector<std::array<T, N>> velocities(parameters.size());
    for (auto& velocity : velocities) {
      std::generate(
        velocity.begin(), std::next(velocity.begin(), this->active_dimensions.size()),
        std::bind(std::uniform_real_distribution<T>(-initial_velocity, initial_velocity), std::ref(random_number_generator()))
      );
    }
    
    std::vector<std::array<T, N>> local_best_parameters = parameters;
    std::vector<T> local_best_objective_values(local_best_parameters.size());

    for (std::size_t n = 0; n < parameters.size(); ++n) {
      const auto& parameter = parameters.at(n);
      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      local_best_objective_values.at(n) = objective_value;
      
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
      const auto n = result.evaluations % parameters.size();
      auto& parameter = parameters.at(n);
      const auto& local_best_parameter = local_best_parameters.at(n);
      
      std::array<T, N> attraction_center;
      const auto weigthed_local_attraction = maximal_local_attraction * std::uniform_real_distribution<T>(0, 1)(random_number_generator());
      const auto weigthed_global_attraction = maximal_global_attraction * std::uniform_real_distribution<T>(0, 1)(random_number_generator());
      for (unsigned k = 0; k < this->active_dimensions.size(); ++k) {
        attraction_center.at(k) = (
            weigthed_local_attraction * (local_best_parameter.at(k) - parameter.at(k)) + 
            weigthed_global_attraction * (result.best_parameter.at(k) - parameter.at(k))) / 
          T(3.0);
      }
      
      const auto&& random_velocity = random_neighbour(
        attraction_center,
        T(0.0),
        std::sqrt(std::inner_product(
          attraction_center.cbegin(), attraction_center.cend(),
          attraction_center.cbegin(),
          T(0.0))),
        this->active_dimensions.size());
      
      auto& velocity = velocities.at(n);
      const auto weigthed_acceleration = maximal_acceleration * std::uniform_real_distribution<T>(0, 1)(random_number_generator());
      for (unsigned k = 0; k < this->active_dimensions.size(); ++k) {
        auto& parameter_element = parameter.at(k);
        auto& velocity_element = velocity.at(k);
        
        velocity_element = weigthed_acceleration * velocity_element + random_velocity.at(k);
        parameter_element += velocity_element;
        
        if (parameter_element < T(0.0)) {
          parameter_element = T(0.0);
          velocity_element *= -T(0.5);
        } else if(parameter_element > T(1.0)) {
          parameter_element = T(1.0);
          velocity_element *= -T(0.5);
        }
      }
        
      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
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
  constexpr unsigned dimensions = 3;
  mant::particle_swarm_optimisation<double, dimensions> optimiser;
  
  SECTION("Default configuration") {
    CHECK(optimiser.initial_velocity == Approx(0.5));
    CHECK(optimiser.maximal_acceleration == Approx(1.0 / (2.0 * std::log(2.0))));
    CHECK(optimiser.maximal_local_attraction == Approx(0.5 + std::log(2.0)));
    CHECK(optimiser.maximal_global_attraction == Approx(0.5 + std::log(2.0)));
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
