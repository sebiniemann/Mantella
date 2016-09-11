#pragma once

inline void seed_random_number_generator(
    const std::random_device::result_type seed);

//
// Implementation
//

inline void seed_random_number_generator(
    const std::random_device::result_type seed) {
#pragma omp parallel num_threads(MANTELLA_MAXIMAL_NUMBER_OF_THREADS)
  {
#pragma omp for schedule(static)
    for (decltype(random_number_generators().size()) n = 0; n < random_number_generators().size(); ++n) {
      // Uses a different seed for each generator, deterministically based on the provided one.
      random_number_generators()[n].seed(seed + n);
    }
  }
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE() {
  auto distribution = std::bind(std::uniform_real_distribution<double>(), random_number_generators()[0]);
  
  mant::seed_random_number_generator(12345);
  std::array<double, 10> expected_random_values;
  std::generate(expected_random_values.begin(), expected_random_values.end(), distribution);      
  mant::seed_random_number_generator(12345);
  std::array<double, 10> actual_random_values;
  std::generate(actual_random_values.begin(), actual_random_values.end(), distribution);
  
  CHECK(actual_random_values == expected_random_values);
}
#endif
