/**
Analysing
=========

  .. code-block:: animation
    :name: video_function.mp4
    
    #include <mantella0>
  
    int main () {
      
      return 0;
    }
  
    :octave:
    
    for n = 5:100
      [X,Y,Z] = peaks(n);
      surf(X, Y, Z)
      axis tight
      zlim([-10, 10])
      saveas(gcf, [name, int2str(n - 5), '.png'])
    end
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
