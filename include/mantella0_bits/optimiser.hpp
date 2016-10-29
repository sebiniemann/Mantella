/**

*/
template <typename T, std::size_t N>
struct optimise_result {
  std::array<T, N> best_parameter;
  T best_objective_value;
  std::size_t number_of_evaluations;
  std::chrono::nanoseconds duration;
  
  constexpr optimise_result() noexcept;
};

template <typename T, std::size_t N>
struct optimiser {
  std::function<optimise_result<T, N>(
      const mant::problem<T, N>& problem,
      const std::vector<std::array<T, N>>& initial_parameters)> optimisation_function;
  
  T acceptable_objective_value;
  std::size_t maximal_number_of_evaluations;
  std::chrono::nanoseconds maximal_duration;
  
  std::vector<std::size_t> active_dimensions;
  
  optimiser() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
constexpr optimise_result<T, N>::optimise_result() noexcept
  : best_parameter{},
    best_objective_value(std::numeric_limits<T>::infinity()),
    number_of_evaluations(0),
    duration(std::chrono::nanoseconds(0)) {

};

template <typename T, std::size_t N>
optimiser<T, N>::optimiser() noexcept 
  : acceptable_objective_value(-std::numeric_limits<T>::infinity()),
    maximal_number_of_evaluations(1000 * N),
    maximal_duration(std::chrono::seconds(10)) {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(N > 0, "");
  
  active_dimensions.resize(N);
  std::iota(active_dimensions.begin(), active_dimensions.end(), 0);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise_result", "[optimiser][optimise_result]") {
  constexpr mant::optimise_result<double, 3> result;
  
  CHECK(result.best_objective_value == std::numeric_limits<double>::infinity());
  CHECK(result.number_of_evaluations == 0);
  CHECK(result.duration == std::chrono::nanoseconds(0));
}

TEST_CASE("optimiser", "[optimiser]") {
  const mant::optimiser<double, 3> optimiser;
  
  CHECK(optimiser.acceptable_objective_value == -std::numeric_limits<double>::infinity());
  CHECK(optimiser.maximal_number_of_evaluations == 3000);
  CHECK(optimiser.maximal_duration == std::chrono::seconds(10));
  CHECK(optimiser.active_dimensions == std::vector<std::size_t>({0, 1, 2}));
}
#endif
