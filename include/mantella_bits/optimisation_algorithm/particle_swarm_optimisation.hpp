#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct particle_swarm_optimisation_state : optimisation_algorithm_state<T, number_of_dimensions> {
  std::vector<std::array<T, number_of_dimensions>> velocities;
  std::vector<std::array<T, number_of_dimensions>> local_best_found_parameters;
  std::vector<T> local_best_objective_values;
  
  constexpr particle_swarm_optimisation_state() noexcept;
};

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2 = particle_swarm_optimisation_state>
struct particle_swarm_optimisation : optimisation_algorithm<T1, number_of_dimensions, T2> {
  T1 initial_velocity;
  T1 maximal_acceleration;
  T1 maximal_local_attraction;
  T1 maximal_global_attraction;
  
  constexpr particle_swarm_optimisation() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr particle_swarm_optimisation_state<T, number_of_dimensions>::particle_swarm_optimisation_state() noexcept
    : optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  velocities.resize(this->parameters.size());
  local_best_found_parameters = this->parameters;
  local_best_objective_values.resize(this->parameters.size());
  std::fill(
    local_best_objective_values.begin(), local_best_objective_values.end(),
    std::numeric_limits<T>::infinity());
}

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
constexpr particle_swarm_optimisation<T1, number_of_dimensions, T2>::particle_swarm_optimisation() noexcept 
    : optimisation_algorithm<T1, number_of_dimensions, T2>() {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<particle_swarm_optimisation_state<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  
  this->initialising_functions = {{
    [this](
        auto& state) {
      state.velocities.resize(state.parameters.size());
      for (auto& velocity : state.velocities) {
        std::generate(
          velocity.begin(), std::next(velocity.begin(), this->active_dimensions.size()),
          std::bind(std::uniform_real_distribution<T1>(-initial_velocity, initial_velocity), std::ref(random_number_generator()))
        );
      }
    },
    "Draws randomly and uniformly one velocity vector per parameter from [-*initial_velocity*, *initial_velocity*]"
  }, {
    [this](
        auto& state) {
      state.local_best_found_parameters = state.parameters;
    },
    "Sets *local_best_found_parameters* to equal the initial parameters."
  }, {
    [this](
        auto& state) {
      state.local_best_objective_values.resize(state.local_best_found_parameters.size());
      std::fill(
        state.local_best_objective_values.begin(), state.local_best_objective_values.end(),
        std::numeric_limits<T1>::infinity());
    },
    "Sets *local_best_objective_values* to infinity."
  }};
  
  this->boundary_handling_functions.insert(this->boundary_handling_functions.begin(), {
    [this](
        auto& state) {
      for (std::size_t n = 0; n < state.parameters.size(); ++n) {
        auto& velocity = state.velocities.at(n);
        const auto& parameter = state.parameters.at(n);
        
        std::transform(
          velocity.cbegin(), std::next(velocity.cbegin(), this->active_dimensions.size()),
          parameter.cbegin(),
          velocity.begin(),
          [](auto& velocity_element, auto& parameter_element) {
            if (parameter_element < T1(0.0) || parameter_element > T1(1.0)) {
              return T1(0.5) * velocity_element;
            }
            return velocity_element;
          });
      }
    },
    "Halves the parameter's velocity per dimension, if the parameter is out-of-bound."
  });
  
  this->next_parameters_functions = {{
    [this](
        auto& state) {
      for (std::size_t n = 0; n < state.objective_values.size(); ++n) {
        if (state.objective_values.at(n) < state.local_best_objective_values.at(n)) {
          state.local_best_found_parameters.at(n) = state.parameters.at(n);
          state.local_best_objective_values.at(n) = state.objective_values.at(n);
        }
      }
    },
    "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration."
  }, {
    [this](
        auto& state) {
      for (std::size_t n = 0; n < state.parameters.size(); ++n) {
        const auto& parameter = state.parameters.at(n);
        const auto& local_best_found_parameter = state.local_best_found_parameters.at(n);
        
        std::array<T1, number_of_dimensions> attraction_center;
        for (std::size_t k = 0; k < this->active_dimensions.size(); ++k) {
          attraction_center.at(k) = (
              maximal_local_attraction * (local_best_found_parameter.at(k) - parameter.at(k)) + 
              maximal_global_attraction * (state.best_found_parameter.at(k) - parameter.at(k))) / 
            T1(3.0);
        }
        
        auto&& random_velocity = random_neighbour(
          attraction_center,
          T1(0.0),
          std::sqrt(std::inner_product(
            attraction_center.cbegin(), attraction_center.cend(),
            attraction_center.cbegin(),
            T1(0.0))),
          this->active_dimensions.size());
        
        auto& velocity = state.velocities.at(n);
        std::transform(
          velocity.cbegin(), std::next(velocity.cbegin(), this->active_dimensions.size()),
          random_velocity.cbegin(),
          velocity.begin(),
          [this](const auto velocity_element, const auto random_velocity_element) {
            return
              maximal_acceleration * std::uniform_real_distribution<T1>(0, 1)(random_number_generator()) * velocity_element +
              random_velocity_element;
          }
        );
      }
    },
    "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration."
  }, {
    [this](
        auto& state) {
      for (std::size_t n = 0; n < state.parameters.size(); ++n) {
        const auto& velocity = state.velocities.at(n);
        auto& parameter = state.parameters.at(n);
        
        std::transform(
          parameter.cbegin(), std::next(parameter.cbegin(), this->active_dimensions.size()),
          velocity.cbegin(),
          parameter.begin(),
          [](auto& velocity_element, auto& parameter_element) {
            return parameter_element + velocity_element;
          });
      }
    },
    "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration."
  }};
  
  maximal_acceleration = 1.0 / (2.0 * std::log(2.0));
  maximal_local_attraction = 0.5 + std::log(2.0);
  maximal_global_attraction = maximal_local_attraction;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("particle_swarm_optimisation_state", "[particle_swarm_optimisation][particle_swarm_optimisation_state]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::particle_swarm_optimisation_state<value_type, number_of_dimensions> particle_swarm_optimisation_state;
  
  CHECK(particle_swarm_optimisation_state.velocities.size() == particle_swarm_optimisation_state.parameters.size());
  CHECK(particle_swarm_optimisation_state.local_best_found_parameters.size() == particle_swarm_optimisation_state.parameters.size());
  CHECK(particle_swarm_optimisation_state.local_best_objective_values.size() == particle_swarm_optimisation_state.parameters.size());
  CHECK(std::all_of(
    particle_swarm_optimisation_state.local_best_objective_values.cbegin(), particle_swarm_optimisation_state.local_best_objective_values.cend(),
    [](const auto element) {
      return element == std::numeric_limits<value_type>::infinity();
    }) == true);
}

TEST_CASE("particle_swarm_optimisation", "[particle_swarm_optimisation]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  mant::particle_swarm_optimisation<value_type, number_of_dimensions> particle_swarm_optimisation;
  mant::particle_swarm_optimisation_state<value_type, number_of_dimensions> particle_swarm_optimisation_state;
  
  SECTION("Default values") {
    CHECK(particle_swarm_optimisation.maximal_acceleration == Approx(1.0 / (2.0 * std::log(2.0))));
    CHECK(particle_swarm_optimisation.maximal_local_attraction == Approx(0.5 + std::log(2.0)));
    CHECK(particle_swarm_optimisation.maximal_global_attraction == Approx(0.5 + std::log(2.0)));
  }
  
  SECTION("Initialising functions") {
    CHECK(particle_swarm_optimisation.initialising_functions.size() == 3);
    
    SECTION("First function") {
      CHECK(std::get<1>(particle_swarm_optimisation.initialising_functions.at(0)) == "Draws randomly and uniformly one velocity vector per parameter from [-*initial_velocity*, *initial_velocity*]");
    }
    
    SECTION("Second function") {
      CHECK(std::get<1>(particle_swarm_optimisation.initialising_functions.at(1)) == "Sets *local_best_found_parameters* to equal the initial parameters.");
    }
    
    SECTION("Third function") {
      CHECK(std::get<1>(particle_swarm_optimisation.initialising_functions.at(2)) == "Sets *local_best_objective_values* to infinity.");
    }
  }
  
  SECTION("Boundary handling functions") {
    // The second boundary handling functions is derived from the the base struct.
    CHECK(particle_swarm_optimisation.boundary_handling_functions.size() == 2);
    CHECK(std::get<1>(particle_swarm_optimisation.boundary_handling_functions.at(0)) == "Halves the parameter's velocity per dimension, if the parameter is out-of-bound.");
    
  }
  
  SECTION("Next parameters function") {
    CHECK(particle_swarm_optimisation.next_parameters_functions.size() == 3);
    
    SECTION("First function") {
      CHECK(std::get<1>(particle_swarm_optimisation.next_parameters_functions.at(0)) == "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration.");
    }
    
    SECTION("Second function") {
      CHECK(std::get<1>(particle_swarm_optimisation.next_parameters_functions.at(1)) == "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration.");
    }
    
    SECTION("Third function") {
      CHECK(std::get<1>(particle_swarm_optimisation.next_parameters_functions.at(2)) == "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration.");
    }
  }
}
#endif
