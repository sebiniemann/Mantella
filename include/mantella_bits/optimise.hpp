#pragma once

template <typename T, std::size_t number_of_dimensions>
struct optimise_result {
  std::array<T, number_of_dimensions> best_found_parameter;
  T best_found_objective_value;
  std::size_t used_number_of_iterations;
  bool is_finished;
  bool is_terminated;
};

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2, typename T3>
optimise_result<T1, number_of_dimensions> optimise(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const T3& optimisation_algorithm,
    typename T3::state_type initial_state);

//
// Implementation
//

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2, typename T3>
optimise_result<T1, number_of_dimensions> optimise(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const T3& optimisation_algorithm,
    typename T3::state_type state) {
  std::transform(
    state.parameters.cbegin(),
    state.parameters.cend(),
    state.parameters.begin(),
    [&optimisation_problem](
        auto parameter) {
      for (std::size_t n = 0; n < number_of_dimensions; ++n) {
        parameter.at(n) = (parameter.at(n) - optimisation_problem.lower_bounds.at(n)) / (optimisation_problem.upper_bounds.at(n) - optimisation_problem.lower_bounds.at(n));
      }
      return parameter;
    }
  );

  state = std::accumulate(
    optimisation_algorithm.boundaries_handling_functions.cbegin(),
    optimisation_algorithm.boundaries_handling_functions.cend(),
    std::accumulate(
      optimisation_algorithm.initialising_functions.cbegin(),
      optimisation_algorithm.initialising_functions.cend(),
      state,
      [](
          const auto& state,
          const auto initialising_function) {
        return initialising_function.first(state);
      }
    ),
    [](
        const auto& state,
        const auto boundaries_handling_function) {
      return boundaries_handling_function.first(state);
    }
  );

  state.objective_values.resize(state.parameters.size());
  std::transform(
    state.parameters.cbegin(),
    state.parameters.cend(),
    state.objective_values.begin(),
    [&optimisation_problem, &optimisation_algorithm](
        auto parameter) {
      std::array<T1, number_of_dimensions> mapped_parameter;
      for (std::size_t n = optimisation_algorithm.active_dimensions.size(); n > 0; --n) {
        mapped_parameter.at(optimisation_algorithm.active_dimensions.at(n - 1)) = optimisation_problem.lower_bounds.at(n) + parameter.at(n - 1) * (optimisation_problem.upper_bounds.at(n) - optimisation_problem.lower_bounds.at(n));
      }
        
      return evaluate(optimisation_problem, mapped_parameter);
  });
  
  const auto best_found_objective_value = std::min_element(
    state.objective_values.cbegin(),
    state.objective_values.cend(),
    [](
        const auto& objective_value,
        const auto& other_objective_value) {
      return objective_value < other_objective_value;
    }
  );
  state.best_found_parameter = state.parameters.at(std::distance(state.objective_values.cbegin(), best_found_objective_value));
  state.best_found_objective_value = *best_found_objective_value;
  
  for (state.used_number_of_iterations = 1; state.used_number_of_iterations < optimisation_algorithm.maximal_number_of_iterations && optimisation_algorithm.acceptable_objective_value < state.best_found_objective_value; ++state.used_number_of_iterations) {
    state = std::accumulate(
      optimisation_algorithm.boundaries_handling_functions.cbegin(),
      optimisation_algorithm.boundaries_handling_functions.cend(),
      std::any_of(
        optimisation_algorithm.is_stagnating_functions.cbegin(),
        optimisation_algorithm.is_stagnating_functions.cend(),
        [&state](
            const auto is_stagnating_function) {
          return is_stagnating_function.first(std::move(state));
        }
      ) ? std::accumulate(
        optimisation_algorithm.restarting_functions.cbegin(),
        optimisation_algorithm.restarting_functions.cend(),
        state,
        [](
            const auto& state,
            const auto restarting_function) {
          return restarting_function.first(state);
        }
      ) : std::accumulate(
        optimisation_algorithm.next_parameters_functions.cbegin(),
        optimisation_algorithm.next_parameters_functions.cend(),
        state,
        [](
            const auto& state,
            const auto next_parameters_function) {
          return next_parameters_function.first(state);
        }
      ),
      [](
          const auto& state,
          const auto boundaries_handling_function) {
        return boundaries_handling_function.first(state);
    });

    std::transform(
      state.parameters.cbegin(),
      state.parameters.cend(),
      state.objective_values.begin(),
      [&optimisation_problem, &optimisation_algorithm](
          auto parameter) {
        std::array<T1, number_of_dimensions> mapped_parameter;
        for (std::size_t n = optimisation_algorithm.active_dimensions.size(); n > 0; --n) {
          mapped_parameter.at(optimisation_algorithm.active_dimensions.at(n - 1)) = optimisation_problem.lower_bounds.at(n) + parameter.at(n - 1) * (optimisation_problem.upper_bounds.at(n) - optimisation_problem.lower_bounds.at(n));
        }
          
        return evaluate(optimisation_problem, mapped_parameter);
    });

    const auto best_found_objective_value = std::min_element(
      state.objective_values.cbegin(),
      state.objective_values.cend(),
      [](
          const auto& objective_value,
          const auto& other_objective_value) {
        return objective_value < other_objective_value;
      }
    );
    state.best_found_parameter = state.parameters.at(std::distance(state.objective_values.cbegin(), best_found_objective_value));
    state.best_found_objective_value = *best_found_objective_value;
  }
  
  for (std::size_t n = 0; n < number_of_dimensions; ++n) {
    state.best_found_parameter.at(n) = optimisation_problem.lower_bounds.at(n) + state.best_found_parameter.at(n) * (optimisation_problem.upper_bounds.at(n) - optimisation_problem.lower_bounds.at(n));
  }

  return {
    state.best_found_parameter,
    state.best_found_objective_value, 
    state.used_number_of_iterations,
    state.best_found_objective_value <= optimisation_algorithm.acceptable_objective_value,
    state.used_number_of_iterations >= optimisation_algorithm.maximal_number_of_iterations,
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise", "[optimise]") {
  const mant::sphere_function<double, 2> sphere_function;
  const mant::particle_swarm_optimisation<double, 2> particle_swarm_optimisation;
  typename mant::particle_swarm_optimisation<double, 2>::state_type initial_state;
  initial_state.parameters = {{0.0, 0.0}, {0.1, 0.0}};
  const auto&& result = mant::optimise(sphere_function, particle_swarm_optimisation, initial_state);
}
#endif