/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
std::array<T, number_of_dimensions> random_neighbour(
    const std::array<T, number_of_dimensions>& parameter,
    const T minimal_distance,
    const T maximal_distance,
    const std::size_t active_number_of_dimensions);

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
std::array<T, number_of_dimensions> random_neighbour(
    const std::array<T, number_of_dimensions>& parameter,
    const T minimal_distance,
    const T maximal_distance,
    const std::size_t active_number_of_dimensions) {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  assert(T(0.0) <= minimal_distance && minimal_distance <= maximal_distance);
  assert(0 < active_number_of_dimensions <= number_of_dimensions);

  /* @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the 
   * surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
   *
   * In summary, it works as followed:
   * 1. Uniformly draw an *n*-dimensional direction vector (by normalising a normal distribution vector).
   * 2. Multiply it with a length, uniformly drawn from [*minimal_distance*, *maximal_distance*].
   * 3. Translate its origin by adding *parameter*.
   */
  std::array<T, number_of_dimensions> neighbour;
  std::generate(
    neighbour.begin(), std::next(neighbour.begin(), active_number_of_dimensions),
    std::bind(std::normal_distribution<T>(), std::ref(random_number_generator())));

  // Instead of iterating twice through the vector (normalising it first and scaling it afterwards), we do everything in
  // one step and normalise the length accordingly.
  const T length = (
      minimal_distance + 
      std::uniform_real_distribution<T>(T(0.0), T(1.0))(random_number_generator()) * (
        maximal_distance - minimal_distance
      )
    ) / 
    std::sqrt(std::inner_product(
      neighbour.cbegin(), std::next(neighbour.cbegin(), active_number_of_dimensions),
      neighbour.cbegin(),
      T(0.0)
    ));

  std::transform(
    neighbour.cbegin(), std::next(neighbour.cbegin(), active_number_of_dimensions),
    parameter.cbegin(), 
    neighbour.begin(), 
    [length] (const auto neighbour_element, const auto parameter_element) {
      return parameter_element + neighbour_element * length;
    });

  return neighbour;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_neighbour", "[randomisation][random_neighbour]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  
  // Checks that the neighbour's distance from the parameter is within [*maximal_distance*, *minimal_distance*].
  constexpr std::array<value_type, number_of_dimensions> parameter = {1.0, -2.0, 3.0};
  constexpr value_type minimal_distance = 1.0;
  constexpr value_type maximal_distance = 3.0;
  constexpr std::size_t active_number_of_dimensions = 2;
  auto&& neighbour = mant::random_neighbour(parameter, minimal_distance, maximal_distance, active_number_of_dimensions);
  auto&& distance = std::sqrt(std::inner_product(
    neighbour.cbegin(), std::next(neighbour.cbegin(), active_number_of_dimensions),
    parameter.cbegin(),
    0.0,
    std::plus<double>(),
    [](const auto neighbour_element, const auto parameter_element) {
      return std::pow(neighbour_element - parameter_element, 2.0);
    }));
  CHECK(distance >= minimal_distance);
  CHECK(distance <= maximal_distance);
}
#endif
