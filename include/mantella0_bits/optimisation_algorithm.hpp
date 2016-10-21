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
      state_type& state)> boundary_handling_function_type;
      
  typedef std::function<bool(
      const state_type& state)> is_stagnating_function_type;
      
  typedef std::function<void(
      state_type& state)> next_parameters_function_type;
      
  typedef std::function<void(
      state_type& state)> restarting_function_type;

  std::vector<std::pair<initialising_function_type, std::string>> initialising_functions;
  std::vector<std::pair<boundary_handling_function_type, std::string>> boundary_handling_functions;
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
  
  parameters.resize(1);
  std::generate(
    parameter.begin(), parameter.end(),
    std::bind(
      std::uniform_real_distribution<T>(0.0, 1.0),
      std::ref(random_number_generator())));
  
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
  static_assert(std::is_base_of<state_type, T2<T1, number_of_dimensions>>::value, "");
  
  boundary_handling_functions = {{
    [this](
        auto& state) {
      for (auto& parameter : state.parameters) {
        std::transform(
          parameter.cbegin(), std::next(parameter.cbegin(), active_dimensions.size()),
          parameter.begin(),
          [] (const auto& element) {
            if (element < T1(0.0)) {
              return T1(0.0);
            } else if(element > T1(1.0)) {
              return T1(1.0);
            }
            
            return element;
          });
      }
    },
    "Default boundary handling"
  }};
  
  is_stagnating_functions = {{
    [this](
        const auto& state) {
      return state.stagnating_number_of_iterations > maximal_stagnating_number_of_iterations;
    },
    "Default is-stagnating"
  }};
  
  restarting_functions = {{
    [this](
        auto& state) {
      for (auto& parameter : state.parameters) {
        std::generate(
          parameter.begin(), std::next(parameter.begin(), active_dimensions.size()),
          std::bind(
            std::uniform_real_distribution<T1>(0.0, 1.0),
            std::ref(random_number_generator())));
      }
    },
    "Default restarting"
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
  constexpr std::size_t number_of_dimensions = 3;
  const mant::optimisation_algorithm_state<value_type, number_of_dimensions> optimisation_algorithm_state;
  
  CHECK(optimisation_algorithm_state.parameters.size() == 1);
  CHECK(optimisation_algorithm_state.objective_values.empty() == true);
  // Checks that all elements are within [0, 1].
  for (const auto& parameter : optimisation_algorithm_state.parameters) {
    CHECK(std::all_of(
      parameter.cbegin(), parameter.end(),
      [](const auto element) {
        return (0.0 <= element && element <= 1.0);
      }) == true);
  }
  CHECK(optimisation_algorithm_state.best_found_objective_value == std::numeric_limits<value_type>::infinity());
  CHECK(optimisation_algorithm_state.used_number_of_iterations == 0);
  CHECK(optimisation_algorithm_state.stagnating_number_of_iterations == 0);
}

TEST_CASE("optimisation_algorithm", "[optimisation_algorithm]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  mant::optimisation_algorithm<value_type, number_of_dimensions> optimisation_algorithm;
  mant::optimisation_algorithm_state<value_type, number_of_dimensions> optimisation_algorithm_state;
  
  SECTION("Default values") {
    CHECK(optimisation_algorithm.acceptable_objective_value == -::std::numeric_limits<double>::infinity());
    CHECK(optimisation_algorithm.maximal_number_of_iterations == 10000);
    CHECK(optimisation_algorithm.maximal_stagnating_number_of_iterations == 100);
    CHECK(optimisation_algorithm.active_dimensions == std::vector<::std::size_t>({0, 1, 2}));
  }
  
  SECTION("Initialising functions") {
    CHECK(optimisation_algorithm.initialising_functions.size() == 0);
  }
  
  SECTION("Boundary handling functions") {
    CHECK(optimisation_algorithm.boundary_handling_functions.size() == 1);
    CHECK(std::get<1>(optimisation_algorithm.boundary_handling_functions.at(0)) == "Default boundary handling");
    
    optimisation_algorithm.active_dimensions = {0, 2};
    optimisation_algorithm_state.parameters = {{-0.1, 0.2, 3.2}, {0.8, 1.2, -2.4}};
    
    std::get<0>(optimisation_algorithm.boundary_handling_functions.at(0))(optimisation_algorithm_state);
    
    CHECK((optimisation_algorithm_state.parameters == std::vector<std::array<value_type, number_of_dimensions>>({{0.0, 0.2, 3.2}, {0.8, 1.0, -2.4}})));
  }
  
  
  SECTION("Is-stagnating functions") {
    optimisation_algorithm.active_dimensions = {0, 2};
  
    CHECK(optimisation_algorithm.is_stagnating_functions.size() == 1);
    CHECK(std::get<1>(optimisation_algorithm.is_stagnating_functions.at(0)) == "Default is-stagnating");
    
    optimisation_algorithm_state.stagnating_number_of_iterations = 0;
    
    CHECK(std::get<0>(optimisation_algorithm.is_stagnating_functions.at(0))(optimisation_algorithm_state) == false);
    
    optimisation_algorithm_state.stagnating_number_of_iterations = 101;
    
    CHECK(std::get<0>(optimisation_algorithm.is_stagnating_functions.at(0))(optimisation_algorithm_state) == true);
  }
  
  SECTION("Next parameters functions") {
    CHECK(optimisation_algorithm.next_parameters_functions.size() == 0);
  }
  
  SECTION("Restarting functions") {
    CHECK(optimisation_algorithm.restarting_functions.size() == 1);
    CHECK(std::get<1>(optimisation_algorithm.restarting_functions.at(0)) == "Default restarting");
    
    optimisation_algorithm.active_dimensions = {0, 2};
    optimisation_algorithm_state.parameters.resize(2);
    
    std::get<0>(optimisation_algorithm.restarting_functions.at(0))(optimisation_algorithm_state);
    
    CHECK(optimisation_algorithm_state.parameters.size() == 2);
    // Checks that all elements are within [0, 1].
    for (const auto& parameter : optimisation_algorithm_state.parameters) {
      CHECK(std::all_of(
        parameter.cbegin(), std::next(parameter.cbegin(), optimisation_algorithm.active_dimensions.size()),
        [](const auto element) {
          return (0.0 <= element && element <= 1.0);
        }) == true);
    }
  }
}
#endif
