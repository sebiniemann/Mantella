#pragma once

/**

*/
template <typename T, std::size_t number_of_elements>
std::array<T, number_of_elements> random_neighbour(
    const std::array<T, number_of_elements>& parameter,
    const T minimal_distance,
    const T maximal_distance);

//
// Implementation
//

template <typename T, std::size_t number_of_elements>
std::array<T, number_of_elements> random_neighbour(
    const std::array<T, number_of_elements>& parameter,
    const T minimal_distance,
    const T maximal_distance) {
  static_assert(std::is_floating_point<T>::value, "The type for the parameter's elements must be a floating point.");
  assert(minimal_distance >= 0 && "random_neighbour: The minimal distance must be positive (including 0).");
  assert(minimal_distance <= maximal_distance && "random_neighbour: The minimal distance must be less than or equal to the maximal one.");
  
  /* @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
   * In summary, 
   * - an *n*-dimensional direction is uniformly drawn (by normalising a vector filled by drawing from a normal distribution),
   * - scaled by a distance uniformly drawn from [*minimal_distance*, *maximal_distance*] and
   * - translated by *parameter*.
   */
  std::array<T, number_of_elements> neighbour;
  std::generate(neighbour.begin(), neighbour.end(), std::bind(std::normal_distribution<T>(), std::ref(random_number_generator())));
  const T scaled_length = (minimal_distance + std::uniform_real_distribution<T>(0, 1)(random_number_generator()) * (maximal_distance - minimal_distance)) / cblas_dnrm2(number_of_elements, neighbour.data(), 1);
  
  for (std::size_t n = 0; n < number_of_elements; ++n) {
    neighbour[n] = parameter[n] + neighbour[n] * scaled_length;
  }
  
  return neighbour;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_neighbour", "[randomisation][random_neighbour]") {
  constexpr std::size_t number_of_repetitions = 10000;
  constexpr std::array<double, 2> parameter = {1.0, -2.0};
  constexpr double minimal_distance = 1.0;
  constexpr double maximal_distance = 3.0;
  
  std::array<double, number_of_repetitions> lengths;
  std::array<double, number_of_repetitions> angles;
  for (std::size_t n = 0; n < number_of_repetitions; ++n) {
    auto neighbour = mant::random_neighbour(parameter, minimal_distance, maximal_distance);
    // Translate *neighbour* to be starting from the origin.
    neighbour = {neighbour[0] - parameter[0], neighbour[1] - parameter[1]};
    
    // Normalises all lengths and angles to be within [0, 1].
    lengths[n] = (cblas_dnrm2(neighbour.size(), neighbour.data(), 1) - minimal_distance) / (maximal_distance - minimal_distance);
    CHECK((0.0 <= lengths[n] && lengths[n] <= 1.0));
    
    angles[n] = (std::atan2(neighbour[1], neighbour[0]) + 3.141592653589793) / 6.283185307179586;
    CHECK((0.0 <= angles[n] && angles[n] <= 1.0));
  }
  
  // Performs an entropy-based uniformity test.
  constexpr std::size_t number_of_bins = 10;
  std::array<double, number_of_bins> histogram;
  histogram.fill(0.0);
  for (const auto length : lengths) {
    // Ensures that the index will never be out-of-range (esp. in case *length* is 1).
    histogram[std::min<std::size_t>(length * number_of_bins, number_of_bins - 1)] += 1.0 / static_cast<double>(number_of_repetitions);
  }
  CHECK(std::accumulate(histogram.cbegin(), histogram.cend(), 0.0, [](const double entropy, const double element) {return entropy + -std::log(element) * element;}) > std::log(number_of_bins) * 0.9);
  
  histogram.fill(0.0);
  for (const auto angle : angles) {
    // Ensures that the index will never be out-of-range (esp. in case *angle* is 1).
    histogram[std::min<std::size_t>(angle * number_of_bins, number_of_bins - 1)] += 1.0 / static_cast<double>(number_of_repetitions);
  }
  CHECK(std::accumulate(histogram.cbegin(), histogram.cend(), 0.0, [](const double entropy, const double element) {return entropy + -std::log(element) * element;}) > std::log(number_of_bins) * 0.9);
}
#endif