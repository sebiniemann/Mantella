#pragma once

inline int number_of_nodes();

//
// Implementation
//

inline int number_of_nodes() {
#if defined(MANTELLA_SUPPORT_MPI)
  int number_of_nodes;
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_nodes);

  return number_of_nodes;
#else
  return 1;
#endif
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("number_of_nodes", "[number_of_nodes]") {
#if defined(SUPPORT_MPI)
      int number_of_nodes = 0;
      MPI_Comm_size(MPI_COMM_WORLD, &number_of_nodes);
      CHECK(mant::number_of_nodes() == number_of_nodes);
#else
      CHECK(mant::number_of_nodes() == 1);
#endif
}
#endif
