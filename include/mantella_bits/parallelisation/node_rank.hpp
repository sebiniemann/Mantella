#pragma once

inline int node_rank();

//
// Implementation
//

inline int node_rank() {
#if defined(MANTELLA_SUPPORT_MPI)
  int node_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &node_rank);

  return node_rank;
#else
  return 0;
#endif
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("node_rank", "[node_rank]") {
#if defined(SUPPORT_MPI)
      int node_rank = 0;
      MPI_Comm_rank(MPI_COMM_WORLD, &node_rank);
      CHECK(mant::node_rank() == node_rank);
#else
      CHECK(mant::node_rank() == 0);

#endif
}
#endif
