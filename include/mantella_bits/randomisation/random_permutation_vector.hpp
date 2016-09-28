#pragma once

/**

*/
template <std::size_t number_of_elements>
std::array<std::size_t, number_of_elements> random_permutation_vector();

//
// Implementation
//
template <std::size_t number_of_elements>
std::array<std::size_t, number_of_elements> random_permutation_vector() {
  std::array<std::size_t, number_of_elements> permutation;
  std::iota(permutation.begin(), permutation.end(), 0);
  std::shuffle(permutation.begin(), permutation.end(), random_number_generator());
  return permutation;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_permutation_vector", "[randomisation][random_permutation_vector]") {
  constexpr std::size_t number_of_elements = 10;
  constexpr std::size_t number_of_repetitions = 10000;
  
  std::array<std::array<std::size_t, number_of_repetitions>, number_of_elements> permutations;
  for (std::size_t n = 0; n < number_of_repetitions; ++n) {
    const auto permutation = mant::random_permutation_vector<number_of_elements>();
    CHECK((mant::is_permutation_vector<std::size_t, number_of_elements>(permutation) == true));
    
    for (std::size_t k = 0; k < number_of_elements; ++k) {
      permutations[k][n] = permutation[k];
    }
  }
  
  // Performs an entropy-based uniformity test.
  constexpr std::size_t number_of_bins = 10;
  std::array<double, number_of_bins> histogram;
  for (std::size_t n = 0; n < number_of_elements; ++n) {
    histogram.fill(0.0);
    for (const auto permutation : permutations[n]) {
      // Ensures that the index will never be out-of-range (esp. in case *length* is 1).
      histogram[permutation] += 1.0 / static_cast<double>(number_of_repetitions);
    }
    CHECK(std::accumulate(histogram.cbegin(), histogram.cend(), 0.0, [](const double entropy, const double element) {return entropy + -std::log(element) * element;}) > std::log(number_of_bins) * 0.9);
  }
}
#endif