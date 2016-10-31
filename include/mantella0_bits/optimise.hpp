/**
Optimisation
============

.. cpp:function:: template<T1, N, T2, T3> optimise(problem, optimiser, initial_parameters)

  .. list-table:: Shortcuts
    :widths: 27 73
    
    * - optimise(problem, optimiser)
        
        
      - Calls :cpp:any:`optimise` ``(problem, optimiser, initial_parameters)``.
      
        The number of initial parameters will be
      
        - ... ``N + 1`` if the optimiser is :cpp:any:`nelder_mead_method` ...
        - ... ``10 * N`` if the optimiser is :cpp:any:`particle_swarm_optimisation` ...
        - ... ``1`` for all other optimisers.
        
        Each parameter is randomly drawn from ``[0, 1]``.
    * - optimise(problem)
        
        
      - Calls :cpp:any:`optimise` ``(problem, optimiser)``.
      
        Uses :cpp:any:`hooke_jeeves_algorithm` as optimiser.

  **Template parameters**
  
    - **T1** - A floating point type
    - **N** - The (``unsigned``) number of dimensions 
    - **T2** - A type derived from ``problem<T1, N>``
    - **T3** - A type derived from ``optimiser<T1, N>``
  
  **Function parameters**
  
    - **problem** (``T2``)
    
      - The problem to be optimised.
      - The problem's boundaries will be remapped to ``[0, 1]``. However, the provided problem remains unchanged.
  
    - **optimiser** (``T3``)
    
      - The optimiser to be used to solve the problem.
      - The optimiser is assumed to 
  
    - **initial_parameters** (``T1``)
    
      - The initial parameters
    
  **Return** (``optimise_result``)
    
      - Lorem ipsum dolor sit amet
*/
template <typename T1, unsigned N, template <class, unsigned> class T2, template <class, unsigned> class T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3<T1, N>& optimiser,
    std::vector<std::array<T1, N>> initial_parameters);

template <typename T1, unsigned N, template <class, unsigned> class T2, template <class, unsigned> class T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3<T1, N>& optimiser);
    
template <typename T1, unsigned N, template <class, unsigned> class T2>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem);
//
// Implementation
//

template <typename T1, unsigned N, template <class, unsigned> class T2, template <class, unsigned> class T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3<T1, N>& optimiser,
    std::vector<std::array<T1, N>> initial_parameters) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(N > 0, "");
  static_assert(std::is_base_of<mant::problem<T1, N>, T2<T1, N>>::value, "");
  static_assert(std::is_base_of<mant::optimiser<T1, N>, T3<T1, N>>::value, "");
  
  assert(static_cast<bool>(problem.objective_function));
  assert(static_cast<bool>(optimiser.optimisation_function));
  assert(initial_parameters.size() > 0);
  
  // Maps the parameter's bounds from [*problem.lower_bounds*, *problem.upper_bounds*] to [0, 1] and places all active 
  // dimensions (in-order) upfront.
  for (auto& parameter : initial_parameters) {
    for (unsigned n = 0; n < optimiser.active_dimensions.size(); ++n) {
      parameter.at(n) = (
          parameter.at(optimiser.active_dimensions.at(n)) - 
          problem.lower_bounds.at(n)
        ) / (problem.upper_bounds.at(n) - problem.lower_bounds.at(n));
    }
  }
  
  mant::problem<T1, N> mapped_problem;
  mapped_problem.objective_function = [&problem, &optimiser](const auto& parameter) {
    std::array<T1, N> mapped_parameter = problem.lower_bounds;
    for (unsigned n = optimiser.active_dimensions.size(); n > 0; --n) {
      mapped_parameter.at(optimiser.active_dimensions.at(n - 1)) = 
        problem.lower_bounds.at(n - 1) +
        parameter.at(n - 1) * (
          problem.upper_bounds.at(n - 1) - problem.lower_bounds.at(n - 1)
        );
    }
    
    return problem.objective_function(mapped_parameter);
  };
  assert(std::all_of(
    mapped_problem.lower_bounds.cbegin(), std::next(mapped_problem.lower_bounds.cbegin(), optimiser.active_dimensions.size()),
    std::bind(std::equal_to<T1>{}, std::placeholders::_1, T1(0.0))
  ));
  assert(std::all_of(
    mapped_problem.upper_bounds.cbegin(), std::next(mapped_problem.upper_bounds.cbegin(), optimiser.active_dimensions.size()),
    std::bind(std::equal_to<T1>{}, std::placeholders::_1, T1(1.0))
  ));
  
  auto&& result = optimiser.optimisation_function(mapped_problem, initial_parameters);
  
  // Maps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*], permutes the parameter to match 
  // the active dimensions.
  for (unsigned n = optimiser.active_dimensions.size(); n > 0; --n) {
    result.parameter.at(optimiser.active_dimensions.at(n - 1)) = 
      problem.lower_bounds.at(n - 1) +
      result.parameter.at(n - 1) * (
        problem.upper_bounds.at(n - 1) - problem.lower_bounds.at(n - 1)
      );
  }

  return result;
}

template <typename T1, unsigned N, template <class, unsigned> class T2, template <class, unsigned> class T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3<T1, N>& optimiser) {
  std::vector<std::array<T1, N>> initial_parameters;
  if (std::is_base_of<nelder_mead_method<T1, N>, T3<T1, N>>::value) {
    initial_parameters.resize(N + 1);
  } else if (std::is_base_of<particle_swarm_optimisation<T1, N>, T3<T1, N>>::value) {
    initial_parameters.resize(10 * N);
  } else {
    initial_parameters.resize(1);
  }
  
  for (auto& parameter : initial_parameters) {
    std::generate(
      parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
      std::bind(
        std::uniform_real_distribution<T1>(0.0, 1.0),
        std::ref(random_number_generator())));
  }
  
  return optimise(problem, optimiser, initial_parameters);
}

template <typename T1, unsigned N, template <class, unsigned> class T2>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem) {
  return optimise(problem, hooke_jeeves_algorithm<T1, N>());
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise", "[optimise]") {
  mant::sphere_function<double, 2> problem;
  problem.lower_bounds = {-5.0, -5.0};
  problem.upper_bounds = {5.0, 5.0};
  
  mant::hooke_jeeves_algorithm<double, 2> optimiser;
  optimiser.acceptable_objective_value = 1e-12;
  
  const auto&& result = mant::optimise(problem, optimiser, {{-3.2, 4.1}});
  CHECK((result.parameter == std::array<double, 2>({0.50000004768371475, 0.49999997019767761})));
  CHECK(result.objective_value == Approx(5.57065506021764692e-13));
  CHECK(result.evaluations == 189);
  
  CHECK_NOTHROW(mant::optimise(problem, optimiser));
  CHECK_NOTHROW(mant::optimise(problem));
}
#endif
