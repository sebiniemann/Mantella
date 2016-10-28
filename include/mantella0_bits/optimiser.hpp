/**

*/
template <typename T, unsigned N>
struct optimise_result {
  std::array<T, N> best_parameter;
  T best_objective_value;
  unsigned evaluations;
  std::chrono::nanoseconds duration;
  
  constexpr optimise_result() noexcept;
};

template <typename T1, unsigned N, template <class, unsigned> class T2>
struct optimiser {
  std::function<optimise_result<T1, N>(
      const T2<T1, N>& problem,
      const std::vector<std::array<T1, N>>& initial_parameters)> optimisation_function;
  
  T1 acceptable_objective_value;
  unsigned maximal_evaluations;
  std::chrono::nanoseconds maximal_duration;
  
  std::vector<unsigned> active_dimensions;
  
  optimiser() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
constexpr optimise_result<T, N>::optimise_result() noexcept
  : best_parameter{},
    best_objective_value(std::numeric_limits<T>::infinity()),
    evaluations(0),
    duration(std::chrono::nanoseconds(0)) {

};

template <typename T1, unsigned N, template <class, unsigned> class T2>
optimiser<T1, N, T2>::optimiser() noexcept 
  : acceptable_objective_value(-std::numeric_limits<T1>::infinity()),
    maximal_evaluations(1'000 * N),
    maximal_duration(std::chrono::seconds(10)) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(N > 0, "");
  static_assert(std::is_base_of<problem<T1, N>, T2<T1, N>>::value, "");
  
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
  CHECK(result.evaluations == 0);
  CHECK(result.duration == std::chrono::nanoseconds(0));
}

TEST_CASE("optimiser", "[optimiser]") {
  const mant::optimiser<double, 3, mant::problem> optimiser;
  
  CHECK(optimiser.acceptable_objective_value == -std::numeric_limits<double>::infinity());
  CHECK(optimiser.maximal_evaluations == 3000);
  CHECK(optimiser.maximal_duration == std::chrono::seconds(10));
  CHECK(optimiser.active_dimensions == std::vector<unsigned>({0, 1, 2}));
}
#endif
