/**
Analysing
=========

.. cpp:function:: analyse(problem, evaluations, acceptable_deviation)

  .. versionadded:: 1.0.0 

  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  
  Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. 
  
  .. list-table:: Template parameters
    :widths: 27 73

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``std::size_t``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.
      
  .. list-table:: Function functions
    :widths: 27 73
    
    * - problem
    
        ``T2``
      - The problem's boundaries will be remapped to ``[0, 1]``. However, the provided problem remains unchanged.
    * - evaluations
    
        ``std::size_t``
      - Lorem ipsum dolor sit amet
    * - acceptable_deviation
    
        ``T1``
      - Lorem ipsum dolor sit amet

  .. list-table:: Returns
    :widths: 27 73
    
    * - ``analyse_result``
      - Lorem ipsum dolor sit amet
    
.. include:: analyser/additive_separability.include
*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2>
analyse_result<T1, N> analyse(
    const T2<T1, N>& problem,
    const std::size_t evaluations,
    const T1 acceptable_deviation);

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
analyse_result<T1, N> analyse(
    const T2<T1, N>& problem,
    const std::size_t evaluations,
    const T1 acceptable_deviation) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(N > 0, "");
  static_assert(std::is_base_of<mant::problem<T1, N>, T2<T1, N>>::value, "");
  
  assert(evaluations > 0);
  assert(acceptable_deviation >= 0);

  return {
    additive_separability(problem, evaluations, acceptable_deviation)
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("analyse", "[analyse]") {
  const mant::sphere_function<double, 5> sphere_function;

  const auto&& result = mant::analyse(sphere_function, 100, 1e-12);
  CHECK((result.additive_separability == std::array<std::size_t, 5>({0, 1, 2, 3, 4})));
}
#endif
