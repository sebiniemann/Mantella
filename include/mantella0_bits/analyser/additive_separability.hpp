/**
Additive separability
---------------------

.. cpp:function:: additive_separability(problem, evaluations, acceptable_deviation)

  .. versionadded:: 1.0.0 

  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  
  Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. 
  
  .. list-table:: Template parameters

    * - T1
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``std::size_t``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.
    * - T2
        
        Derived from :cpp:any:`problem`
      - The problem type to be analysed.
      
  .. list-table:: Function functions

    
    * - problem
    
        ``T2``
      - The problem to be analysed for additive separation.
      
        Make sure that the problems objective function (``.objective_function``) is set and its lower bounds
        (``.lower_bounds``) are less than or equal to their upper bounds (``.upper_bounds``).
    * - evaluations
    
        ``std::size_t``
      - The maximal number of evaluations per tried out separation. If an evaluation is found that exceeds ``acceptable_deviation``, the separation candidate is discarded and the next one is tried.
      
        Instead of trying out each possible separation, which will be a `Bell number <https://en.wikipedia.org/wiki/Bell_number>`_ and grows very quickly (2, 5, 15, 52, 203, 877, 4140, 21147, ... `A000110 in OEIS <https://oeis.org/A000110>`_), only the separations into two parts are explicitly tested and reasoning is applied to check for further separations.
        
        Therefore, the number of tested separations is at most a `Mersenne number <https://en.wikipedia.org/wiki/Mersenne_prime>`_, which will still grow exponentially (1, 3, 7, 15, 31, 63, 127, 255, 511, ... `A000225 in OEIS <https://oeis.org/A000225>`_), but grows more slowly.
        
        In summary
        
        .. math::
        
          \text{Overall evaluations} \leq \text{evaluations} \cdot \underbrace{(2^n - 1)}_{\llap{\text{Mersenn}\rlap{\text{e number}}}}
    * - acceptable_deviation
    
        ``T1``
      - The maximal acceptable deviation from ``0``.

  .. list-table:: Returns

    
    * - ``std::array<std::size_t, N>``
      - Lorem ipsum dolor sit amet
*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2>
std::array<std::size_t, N> additive_separability(
    const T2<T1, N>& problem,
    const std::size_t evaluations,
    const T1 acceptable_deviation);

//
// Implementation
// 

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
std::array<std::size_t, N> additive_separability(
    const T2<T1, N>& problem,
    const std::size_t evaluations,
    const T1 acceptable_deviation) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(N > 0, "");
  assert(std::equal(
    problem.lower_bounds.cbegin(), problem.lower_bounds.cend(),
    problem.upper_bounds.cbegin(), problem.upper_bounds.cend(),
    [](const auto lower_bound, const auto upper_bound) {
      return lower_bound <= upper_bound;
    })); 
  assert(evaluations > 0);
  assert(acceptable_deviation >= 0);
      
  // Initialises *partition* with 0, indicating that all dimensions are in the same group and none is separable.
  std::array<std::size_t, N> partition;
  partition.fill(0);
  
  if (N == 1) {
    return partition;
  }
  
  // Iterates through all partitions with two parts.
  // The parts are ordered, such that the first one will not contain more elements than the second one.
  // *n* stands for the number of elements in the first part (and is therefore limited by floor
  // (*N* / 2)).
  for (std::size_t n = 1; n <= N / 2; ++n) {
    /* Each partition is represented by a bitmask, whereby *true* marks elements in the first part and *false* marks 
     * elements in the second part.
     * By iterating over all permutation of *bitmask*, all possible partitions are proceeded.
     *
     *   (1, 0, 0, 0) Bit mask (example)
     *    |
     *   (0         ) First part  \
     *       |  |  |               |- Partition
     *   (   1  2  3) Second part /
     */
    
    // Sets the first *n* values inside *bitmask* to true and all others to *false*.
    std::array<bool, N> bitmask;
    std::fill(bitmask.begin(), std::next(bitmask.begin(), n), true);
    std::fill(std::next(bitmask.begin(), n), bitmask.end(), false);
      
    do {
      /* Avoids adding duplicates when we split the elements in equally sized parts, skipping the second half of 
       * permutations.
       *
       * If *N* was 4 and *n* was 2, we would get:
       *
       *   (1, 1, 0, 0) (Bit mask)
       *   (1, 0, 1, 0) (Bit mask)
       *   (1, 0, 0, 1) (Bit mask)
       *   (0, 1, 1, 0) (Bit mask) Invert of the third bit mask <- Skipped from here on
       *   (0, 1, 0, 1) (Bit mask) Invert of the second bit mask
       *   (0, 0, 1, 1) (Bit mask) Invert of the first bit mask
       */
      if (2 * n == N && !std::get<0>(bitmask)) {
        break;
      }

      for (std::size_t k = 0; k < evaluations; ++k) {
        /* Tests whether the function is separable into the two parts or not.
         * A function *f* is additive separable into two other function *g*, *h* if the following holds:
         *
         * f(x, y) - g(x) - h(y) = 0, for all x, y
         *
         * As it is practical impossible to get two separations *g*, *h* of *f*, when we only got a caller to *f* and no
         * analytic/symbolic form, we use a direct consequence from the above equation instead, that must also hold true
         * for additive separable functions and uses only *f*:
         * 
         * f(a, c) + f(b, d) - f(a, d) - f(b, c) = 0, for all a, b, c, d
         *
         */
        
        // Fills (a, c) with randomly and uniformly values, drawn from [0, 1].
        std::array<T1, N> parameter_ac;
        std::generate(
          parameter_ac.begin(), parameter_ac.end(),
          std::bind(
            std::uniform_real_distribution<T1>(T1(0.0), T1(1.0)),
            std::ref(mant::random_number_generator())));
            
        // Fills (b, d) with randomly and uniformly values, drawn from [0, 1].
        std::array<T1, N> parameter_bd;
        std::generate(
          parameter_bd.begin(), parameter_bd.end(),
          std::bind(
            std::uniform_real_distribution<T1>(T1(0.0), T1(1.0)),
            std::ref(mant::random_number_generator())));
        
        std::array<T1, N> parameter_ad;
        std::array<T1, N> parameter_bc;
        for (std::size_t l = 0; l < N; ++l) {
          // Maps [0, 1] linear to [*problem.lower_bounds*, *problem.upper_bounds*], 
          // addressing the actual bounds per dimension.
          parameter_ac.at(l) = 
            problem.lower_bounds.at(l) + 
            parameter_ac.at(l) * (
              problem.upper_bounds.at(l) - problem.lower_bounds.at(l)
            );
          parameter_bd.at(l) = 
            problem.lower_bounds.at(l) +
            parameter_bd.at(l) * (
              problem.upper_bounds.at(l) - problem.lower_bounds.at(l)
            );
          
          // Fills (a, d) and (b, d), based on (a, c) and (b, d).
          if (bitmask.at(l)) {
            parameter_ad.at(l) = parameter_ac.at(l);
            parameter_bc.at(l) = parameter_bd.at(l);
          } else {
            parameter_ad.at(l) = parameter_bd.at(l);
            parameter_bc.at(l) = parameter_ac.at(l);
          }
        }
      
        // Tests whether the above condition for separable functions holds true or not.
        if (std::fabs(
              problem.objective_function(parameter_ac) +
              problem.objective_function(parameter_bd) -
              problem.objective_function(parameter_ad) -
              problem.objective_function(parameter_bc)
            ) > 
            acceptable_deviation) {
          break;
        }
        
        /* Aggregates the partition into multiple parts by combining all acceptable partitions with two parts.
         * If we would write all bit masks of acceptable partitions with two parts below each other ...
         *
         *   (1, 1, 1, 0) (Bit mask #1)
         *   (1, 0, 0, 0) (Bit mask #2)
         *   (1, 1, 1, 0) (Bit mask #3)
         *
         * ... dimensions with the same sequence along the column axis are not separable and in the same part.
         * For the given example, the resulting partition would therefore be (0, 1, 1, 2), separating the function into 
         * three parts.
         *
         * In order to avoid storing all bit masks and doing it incrementally instead, we compare the current bit mask 
         * with the current *partition* instead.
         *
         *   (1, 1, 1, 0) (Current bit mask)
         *   (0, 1, 1, 2) (Current *partition*)
         *
         * Analogous to the statement above, two dimensions remain in the same part, if the sequence along their column 
         * axis is equal.
         */
        
        std::array<bool, N> is_remaining_dimension;
        is_remaining_dimension.fill(true);
        for (std::size_t l = 0; l < N; ++l) {
          if (is_remaining_dimension.at(l)) {
            // Compares the sequence for all following, remaining dimensions to the current one.
            // If the sequence matches, both get the same part number (*l*) and the dimension is marked as 
            // non-remaining.
            for (std::size_t m = l + 1; m < N; ++m) {
              if (is_remaining_dimension.at(m) && 
                  bitmask.at(l) == bitmask.at(m) && 
                  partition.at(l) == partition.at(m)) {
                partition.at(m) = l;
                is_remaining_dimension.at(m) = false;
              }
            }
            
            partition.at(l) = l;
            
            // We are already finished, as there is no better partition as having one part per dimensions.
            if (l == N - 1) {
              return partition;
            }
          }
        }
      }
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  }
  
  return partition;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("additive_separability", "[property_analysis][additive_separability]") {
  // Fully-separable
  const mant::sphere_function<double, 5> sphere_function;
  CHECK((additive_separability(sphere_function, 100, 1e-12) == std::array<std::size_t, 5>({0, 1, 2, 3, 4})));
  
  // Partly-separable
  mant::problem<double, 5> problem;
  problem.objective_function = [](const auto& parameter) {
    return std::get<0>(parameter) * std::get<4>(parameter) + std::get<2>(parameter) * std::get<3>(parameter);
  };
  CHECK((additive_separability(problem, 100, 1e-12) == std::array<std::size_t, 5>({0, 1, 2, 2, 0})));
  
  // Non-separable
  const mant::ackley_function<double, 5> ackley_function;
  CHECK((additive_separability(ackley_function, 100, 1e-12) == std::array<std::size_t, 5>({0, 0, 0, 0, 0})));
  
  // Non-separable with a permissive, acceptable deviation
  CHECK((additive_separability(ackley_function, 100, 100.0) == std::array<std::size_t, 5>({0, 1, 2, 3, 4})));
}
#endif
