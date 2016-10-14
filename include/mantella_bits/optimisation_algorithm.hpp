#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct optimisation_algorithm_state {
  std::vector<std::array<T, number_of_dimensions>> parameters;
  std::vector<T> objective_values;
  
  std::array<T, number_of_dimensions> best_found_parameter;
  T best_found_objective_value;
  std::size_t used_number_of_iterations;
  std::size_t stagnating_number_of_iterations;
  
  constexpr optimisation_algorithm_state() noexcept;
};

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2 = optimisation_algorithm_state>
struct optimisation_algorithm {
  typedef T1 value_type;
  
  typedef T2<T1, number_of_dimensions> state_type;
  
  typedef std::function<void(
      state_type& state)> initialising_function_type;
      
  typedef std::function<void(
      state_type& state)> boundaries_handling_function_type;
      
  typedef std::function<bool(
      const state_type& state)> is_stagnating_function_type;
      
  typedef std::function<void(
      state_type& state)> next_parameters_function_type;
      
  typedef std::function<void(
      state_type& state)> restarting_function_type;

  std::vector<std::pair<initialising_function_type, std::string>> initialising_functions;
  std::vector<std::pair<boundaries_handling_function_type, std::string>> boundaries_handling_functions;
  std::vector<std::pair<is_stagnating_function_type, std::string>> is_stagnating_functions;
  std::vector<std::pair<next_parameters_function_type, std::string>> next_parameters_functions;
  std::vector<std::pair<restarting_function_type, std::string>> restarting_functions;
  
  T1 acceptable_objective_value;
  std::size_t maximal_number_of_iterations;
  std::size_t maximal_stagnating_number_of_iterations;
  
  std::vector<std::size_t> active_dimensions;
  
  constexpr optimisation_algorithm() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() noexcept {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  best_found_objective_value = std::numeric_limits<T>::infinity();
  used_number_of_iterations = 0;
  stagnating_number_of_iterations = 0;
}

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
constexpr optimisation_algorithm<T1, number_of_dimensions, T2>::optimisation_algorithm() noexcept {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<optimisation_algorithm_state, T2>::value, "");
  
  boundaries_handling_functions = {{
    [](
        auto&& state) {
      for (auto& bounded_parameter : state.parameters) {
        for (auto& element : bounded_parameter) {
          if (element < T1(0.0)) {
            element = T1(0.0);
          } else if(element > T1(1.0)) {
            element = T1(1.0);
          }
        }
      }

      return state;
    },
    "Clamps all elements to be between [0, 1]."
  }};
  
  is_stagnating_functions = {{
    [](
        const auto& state) {
      return state.stagnating_number_of_iterations > maximal_stagnating_number_of_iterations;
    },
    "Returns true if *state.stagnating_number_of_iterations* is greater than *maximal_stagnating_number_of_iterations*."
  }};
  
  restarting_functions = {{
    [](
        auto&& state) {
      std::generate(
        state.parameters.begin(), state.parameters.end(),
        []() {
          std::array<T1, number_of_dimensions> parameter;
          std::generate(
            parameter.begin(), parameter.end(),
            std::bind(
              std::uniform_real_distribution<T1>(0.0, 1.0),
              std::ref(random_number_generator())));
          
          return parameter;
        }
      );
          
      return state;
    },
    "Draws all parameters randomly and uniformly from [*lower_bounds*, *upper_bounds*]."
  }};
  
  acceptable_objective_value = -std::numeric_limits<T1>::infinity();
  maximal_number_of_iterations = 10000;
  maximal_stagnating_number_of_iterations = 100;
  
  active_dimensions.resize(number_of_dimensions);
  std::iota(active_dimensions.begin(), active_dimensions.end(), 0);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimisation_algorithm_state", "[optimisation_algorithm][optimisation_algorithm_state]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 2;
  mant::optimisation_algorithm_state<value_type, value_type> optimisation_algorithm_state;
  
  // Checks that the parameters contain initially one parameter.
  CHECK(optimisation_algorithm_state.parameters.size() == 1);
  // Checks that the objective values are initially unset.
  CHECK(optimisation_algorithm_state.objective_values.empty() == true);
  // The best found parameter is untested, as it might be anything initially.
  // Checks that the best found objective value is infinity initially.
  CHECK(optimisation_algorithm_state.best_found_objective_value == std::numeric_limits<value_type>::infinity());
  // Checks that the used number of iterations is zero initially.
  CHECK(optimisation_algorithm_state.used_number_of_iterations == 0);
  // Checks that the stagnating number of iterations is zero initially.
  CHECK(optimisation_algorithm_state.stagnating_number_of_iterations == 0);
}

// TEST_CASE("optimisation_algorithm", "[optimisation_algorithm]") {
  // mant::optimisation_algorithm<double, 2> optimisation_algorithm;
  
  // SECTION("Default parameters") {
    // CHECK(optimisation_algorithm.acceptable_objective_value == -::std::numeric_limits<double>::infinity());
    // CHECK(optimisation_algorithm.maximal_number_of_iterations == 10000);
    // CHECK(optimisation_algorithm.active_dimensions == std::vector<::std::size_t>({0, 1}));
  // }
  
  // SECTION("Functions") {
    // CHECK(optimisation_algorithm.initialising_functions.size() == 0);
    
    // CHECK(optimisation_algorithm.boundaries_handling_functions.size() == 1);
    // CHECK(optimisation_algorithm.boundaries_handling_functions.at(0).second == "Places dimensions out of bound on the missed bound");
    
    // SECTION("Places dimensions out of bound on the missed bound") {
      
      
      // optimisation_algorithm.boundaries_handling_functions.at(0).first()
    // }
    
    // CHECK(optimisation_algorithm.is_stagnating_functions.size() == 1);
    // CHECK(optimisation_algorithm.is_stagnating_functions.at(0).second == "Returns true if the solver stagnated for 100 iterations or more");
    
    // CHECK(optimisation_algorithm.next_parameters_functions.size() == 0);
    
    // CHECK(optimisation_algorithm.restarting_functions.size() == 1);
    // CHECK(optimisation_algorithm.restarting_functions.at(0).second == "Draws parameters uniformly within the bounds");
  // }
// }
#endif
