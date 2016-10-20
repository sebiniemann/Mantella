#pragma once

template <
  typename T,
  std::size_t number_of_dimensions>
struct optimise_result {
  std::array<T, number_of_dimensions> best_found_parameter;
  T best_found_objective_value;
  std::size_t used_number_of_iterations;
  bool is_finished;
  bool is_terminated;
};

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2,
  typename T3>
optimise_result<T1, number_of_dimensions> optimise(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const T3& optimisation_algorithm,
    typename T3::state_type initial_state);

//
// Implementation
//

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2,
  typename T3>
optimise_result<T1, number_of_dimensions> optimise(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const T3& optimisation_algorithm,
    typename T3::state_type state) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<mant::optimisation_problem<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  assert(state.parameters.size() > 0);
  
  // Maps the parameter's bounds from [*lower_bounds*, *upper_bounds*] to [0, 1] and places all active dimensions 
  // (in-order) upfront.
  for (auto& parameter : state.parameters) {
    for (std::size_t n = 0; n < optimisation_algorithm.active_dimensions.size(); ++n) {
      parameter.at(n) = (
          parameter.at(optimisation_algorithm.active_dimensions.at(n)) - 
          optimisation_problem.lower_bounds.at(n)
        ) / (
          optimisation_problem.upper_bounds.at(n) - optimisation_problem.lower_bounds.at(n)
        );
    }
  }

  for(const auto boundary_handling_function : optimisation_algorithm.boundary_handling_functions) {
    for(const auto initialising_function : optimisation_algorithm.initialising_functions) {
      std::get<0>(initialising_function)(state);
    }
    std::get<0>(boundary_handling_function)(state);
  }

  // Maps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*], permutes the parameter to match 
  // the active dimensions and evaluates the objective value afterwards for each parameter.
  // The mapping and permutation is done on a local copy and does not affect the actual parameters.
  state.objective_values.resize(state.parameters.size());
  std::transform(
    state.parameters.cbegin(), state.parameters.cend(),
    state.objective_values.begin(),
    [&optimisation_problem, &optimisation_algorithm](
        auto parameter) {
      std::array<T1, number_of_dimensions> mapped_parameter;
      for (std::size_t n = optimisation_algorithm.active_dimensions.size(); n > 0; --n) {
        mapped_parameter.at(optimisation_algorithm.active_dimensions.at(n - 1)) = 
          optimisation_problem.lower_bounds.at(n - 1) +
          parameter.at(n - 1) * (
            optimisation_problem.upper_bounds.at(n - 1) - optimisation_problem.lower_bounds.at(n - 1)
          );
      }
        
      return evaluate(optimisation_problem, mapped_parameter);
  });
  
  // Updates *best_found_parameter* and *best_found_objective_value*.
  const auto best_found_objective_value = std::min_element(
    state.objective_values.cbegin(),
    state.objective_values.cend(),
    [](
        const auto& objective_value,
        const auto& other_objective_value) {
      return objective_value < other_objective_value;
    }
  );
  if (*best_found_objective_value < state.best_found_objective_value) {
    state.best_found_parameter = state.parameters.at(std::distance(state.objective_values.cbegin(), best_found_objective_value));
    state.best_found_objective_value = *best_found_objective_value;
    state.stagnating_number_of_iterations = 0;
  } else {
    ++state.stagnating_number_of_iterations;
  }
  
  for (state.used_number_of_iterations = 1; state.used_number_of_iterations < optimisation_algorithm.maximal_number_of_iterations && optimisation_algorithm.acceptable_objective_value < state.best_found_objective_value; ++state.used_number_of_iterations) {
    for(const auto boundary_handling_function : optimisation_algorithm.boundary_handling_functions) {
      // Tests if the optimisation algorithm is stagnating and either restarts it or proceeds with the next parameters.
      if(std::any_of(
        optimisation_algorithm.is_stagnating_functions.cbegin(),
        optimisation_algorithm.is_stagnating_functions.cend(),
        [&state](
            const auto is_stagnating_function) {
          return std::get<0>(is_stagnating_function)(state);
        }
      )) {
        for(const auto restarting_function : optimisation_algorithm.restarting_functions) {
          std::get<0>(restarting_function)(state);
        }
      } else {
        for(const auto next_parameters_function : optimisation_algorithm.next_parameters_functions) {
          std::get<0>(next_parameters_function)(state);
        }
      }
      std::get<0>(boundary_handling_function)(state);
    };
    
    // Maps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*], permutes the parameter to match 
    // the active dimensions and evaluates the objective value afterwards for each parameter.
    // The mapping and permutation is done on a local copy and does not affect the actual parameters.
    state.objective_values.resize(state.parameters.size());
    std::transform(
      state.parameters.cbegin(), state.parameters.cend(),
      state.objective_values.begin(),
      [&optimisation_problem, &optimisation_algorithm](
          auto parameter) {
        std::array<T1, number_of_dimensions> mapped_parameter;
        for (std::size_t n = optimisation_algorithm.active_dimensions.size(); n > 0; --n) {
          mapped_parameter.at(optimisation_algorithm.active_dimensions.at(n - 1)) = 
            optimisation_problem.lower_bounds.at(n - 1) +
            parameter.at(n - 1) * (
              optimisation_problem.upper_bounds.at(n - 1) - optimisation_problem.lower_bounds.at(n - 1)
            );
        }
          
        return evaluate(optimisation_problem, mapped_parameter);
    });
    
    // Updates *best_found_parameter* and *best_found_objective_value*.
    const auto best_found_objective_value = std::min_element(
      state.objective_values.cbegin(),
      state.objective_values.cend(),
      [](
          const auto& objective_value,
          const auto& other_objective_value) {
        return objective_value < other_objective_value;
      }
    );
    if (*best_found_objective_value < state.best_found_objective_value) {
      state.best_found_parameter = state.parameters.at(std::distance(state.objective_values.cbegin(), best_found_objective_value));
      state.best_found_objective_value = *best_found_objective_value;
      state.stagnating_number_of_iterations = 0;
    } else {
      ++state.stagnating_number_of_iterations;
    }
  }
  
  // Maps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*], permutes the parameter to match 
  // the active dimensions.
  for (std::size_t n = optimisation_algorithm.active_dimensions.size(); n > 0; --n) {
    state.best_found_parameter.at(optimisation_algorithm.active_dimensions.at(n - 1)) = 
      optimisation_problem.lower_bounds.at(n - 1) +
      state.best_found_parameter.at(n - 1) * (
        optimisation_problem.upper_bounds.at(n - 1) - optimisation_problem.lower_bounds.at(n - 1)
      );
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
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 2;
  
  mant::sphere_function<value_type, number_of_dimensions> sphere_function;
  sphere_function.lower_bounds = {-5.0, -5.0};
  sphere_function.upper_bounds = {5.0, 5.0};
  mant::hooke_jeeves_algorithm<value_type, number_of_dimensions> hooke_jeeves_algorithm;
  hooke_jeeves_algorithm.acceptable_objective_value = 1e-12;
  hooke_jeeves_algorithm_state<value_type, number_of_dimensions> hooke_jeeves_algorithm_state;
  hooke_jeeves_algorithm_state.parameters = {{-3.2, 4.1}, {6.0, -4.1}};
  
  const auto&& result = mant::optimise(sphere_function, hooke_jeeves_algorithm, hooke_jeeves_algorithm_state);
  CHECK((result.best_found_parameter == std::array<value_type, number_of_dimensions>({-6.67572022550189104e-07, 3.33786010386916132e-07})));
  CHECK(result.best_found_objective_value == Approx(5.57065506021764692e-13));
  CHECK(result.used_number_of_iterations == 48);
  CHECK(result.is_finished == true);
  CHECK(result.is_terminated == false);
}
#endif