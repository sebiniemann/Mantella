#pragma once

inline std::array<random_number_generator_t, MANTELLA_MAXIMAL_NUMBER_OF_THREADS>& random_number_generators();

//
// Implementation
//

inline std::array<random_number_generator_t, MANTELLA_MAXIMAL_NUMBER_OF_THREADS>& random_number_generators() {
  static std::array<random_number_generator_t, MANTELLA_MAXIMAL_NUMBER_OF_THREADS> random_number_generators;
  return random_number_generators;
};

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_number_generators", "[random_number_generators]") {
  CHECK(mant::random_number_generators().size() == MANTELLA_MAXIMAL_NUMBER_OF_THREADS);
}
#endif