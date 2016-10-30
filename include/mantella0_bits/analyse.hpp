/**
analyse_result
==============

.. cpp:class:: template<T, N> analyse_result

  **Template parameters**
  
    - **T** (``floating point``)

      - Lorem ipsum dolor sit amet
    
    - **N** (``unsigned``)

      - Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::array<unsigned, N> additive_separability
  
    Lorem ipsum dolor sit amet
*/
template <typename T, unsigned N>
struct analyse_result {
  std::array<unsigned, N> additive_separability;
};

/**
analyse
=======

.. cpp:function:: template<T1, N, T2> analyse(problem, evaluations, acceptable_deviation)

  **Template parameters**
  
    - **T1** (``floating point``)

      - Lorem ipsum dolor sit amet
    
    - **N** (``unsigned``)

      - Lorem ipsum dolor sit amet
    
    - **T2** (derived from ``mant::problem<T1, N>``)

      - Lorem ipsum dolor sit amet
  
  **Function parameters**
  
    - **problem** (``T2``)
    
      - Lorem ipsum dolor sit amet
  
    - **evaluations** (``unsigned``)
    
      - Lorem ipsum dolor sit amet
  
    - **acceptable_deviation** (``T1``)
    
      - Lorem ipsum dolor sit amet
    
  **Return** (``analyse_result``)
    
      - Lorem ipsum dolor sit amet
*/
template <typename T1, unsigned N, template <class, unsigned> class T2>
analyse_result<T1, N> analyse(
    const T2<T1, N>& problem,
    const unsigned evaluations,
    const T1 acceptable_deviation);

//
// Implementation
//

template <typename T1, unsigned N, template <class, unsigned> class T2>
analyse_result<T1, N> analyse(
    const T2<T1, N>& problem,
    const unsigned evaluations,
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
  CHECK((result.additive_separability == std::array<unsigned, 5>({0, 1, 2, 3, 4})));
}
#endif
