#pragma once
    
/**
  .. cpp:var:: random_number_generators()
*/
inline std::array<random_number_generator_t, MANTELLA_NUMBER_OF_THREADS>& random_number_generators();

//
// Implementation
//

inline std::array<random_number_generator_t, MANTELLA_NUMBER_OF_THREADS>& random_number_generators() {
  static std::array<random_number_generator_t, MANTELLA_NUMBER_OF_THREADS> random_number_generators;
  return random_number_generators;
};

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_number_generators", "[random_number_generator][random_number_generators]") {
  CHECK(mant::random_number_generators().size() == MANTELLA_NUMBER_OF_THREADS);
}
#endif