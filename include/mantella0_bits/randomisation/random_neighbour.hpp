/**
random_neighbour
================

.. cpp:function:: template<T, N> random_neighbour(parameter, minimal_distance, maximal_distance, number_of_active_dimensions)

  **Template parameters**
  
    - **T** (``floating point``)

      - The parameter's value type
    
    - **N** (``std::size_t``)

      - The parameter size
  
  **Function parameters**
  
    - **parameter** (``std::array<T, N>``)
    
      - Lorem ipsum dolor sit amet
  
    - **minimal_distance** (``T``)
    
      - Lorem ipsum dolor sit amet
  
    - **maximal_distance** (``T``)
    
      - Lorem ipsum dolor sit amet
  
    - **number_of_active_dimensions** (``unsigned``)
    
      - Lorem ipsum dolor sit amet
    
  **Return**
    
      - Lorem ipsum dolor sit amet
*/
template <typename T, std::size_t N>
std::array<T, N> random_neighbour(
    const std::array<T, N>& parameter,
    const T minimal_distance,
    const T maximal_distance,
    const std::size_t number_of_active_dimensions);

//
// Implementation
//

template <typename T, std::size_t N>
std::array<T, N> random_neighbour(
    const std::array<T, N>& parameter,
    const T minimal_distance,
    const T maximal_distance,
    const std::size_t number_of_active_dimensions) {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(N > 0, "");
  
  assert(T(0.0) <= minimal_distance && minimal_distance <= maximal_distance);
  assert(0 < number_of_active_dimensions && number_of_active_dimensions <= N);

  /* @see J. S. Hicks and R. F. Wheeling (1959). An efficient method for generating uniformly distributed points on the 
   * surface of an n-dimensional sphere. Communications of the ACM, 2(4), pp. 17-19.
   *
   * In summary, it works as followed:
   * 1. Uniformly draw an *n*-dimensional direction vector (by normalising a normal distribution vector).
   * 2. Multiply it with a length, uniformly drawn from [*minimal_distance*, *maximal_distance*].
   * 3. Translate its origin by adding *parameter*.
   */
  std::array<T, N> neighbour;
  std::generate(
    neighbour.begin(), std::next(neighbour.begin(), number_of_active_dimensions),
    std::bind(std::normal_distribution<T>(), std::ref(random_number_generator())));

  // Instead of iterating twice through the vector (normalising it first and scaling it afterwards), we do everything in
  // one step and normalise the length accordingly.
  const T length = (
      minimal_distance + 
      std::uniform_real_distribution<T>(T(0.0), T(1.0))(random_number_generator()) * (
        maximal_distance - minimal_distance
      )
    ) / 
    std::sqrt(std::inner_product(
      neighbour.cbegin(), std::next(neighbour.cbegin(), number_of_active_dimensions),
      neighbour.cbegin(),
      T(0.0)
    ));

  std::transform(
    neighbour.cbegin(), std::next(neighbour.cbegin(), number_of_active_dimensions),
    parameter.cbegin(), 
    neighbour.begin(), 
    [length](const auto neighbour_element, const auto parameter_element) {
      return parameter_element + neighbour_element * length;
    });

  return neighbour;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_neighbour", "[randomisation][random_neighbour]") {
  constexpr std::array<double, 3> parameter = {1.0, -2.0, 3.0};
  
  auto&& neighbour = mant::random_neighbour(parameter, 1.0, 3.0, 2);
  const auto distance = std::sqrt(std::inner_product(
    neighbour.cbegin(), std::next(neighbour.cbegin(), 2),
    parameter.cbegin(),
    0.0,
    std::plus<double>(),
    [](const auto neighbour_element, const auto parameter_element) {
      return std::pow(neighbour_element - parameter_element, 2.0);
    }));
    
  CHECK(1.0 <= distance);
  CHECK(distance <= 3.0);
}
#endif
