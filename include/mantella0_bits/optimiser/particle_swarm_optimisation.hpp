/**
Particle swarm optimisation
===========================

.. cpp:class:: particle_swarm_optimisation : public optimiser

  .. versionadded:: 1.0.0 

  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  
  Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. 

  .. list-table:: Template parameters
    :widths: 27 73

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``unsigned``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<unsigned>::max()]``.

  .. list-table:: Member variables
    :widths: 27 73
  
    * - initial_velocity
    
        ``T``
      - Lorem ipsum dolor sit amet
  
    * - maximal_acceleration
    
        ``T``
      - Lorem ipsum dolor sit amet
  
    * - maximal_local_attraction
    
        ``T``
      - Lorem ipsum dolor sit amet
  
    * - maximal_global_attraction
    
        ``T``
      - Lorem ipsum dolor sit amet
      
  .. list-table:: Member functions
    :widths: 27 73
    
    * - particle_swarm_optimisation
    
        Constructor
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
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
    
    std::vector<std::array<T, N>> local_parameters = parameters;
    std::vector<T> local_objective_values(local_parameters.size());

    for (std::size_t n = 0; n < parameters.size(); ++n) {
      const auto& parameter = parameters.at(n);
      const auto objective_value = problem.objective_function(parameter);
      ++result.evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      local_objective_values.at(n) = objective_value;
      
      if (objective_value < result.objective_value) {
        result.parameter = parameter;
        result.objective_value = objective_value;
        
        if (result.objective_value <= this->acceptable_objective_value) {
          return result;
        }
      }
      
      if (result.evaluations >= this->maximal_evaluations) {
        return result;
      } else if (result.duration >= this->maximal_duration) {
        return result;
      }
    }
    
    while (result.duration < this->maximal_duration && result.evaluations < this->maximal_evaluations && result.objective_value > this->acceptable_objective_value) {
      const auto n = result.evaluations % parameters.size();
      auto& parameter = parameters.at(n);
      const auto& local_parameter = local_parameters.at(n);
      
      std::array<T, N> attraction_center;
      const auto weigthed_local_attraction = maximal_local_attraction * std::uniform_real_distribution<T>(0, 1)(random_number_generator());
      const auto weigthed_global_attraction = maximal_global_attraction * std::uniform_real_distribution<T>(0, 1)(random_number_generator());
      for (unsigned k = 0; k < this->active_dimensions.size(); ++k) {
        attraction_center.at(k) = (
            weigthed_local_attraction * (local_parameter.at(k) - parameter.at(k)) + 
            weigthed_global_attraction * (result.parameter.at(k) - parameter.at(k))) / 
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
      
      if (objective_value < result.objective_value) {
        result.parameter = parameter;
        result.objective_value = objective_value;
        local_parameters.at(n) = parameter;
        local_objective_values.at(n) = objective_value;
      } else if (objective_value < local_objective_values.at(n)) {
        local_parameters.at(n) = parameter;
        local_objective_values.at(n) = objective_value;
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
      result.parameter.cbegin(), std::next(result.parameter.cbegin(), optimiser.active_dimensions.size()),
      std::bind(std::greater_equal<double>{}, std::placeholders::_1, 0.0)
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
