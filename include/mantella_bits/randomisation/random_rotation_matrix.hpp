#pragma once

/**

*/
template <typename T, std::size_t number_of_columns>
std::array<T, number_of_columns*number_of_columns> random_rotation_matrix();

//
// Implementation
//

template <typename T, std::size_t number_of_columns>
std::array<T, number_of_columns*number_of_columns> random_rotation_matrix() {
  static_assert(number_of_columns > 1, "The number of dimensions must be greater than 1.");
  static_assert(std::is_same<T, double>::value, "The number of dimensions must be greater than 1.");

  /* @see Francesco Mezzadri (2007). How to Generate Random Matrices from the Classical Compact Groups. Notices of the AMS, 54(5), pp. 592-604.
   * In summary, 
   * 1. Fills a square, *number_of_columns*-dimensional matrix with numbers drawn from a normal distribution.
   * 2. Decompose the matrix into an orthonormal and rectangular matrix (using a QR decomposition).
   * 3. Multiply orthonormal matrix's diagonal with the signs of the rectangular matrix's diagonal.
   */
  std::array<T, number_of_columns*number_of_columns> q;
  std::generate(q.begin(), q.end(), std::bind(std::normal_distribution<T>(), std::ref(random_number_generator())));
  
  /* Computing the matrices *Q* and *R* of a QR decomposition using LAPACK is based on two steps.
   * 1. At first, the upper triangular matrix *R* is calculated using `geqrf`, while *Q* is given as a product of `number_of_columns` elementary reflectors.
   * 2. Next, the orthogonal matrix *Q* is calculated - based on the previous `number_of_columns` elementary reflectors - using `orgqr`.
   */
  std::array<T, number_of_columns> elementary_reflectors;
  if (LAPACKE_dgeqrf(LAPACK_COL_MAJOR, number_of_columns, number_of_columns, q.data(), number_of_columns, elementary_reflectors.data()) != 0) {
  // if (LAPACKE_dgeqp3(LAPACK_COL_MAJOR, number_of_columns, number_of_columns, q.data(), number_of_columns, elementary_reflectors.data(), work.data(), number_of_columns) != 0) {
    // In very rare cases, the QR decomposition fails due to the limited numerical precision. In these rare cases, we simply return an identity matrix.
    q.fill(0.0);
    for (std::size_t n = 0; n < number_of_columns; ++n) {
      q[n * number_of_columns + n] = T(1.0);
    }
    return q;
  }
  
  // Stores whether the sign of an element on r's diagonal is negative.
  std::array<bool, number_of_columns> r;
  for (std::size_t n = 0; n < number_of_columns; ++n) {
    r[n] = (q[n * number_of_columns + n] < T(0.0)) ? true : false;
  }
  
  // Computes q.
  if (LAPACKE_dorgqr(LAPACK_COL_MAJOR, number_of_columns, number_of_columns, number_of_columns, q.data(), number_of_columns, elementary_reflectors.data()) != 0) {
    // In very rare cases, the QR decomposition fails due to the limited numerical precision. In these rare cases, we simply return an identity matrix.
    q.fill(0.0);
    for (std::size_t n = 0; n < number_of_columns; ++n) {
      q[n * number_of_columns + n] = T(1.0);
    }
    return q;
  }
  
  for (std::size_t n = 0; n < number_of_columns; ++n) {
    if (r[n]) {
      q[n * number_of_columns + n] *= T(-1.0);
    }
  }
    
  return q;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_rotation_matrix", "[randomisation][random_rotation_matrix]") {
  constexpr std::size_t number_of_repetitions = 10000;
  
  std::array<double, number_of_repetitions> angles;
  std::size_t n = 0;
  for (n = 0; n < number_of_repetitions; ++n) {
    auto rotation_matrix = mant::random_rotation_matrix<double, 2>();
    
    std::array<double, 2> unit_vector = {0.0, 1.0};
    std::array<double, 2> rotated_vector;
	  cblas_dgemv(CblasColMajor, CblasNoTrans, 2, 2, 1.0, rotation_matrix.data(), 2, unit_vector.data(), 1, 0.0, rotated_vector.data(), 1);
    
    // Normalises all angles to be within [0, 1].
    angles[n] = (std::atan2(rotated_vector[1], rotated_vector[0]) + 3.141592653589793) / 6.283185307179586;
    CHECK((0.0 <= angles[n] && angles[n] <= 1.0));
  }
  
  // Performs an entropy-based uniformity test.
  constexpr std::size_t number_of_bins = 10;
  std::array<double, number_of_bins> histogram;
  histogram.fill(0.0);
  for (const auto angle : angles) {
    // Ensures that the index will never be out-of-range (esp. in case *angle* is 1).
    histogram[std::min<std::size_t>(angle * number_of_bins, number_of_bins - 1)] += 1.0 / static_cast<double>(number_of_repetitions);
  }
  CHECK(std::accumulate(histogram.cbegin(), histogram.cend(), 0.0, [](const double entropy, const double element) {return entropy + -std::log(element) * element;}) > std::log(number_of_bins) * 0.9);
}
#endif
