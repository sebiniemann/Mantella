/**
evaluate
========

.. cpp:function:: evaluate(problem, parameters)

  .. versionadded:: 1.0.0

  This function evaluates all given parameters on a problem, and returns the results as vector, in the same order.

  ``evaluate`` is used by optimisers to calculate all objective values of a single step. It is part of Mantellas public API, so you are encouraged to use it when implementing your own optimisers. In addition to saving a loop, it will use the STLs automatic parallelism, once the C++17 standard is released supported by your compiler.

  .. list-table:: Template parameters

    * - T

        Any floating point type
      - The value type of the parameter and objective value.
    * - N

        ``std::size_t``
      - The number of dimensions.

        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.

  .. list-table:: function parameters

    * - problem

        ``mant::problem<T, N>``
      - The problem to evaluate the parameters.
    * - parameters

        ``std::vector<std::array<T, N>>``
      - The list of parameters that will be evaluated on `problem`.

  .. list-table:: Returns

    * - objective_values

        ``std::vector<T>``
      - A vector with the same length as ``parameters``. Each index stores the objective value for the corresponding parameter.
*/

template <typename T, std::size_t N>
T evaluate(
    const problem<T, N>& problem,
    const std::array<T, N>& parameter);

template <typename T, std::size_t N>
std::vector<T> evaluate(
    const problem<T, N>& problem,
    const std::vector<std::array<T, N>>& parameters);

//
// Implementation
//

template <typename T, std::size_t N>
T evaluate(
    const problem<T, N>& problem,
    const std::array<T, N>& parameter) {
  return problem.objective_function(parameter);
}

template <typename T, std::size_t N>
std::vector<T> evaluate(
    const problem<T, N>& problem,
    const std::vector<std::array<T, N>>& parameters) {
  std::vector<T> objective_values(parameters.size());
  std::transform(
    parameters.cbegin(), parameters.cend(),
    objective_values.begin(),
    [&problem](const auto& parameter) {
      return problem.objective_function(parameter);
    });

  return objective_values;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("evaluate", "[evaluate]") {
  mant::problem<double, 2> problem;
  problem.objective_function = [](const auto& parameter) {
    return parameter.at(0) * parameter.at(1);
  };

  CHECK((evaluate(problem, {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}}) == std::vector<double>{2.0, 12.0, 30.0}));
  CHECK(evaluate(problem, {3.0, 7.0}) == problem.objective_function({3.0, 7.0}));
}
#endif
