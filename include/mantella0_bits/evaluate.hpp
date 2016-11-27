/**

evaluate
========

.. cpp:class:: optimise_result

  .. versionadded:: 1.0.0

  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.

  Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.

  .. list-table:: Template parameters

    * - T

        Any floating point type
      - The value type of the parameter and objective value.
    * - N

        ``std::size_t``
      - The number of dimensions.

        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.

  .. list-table:: Member variables

    * - parameter

        ``std::array<T, N>``
      - Lorem ipsum dolor sit amet

    * - objective_value

        ``T``
      - Lorem ipsum dolor sit amet

    * - evaluations

        ``std::size_t``
      - Lorem ipsum dolor sit amet

    * - duration

        ``std::chrono::nanoseconds``
      - Lorem ipsum dolor sit amet
*/

template <typename T, std::size_t N>
T evaluate(
    const problem& problem,
    const std::array<T, N>& parameter);

template <typename T>
std::vector<T> evaluate(
    const problem& problem,
    const std::vector<std::array<T, N>>& parameters);

//
// Implementation
//

template <typename T, std::size_t N>
T evaluate(
    const problem& problem,
    const std::array<T, N>& parameter) {
  return problem.objective_function(parameter);
}

template <typename T>
std::vector<T> evaluate(
    const problem& problem,
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

}
#endif
