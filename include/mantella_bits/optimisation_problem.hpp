#pragma once

/**
.. cpp:class:: template <typename T, std::size_t N> optimisation_problem

The `optimisation_problem` struct is the base of every optimisation problem and aggregates

* the problem's number of dimension,
* a set of objective functions,
* lower and upper bounds

as well as attributes to

* permute, scale, translate or rotate a given parameter prior to evaluation each objective function and
* permute and scale the final objective value after adding the results of each objective function.

The later mentioned attributes are set to their neutral element as default (therefore leaving the parameter and objective value unaffected) and come in handy to test for parameter / objective value biases in optimisation algorithms.

.. code-block: c++
   #include <mantella>
  
   int main() {
     constexpr number_of_dimensions = 2;
     
     mant::optimisation_problem<double, number_of_dimensions> optimisation_problem;
     optimisation_problem.objective_functions = 
       {{[optimisation_problem](
           const auto parameter) {
         return std::accumulate(parameter, std::next(parameter, optimisation_problem.number_of_dimensions), 0);
       },
       "My first objective function"},
       {[optimisation_problem](
           const auto parameter) {
         return std::accumulate(parameter, std::next(parameter, optimisation_problem.number_of_dimensions), 1, std::multiplies<double>());
       },
       "My second objective function"}};
     
     return 0;
   }
  
Lorem ipsum ...
*/
template <typename T, std::size_t N>
struct optimisation_problem {
  const std::size_t number_of_dimensions = N;
  std::vector<std::pair<std::function<T(const typename std::array<T, N>::const_iterator parameter)>, std::string>> objective_functions;
  std::array<T, N> lower_bounds;
  std::array<T, N> upper_bounds;
  std::array<std::size_t, N> parameter_permutation;
  std::array<T, N> parameter_scaling;
  std::array<T, N> parameter_translation;
  std::array<T, N*N> parameter_rotation;
  T objective_value_scaling;
  T objective_value_translation;
  
  constexpr optimisation_problem() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
constexpr optimisation_problem<T, N>::optimisation_problem() noexcept {
  static_assert(std::is_floating_point<T>::value, "The type must be a floating point");
  static_assert(N > 0, "The number of dimensions mus be greater than 0.");
  
  lower_bounds.fill(-10.0);
  upper_bounds.fill(10.0);
  
  parameter_rotation.fill(0.0);
  for (decltype(N) n = 0; n < N; n++) {
    parameter_permutation[n] = n;
    parameter_rotation[n * N + n] = 1.0;
  }
  
  parameter_scaling.fill(1.0);
  parameter_translation.fill(0.0);
  objective_value_scaling = 1.0;
  objective_value_translation = 0.0;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimisation_problem", "[optimisation_problem]") {
  mant::optimisation_problem<double, 2> optimisation_problem;
  
  const std::array<double, 2> lower_bounds = {-10.0, -10.0};
  CHECK(optimisation_problem.lower_bounds == lower_bounds);
  
  const std::array<double, 2> upper_bounds = {10.0, 10.0};
  CHECK(optimisation_problem.upper_bounds == upper_bounds);
  
  const std::array<std::size_t, 2> parameter_permutation = {0, 1};
  CHECK(optimisation_problem.parameter_permutation == parameter_permutation);
  
  const std::array<double, 2> parameter_scaling = {1.0, 1.0};
  CHECK(optimisation_problem.parameter_scaling == parameter_scaling);
  
  const std::array<double, 2> parameter_translation = {0.0, 0.0};
  CHECK(optimisation_problem.parameter_translation == parameter_translation);
  
  const std::array<double, 4> parameter_rotation = {1.0, 0.0, 0.0, 1.0};
  CHECK(optimisation_problem.parameter_rotation == parameter_rotation);
  
  CHECK(optimisation_problem.objective_value_scaling == Approx(1.0));
  CHECK(optimisation_problem.objective_value_translation == Approx(0.0));
}
#endif