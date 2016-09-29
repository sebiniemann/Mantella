#pragma once

/**
.. cpp:function:: T evaluate(optimisation_problem, begin_parameter, end_parameter)

  .. versionadded:: 1.x

  Sums the objective values for the parameter ``(*begin_parameter, ..., *(end_parameter-1))`` over all functions in ``optimisation_problem.objective_functions``.
  
  .. math::
    f(\mathbf{x}) = \sum_{i = 1}^n f_i(\mathbf{x}), \text{ (simplified, see below)}
    
  Prior to the evaluation, the parameter is permuted, rotated, scaled and translated, while the objective value is scaled and translated after the summation.
    
  .. math::
    \begin{align}
      f(\mathbf{x}) & = s^f \cdot \bigg(\sum_{i = 1}^n f_i(\mathbf{s}^x \cdot R^x \cdot \tilde{\mathbf{x}} + \mathbf{t}^x)\bigg) + t^f, \tilde{\mathbf{x}} = (\mathbf{x}_{\mathbf{p}^x_1}, ..., \mathbf{x}_{\mathbf{p}^x_{|\mathbf{x}|}}) \\
      \mathbf{p}^x & = \text{optimisation_problem.parameter_permutation} \\
      \mathbf{s}^x & = \text{optimisation_problem.parameter_scaling} \\
      \mathbf{t}^x & = \text{optimisation_problem.parameter_translation} \\
      R^x & = \text{optimisation_problem.parameter_rotation} \\
      s^f & = \text{optimisation_problem.objective_value_scaling} \\
      t^f & = \text{optimisation_problem.objective_value_translation}
    \end{align}
  
  The following example shows a common use case in population-based/evolutionary optimisation algorithms, where multiple parameters are generated at once, making use of the iterators to avoid additional copies.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      constexpr std::size_t number_of_dimensions = 2;
      mant::bbob::sphere_function<double, number_of_dimensions> optimisation_problem;
      
      constexpr std::size_t number_of_parameters = 10;
      std::array<double, number_of_parameters*number_of_dimensions> parameters;
      
      // Fills all parameters with uniformly distributed values.
      // Since we are accessing mant::random_number_generators() directly, first ensure that the element exists.
      // **Note:** The number of generator returned by mant::random_number_generators() is set by and equal to MANTELLA_MAXIMAL_NUMBER_OF_THREADS.
      static_assert(MANTELLA_MAXIMAL_NUMBER_OF_THREADS > 0, "Why should this even fail, what were you doing?");
      std::generate(parameters.begin(), parameters.end(), std::bind(std::uniform_real_distribution<double>(0.0, 1.0), std::ref(mant::random_number_generators()[0])));
      
      // At this point, we should also parallelise the whole computation.
      #pragma omp parallel {
        #pragma omp for schedule(static) {
          for (auto begin = parameters.cbegin(), end = std::next(begin, number_of_dimensions); begin != parameters.cend(); begin = end, end = std::next(begin, number_of_dimensions)) {
            std::cout << mant::evaluate(optimisation_problem, begin, end) << std::endl;
          }
        }
      }
      
      return 0;
    }
  
  **Parameters**
   
    * **optimisation_problem** (:cpp:class:`optimisation_problem` ``<T, N>``) - The optimisation problem.
    * **begin_parameter** (``std::array<T, N>::const_iterator``) - An iterator referring to the first element within the parameter.
    * **end_parameter** (``std::array<T, N>::const_iterator``) - An iterator referring to the *past-the-end* element within the parameter.

  **Returns**
   
    ``T`` - The computed objective value. 
*/
    
/**
.. cpp:function:: T evaluate(optimisation_problem, parameter)

  .. versionadded:: 1.x

  This is a short-cut for ``evaluate(optimisation_problem, parameter.cbegin(), parameter.cend())``.
  
  It is especially useful if the parameter is an `rvalue <http://en.cppreference.com/w/cpp/language/value_category>`_.
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      constexpr std::size_t number_of_dimensions = 2;
      mant::bbob::sphere_function<double, number_of_dimensions> optimisation_problem;
      
      std::cout << mant::evaluate(optimisation_problem, {1.0, 2.0}) << std::endl;
      
      return 0;
    }
  
  **Parameters**
   
    * **optimisation_problem** (:cpp:class:`optimisation_problem` ``<T, N>``) - The optimisation problem.
    * **parameter** (``std::array<T, N>``) - The parameter.

  **Returns**
   
    ``T`` - The computed objective value. 
*/
template <typename T, std::size_t number_of_dimensions>
T evaluate(
    const optimisation_problem_t<T, number_of_dimensions>& optimisation_problem,
    const std::array<T, number_of_dimensions>& parameter);
    
//
// Implementation
//  

template <typename T, std::size_t number_of_dimensions>
T evaluate(
    const optimisation_problem_t<T, number_of_dimensions>& optimisation_problem,
    const std::array<T, number_of_dimensions>& parameter) {
  std::array<T, number_of_dimensions> permuted_parameter;
  for (std::size_t n = 0; n < number_of_dimensions; ++n) {
    permuted_parameter[n] = parameter[optimisation_problem.parameter_permutation[n]];
  }
  
  std::array<T, number_of_dimensions> adjusted_parameter;
	cblas_dgemv(CblasColMajor, CblasNoTrans, number_of_dimensions, number_of_dimensions, T(1.0), optimisation_problem.parameter_rotation.data(), number_of_dimensions, permuted_parameter.data(), 1, T(0.0), adjusted_parameter.data(), 1);
  
  for (std::size_t n = 0; n < number_of_dimensions; ++n) {
    adjusted_parameter[n] = optimisation_problem.parameter_scaling[n] * adjusted_parameter[n] + optimisation_problem.parameter_translation[n];
  }
  
  return optimisation_problem.objective_value_scaling * std::accumulate(optimisation_problem.objective_functions.cbegin(), optimisation_problem.objective_functions.cend(), T(0.0), [&adjusted_parameter](const T sum, const auto objective_function) {return sum + objective_function.first(adjusted_parameter);}) + optimisation_problem.objective_value_translation;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("evaluate", "[evaluate]") {
  mant::optimisation_problem_t<double, 3> optimisation_problem;
  optimisation_problem.lower_bounds = {10.0, 10.0, 10.0};
  optimisation_problem.upper_bounds = {-10.0, -10.0, -10.0};
  optimisation_problem.objective_functions = {{
    [](
        const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), 0.0);
    },
    "My objective function"
  }, {
    [](
        const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), 1.0, std::multiplies<double>());
    },
    "My other objective function"
  }};
  optimisation_problem.parameter_permutation = {2, 1, 0};
  optimisation_problem.parameter_scaling = {2.0, 3.0, 4.0};
  optimisation_problem.parameter_translation = {2.0, 2.0, 3.0};
  optimisation_problem.parameter_rotation = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0};
  optimisation_problem.objective_value_scaling = 3.0;
  optimisation_problem.objective_value_translation = 1.0;
  
  CHECK(mant::evaluate(optimisation_problem, {1.0, 2.0, 3.0}) == Approx(-209.0));
}
#endif
