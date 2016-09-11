#pragma once

inline std::random_device::result_type random_seed();

//
// Implementation
//

inline std::random_device::result_type random_seed() {
  std::random_device random_device;
  std::random_device::result_type seed = random_device();

  // Provides a different seed for each MPI node.
  for (decltype(node_rank()) n = 0; n < node_rank(); ++n) {
    seed = random_device();
  }

  return seed;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)

#endif
