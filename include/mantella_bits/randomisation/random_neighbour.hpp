#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
std::array<T, number_of_dimensions> random_neighbour(
    const std::array<T, number_of_dimensions>& parameter,
    const T minimal_distance,
    const T maximal_distance,
    const std::size_t active_number_of_dimensions);

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
std::array<T, number_of_dimensions> random_neighbour(
    const std::array<T, number_of_dimensions>& parameter,
    const T minimal_distance,
    const T maximal_distance,
    const std::size_t active_number_of_dimensions) {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  assert(T(0.0) <= minimal_distance && minimal_distance <= maximal_distance);
  assert(0 < active_number_of_dimensions <= number_of_dimensions);
  
  /* @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
   *
   * In summary, it works as followed:
   * 1. Uniformly draw an *n*-dimensional direction vector (by normalising a normal distribution vector).
   * 2. Multiply it with a length, uniformly drawn from [*minimal_distance*, *maximal_distance*].
   * 3. Translate its origin by adding *parameter*.
   */
  std::array<T, number_of_dimensions> neighbour;
  std::generate(
    neighbour.begin(),
    std::next(neighbour.begin(), active_number_of_dimensions),
    std::bind(std::normal_distribution<T>(), std::ref(random_number_generator())));
  
  // Instead of iterating twice through the vector (normalising it first and scaling it afterwards), we do everything in one step and normalise the length accordingly.
  const T length = (minimal_distance + std::uniform_real_distribution<T>(T(0.0), T(1.0))(random_number_generator()) * (maximal_distance - minimal_distance)) / cblas_dnrm2(active_number_of_dimensions, neighbour.data(), 1);
  
  for (std::size_t n = 0; n < active_number_of_dimensions; ++n) {
    neighbour.at(n) = parameter.at(n) + neighbour.at(n) * length;
  }
  
  return neighbour;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_neighbour", "[randomisation][random_neighbour]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  
  // Checks that the neighbour is uniformly drawn from a (partly hollow) sphere with outer radius *maximal_distance* and inner distance *minimal_distance*.
  
  // Repeats the process a few times to generate enough samples to test its statistical properties.
  constexpr std::size_t number_of_repetitions = 1000;
  std::array<double, number_of_repetitions> lengths;
  std::array<double, number_of_repetitions> angles;
  
  // The parameters to be used in each repetition.
  constexpr std::array<value_type, number_of_dimensions> parameter = {1.0, -2.0, 3.0};
  constexpr value_type minimal_distance = 1.0;
  constexpr value_type maximal_distance = 3.0;
  constexpr std::size_t active_number_of_dimensions = 2;
  
  for (std::size_t n = 0; n < number_of_repetitions; ++n) {
    auto&& neighbour = mant::random_neighbour(parameter, minimal_distance, maximal_distance, active_number_of_dimensions);
    // Transforms the neighbour's origin from [0, ..., 0] to *parameter*.
    neighbour = {std::get<0>(neighbour) - std::get<0>(parameter), std::get<1>(neighbour) - std::get<1>(parameter)};
    
    // Normalises all lengths and angles to be within [0, 1].
    // This eases the later on histogram generation.
    lengths.at(n) = (cblas_dnrm2(neighbour.size(), neighbour.data(), 1) - minimal_distance) / (maximal_distance - minimal_distance);
    angles.at(n) = (std::atan2(std::get<1>(neighbour), std::get<0>(neighbour)) + 3.141592653589793) / 6.283185307179586;
  }
  
  // Performs an entropy-based uniformity test.
  constexpr std::size_t number_of_bins = 10;
  std::array<double, number_of_bins> histogram;
  
  // Checks the lengths
  histogram.fill(0.0);
  for (const auto length : lengths) {
    // Ensures that the index will never be out-of-range (this can only happen if *length* is 1).
    histogram.at(std::min<std::size_t>(length * number_of_bins, number_of_bins - 1)) += 1.0 / static_cast<double>(number_of_repetitions);
  }
  CHECK(std::accumulate(
    histogram.cbegin(),
    histogram.cend(),
    0.0,
    [](
        const double entropy,
        const double element) {
      return entropy + -std::log(element) * element;
    }) > std::log(number_of_bins) * 0.9);
  
  // Checks the angles
  histogram.fill(0.0);
  for (const auto angle : angles) {
    // Ensures that the index will never be out-of-range (this can only happen if *angle* is 1).
    histogram.at(std::min<std::size_t>(angle * number_of_bins, number_of_bins - 1)) += 1.0 / static_cast<double>(number_of_repetitions);
  }
  CHECK(std::accumulate(
    histogram.cbegin(),
    histogram.cend(),
    0.0,
    [](
        const double entropy,
        const double element) {
      return entropy + -std::log(element) * element;
    }) > std::log(number_of_bins) * 0.9);
}
#endif