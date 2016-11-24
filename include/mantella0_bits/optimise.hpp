/**
optimisation
============

.. cpp:function:: optimise(problem, optimiser, initial_parameters)

  .. versionadded:: 1.0.0

  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.

  Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.

  .. list-table:: Shortcuts

    * - optimise(problem, optimiser)

      - Calls ``optimise(problem, optimiser, initial_parameters)``.

        The number of initial parameters will be

        - ... ``N + 1`` if the optimiser is :cpp:any:`nelder_mead_method` ...
        - ... ``10 * N`` if the optimiser is :cpp:any:`particle_swarm_optimisation` ...
        - ... ``1`` for all other optimisers.

        Each parameter is randomly drawn from ``[problem.lower_bounds, problem.upper_bounds]``.
    * - optimise(problem)

      - Calls ``optimise(problem, optimiser)``.

        Uses :cpp:any:`hooke_jeeves_algorithm` as optimiser.

  .. list-table:: Template parameters

    * - T

        Any floating point type
      - The value type of the parameter and objective value.
    * - N

        ``std::size_t``
      - The number of dimensions.

        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.

  .. list-table:: Function functions

    * - problem

        ``T2``
      - The problem's boundaries will be remapped to ``[0, 1]``. However, the provided problem remains unchanged.
    * - optimiser

        ``T3``
      - Lorem ipsum dolor sit amet
    * - initial_parameters

        ``std::vector<std::array<T1, N>>``
      - Lorem ipsum dolor sit amet

  .. list-table:: Returns

    * - ``optimise_result``
      - Lorem ipsum dolor sit amet
*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2, template <class, std::size_t> class T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3<T1, N>& optimiser,
    std::vector<std::array<T1, N>> initial_parameters);

template <typename T1, std::size_t N, template <class, std::size_t> class T2, template <class, std::size_t> class T3>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem,
    const T3<T1, N>& optimiser);

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem);

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2, template <class, std::size_t> class T3>
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

  // Maps the parameter's bounds from [*problem.lower_bounds*, *problem.upper_bounds*] to [0, 1] and places all active dimensions (in-order) upfront.
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
  assert(std::all_of(
    mapped_problem.lower_bounds.cbegin(), std::next(mapped_problem.lower_bounds.cbegin(), optimiser.active_dimensions.size()),
    std::bind(std::equal_to<T1>{}, std::placeholders::_1, T1(0.0))
  ));
  assert(std::all_of(
    mapped_problem.upper_bounds.cbegin(), std::next(mapped_problem.upper_bounds.cbegin(), optimiser.active_dimensions.size()),
    std::bind(std::equal_to<T1>{}, std::placeholders::_1, T1(1.0))
  ));

  auto&& result = optimiser.optimisation_function(mapped_problem, initial_parameters);

  // Remaps the parameter's bounds back from [0, 1] to [*lower_bounds*, *upper_bounds*] and permutes the parameter to match the active dimensions.
  for (std::size_t n = optimiser.active_dimensions.size(); n > 0; --n) {
    result.parameter.at(optimiser.active_dimensions.at(n - 1)) =
      problem.lower_bounds.at(n - 1) +
      result.parameter.at(n - 1) * (
        problem.upper_bounds.at(n - 1) - problem.lower_bounds.at(n - 1)
      );
  }

  return result;
}

template <typename T1, std::size_t N, template <class, std::size_t> class T2, template <class, std::size_t> class T3>
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

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
optimise_result<T1, N> optimise(
    const T2<T1, N>& problem) {
  return optimise(problem, hooke_jeeves_algorithm<T1, N>());
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise", "[optimise]") {
  mant::sphere_function<double, 2u> problem;
  problem.lower_bounds = {-5.0, -5.0};
  problem.upper_bounds = {5.0, 5.0};

  mant::hooke_jeeves_algorithm<double, 2u> optimiser;
  optimiser.acceptable_objective_value = 1e-12;

  const auto&& result = mant::optimise(problem, optimiser, {{-3.2, 4.1}});
  CHECK((result.parameter == std::array<double, 2u>({-6.675720225501891e-07, 0.0})));
  CHECK(result.objective_value == Approx(5.57065506021764692e-13));
  CHECK(result.evaluations == 137);

  CHECK_NOTHROW(mant::optimise(problem, optimiser));
  CHECK_NOTHROW(mant::optimise(problem));
}
#endif
