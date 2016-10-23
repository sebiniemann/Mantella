template <typename T1, std::size_t N, template <class, std::size_t> class T2, typename T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3& optimiser,
    std::vector<std::array<T1, N>> initial_parameters);

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2, typename T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3& optimiser,
    std::vector<std::array<T1, N>> initial_parameters) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(N > 0, "");
  static_assert(std::is_base_of<mant::problem<T1, N>, T2<T1, N>>::value, "");
  // static_assert(std::is_base_of<mant::optimiser<T1, N, T2>, T3<T1, N, T2<T1, N>>>::value, "");
  
  assert(initial_parameters.size() > 0);
  
  // Maps the parameter's bounds from [*problem.lower_bounds*, *problem.upper_bounds*] to [0, 1] and places all active 
  // dimensions (in-order) upfront.
  for (auto& parameter : initial_parameters) {
    for (std::size_t n = 0; n < optimiser.active_dimensions.size(); ++n) {
      parameter.at(n) = (
          parameter.at(optimiser.active_dimensions.at(n)) - 
          problem.lower_bounds.at(n)
        ) / (problem.upper_bounds.at(n) - problem.lower_bounds.at(n));
    }
  }
  
  mant::problem<T1, N> mapped_problem;
  mapped_problem.objective_function = [&problem, &optimiser](const auto& parameter) {
    std::array<T1, N> mapped_parameter = problem.lower_bounds;
    for (std::size_t n = optimiser.active_dimensions.size(); n > 0; --n) {
      mapped_parameter.at(optimiser.active_dimensions.at(n - 1)) = 
        problem.lower_bounds.at(n - 1) +
        parameter.at(n - 1) * (
          problem.upper_bounds.at(n - 1) - problem.lower_bounds.at(n - 1)
        );
    }
    
    return problem.objective_function(mapped_parameter);
  };
  
  auto&& result = optimiser.optimisation_function(mapped_problem, initial_parameters);
  
  // Maps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*], permutes the parameter to match 
  // the active dimensions.
  for (std::size_t n = optimiser.active_dimensions.size(); n > 0; --n) {
    result.best_parameter.at(optimiser.active_dimensions.at(n - 1)) = 
      problem.lower_bounds.at(n - 1) +
      result.best_parameter.at(n - 1) * (
        problem.upper_bounds.at(n - 1) - problem.lower_bounds.at(n - 1)
      );
  }

  return result;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise", "[optimise]") {
  mant::sphere_function<double, 2> problem;
  problem.lower_bounds = {-5.0, -5.0};
  problem.upper_bounds = {5.0, 5.0};
  
  mant::hooke_jeeves_algorithm<double, 2, mant::problem> optimiser;
  optimiser.acceptable_objective_value = 1e-12;
  
  const auto&& result = mant::optimise(problem, optimiser, {{-3.2, 4.1}});
  CHECK((result.best_parameter == std::array<double, 2>({-6.67572022550189104e-07, 3.33786010386916132e-07})));
  CHECK(result.best_objective_value == Approx(5.57065506021764692e-13));
  CHECK(result.number_of_evaluations == 40);
}
#endif
