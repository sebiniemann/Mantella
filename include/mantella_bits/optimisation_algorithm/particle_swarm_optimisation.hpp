#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct particle_swarm_optimisation_state : optimisation_algorithm_state<T, number_of_dimensions> {
  std::vector<std::array<T, number_of_dimensions>> velocities;
  std::vector<std::array<T, number_of_dimensions>> local_best_found_parameter;
  std::vector<T> local_best_objective_values;
  
  constexpr particle_swarm_optimisation_state() noexcept;
};

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2 = particle_swarm_optimisation_state>
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

template <typename T, std::size_t number_of_dimensions>
constexpr particle_swarm_optimisation_state<T, number_of_dimensions>::particle_swarm_optimisation_state() noexcept
    : optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() {

}

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2>
constexpr particle_swarm_optimisation<T1, number_of_dimensions, T2>::particle_swarm_optimisation() noexcept 
    : optimisation_algorithm<T1, number_of_dimensions, T2>() {
  this->initialising_functions = {{
    [this](
        auto state) {
      state.velocities.resize(state.parameters.size());
      std::generate(
        state.velocities.begin(),
        state.velocities.end(),
        [this]() {
          std::array<T1, number_of_dimensions> velocity;
          std::generate(
            velocity.begin(),
            std::next(velocity.begin(), this->active_dimensions.size()),
            std::bind(std::uniform_real_distribution<T1>(-initial_velocity, initial_velocity), std::ref(random_number_generator()))
          );
          
          return velocity;
        }
      );
      
      return state;
    },
    "Draws randomly and uniformly one velocity vector per parameter from [-*initial_velocity*, *initial_velocity*]"
  }, {
    [this](
        auto state) {
      state.local_best_found_parameter = state.parameters;
            
      return state;
    },
    "Sets *local_best_found_parameter* to equal the initial parameters."
  }, {
    [this](
        auto state) {
      state.local_best_objective_values.resize(state.local_best_found_parameter.size());
      std::fill(state.local_best_objective_values.begin(), state.local_best_objective_values.end(), std::numeric_limits<T1>::infinity());
            
      return state;
    },
    "Sets *local_best_objective_values* to infinity."
  }};
  
  this->boundaries_handling_functions.insert(this->boundaries_handling_functions.begin(), {
    [this](
        auto state) {
      for (std::size_t n = 0; n < state.parameters.size(); ++n) {
        for (std::size_t k = 0; k < this->active_dimensions.size(); ++k) {
          if (state.parameters.at(n).at(k) < T1(0.0) || state.parameters.at(n).at(k) > T1(1.0)) {
            state.velocities.at(n).at(k) *= T1(0.5);
          }
        }
      }

      return state;
    },
    "Halves the parameter's velocity per dimension, if the parameter is out-of-bound."
  });
  
  this->next_parameters_functions = {{
    [this](
        auto state) {
      for (std::size_t n = 0; n < state.objective_values.size(); ++n) {
        if (state.objective_values.at(n) < state.local_best_objective_values.at(n)) {
          state.local_best_found_parameter.at(n) = state.parameters.at(n);
          state.local_best_objective_values.at(n) = state.objective_values.at(n);
        }
      }
      
      return state;
    },
    "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration."
  }, {
    [this](
        auto state) {
      for (std::size_t n = 0; n < state.parameters.size(); ++n) {
        const auto& parameter = state.parameters.at(n);
        const auto& local_best_found_parameter = state.local_best_found_parameter.at(n);
        
        std::array<T1, number_of_dimensions> attraction_center;
        for (std::size_t k = 0; k < this->active_dimensions.size(); ++k) {
          attraction_center.at(k) = (maximal_local_attraction * (local_best_found_parameter.at(k) - parameter.at(k)) + maximal_global_attraction * (state.best_found_parameter.at(k) - parameter.at(k))) / T1(3.0);
        }
        
        auto&& random_velocity = random_neighbour(attraction_center, T1(0.0), cblas_dnrm2(this->active_dimensions.size(), attraction_center.data(), 1), this->active_dimensions.size());
        
        auto& velocity = state.velocities.at(n);
        for (std::size_t k = 0; k < this->active_dimensions.size(); ++k) {
          velocity.at(k) = maximal_acceleration * std::uniform_real_distribution<T1>(0, 1)(random_number_generator()) * velocity.at(k) + random_velocity.at(k);
        }
      }
      
      return state;
    },
    "Updates the local best parameter and objective value, if the parameter's objective value improved in the previous iteration."
  }, {
    [this](
        auto state) {
      for (std::size_t n = 0; n < state.parameters.size(); ++n) {
        const auto& velocity = state.velocities.at(n);
        auto& parameter = state.parameters.at(n);
        for (std::size_t k = 0; k < this->active_dimensions.size(); ++k) {
          parameter.at(k) += velocity.at(k);
        }
      }
      
      return state;
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
TEST_CASE("particle_swarm_optimisation", "[particle_swarm_optimisation]") {
  
}
#endif
