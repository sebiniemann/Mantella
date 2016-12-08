/**

Optimisation result
===================

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
struct optimise_result {
  std::array<T, N> parameter{};
  T objective_value = std::numeric_limits<T>::infinity();
  std::size_t evaluations = 0;
  std::chrono::nanoseconds duration = std::chrono::nanoseconds(0);
};

//
// Implementation
//

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise_result", "[optimiser][optimise_result]") {
  constexpr mant::optimise_result<double, 3> result{};

  CHECK(result.objective_value == std::numeric_limits<double>::infinity());
  CHECK(result.evaluations == 0);
  CHECK(result.duration == std::chrono::nanoseconds(0));
}
#endif
