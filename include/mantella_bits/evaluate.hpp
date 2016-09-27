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
  
  .. note::

    The summation will follow the ordering in ``optimisation_problem.objective_functions``. In case objective functions with widely different co-domain spaces are used together (especially if their results differ by more than ``std::numeric_limits<T>::digits10`` digits), rounding errors can be minimised by ordering the objective functions, such that their results are ascending.
  
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
template <typename T1, typename T2, std::size_t N1, std::size_t N2>
typename std::common_type<T1, T2>::type evaluate(
    const optimisation_problem<T1, N1>& optimisation_problem,
    const std::array<T2, N2*N1>& parameter);
    
//
// Implementation
//  

template <typename T, std::size_t N>
T evaluate(
    const optimisation_problem<T, N>& optimisation_problem,
    const typename std::array<T, N>::const_iterator parameter) {
  T objective_value = 0.0;
  // We make no guarantees about the `objective_value`'s correctness, as it depends on the order in which `optimisation_problem.objective_functions` is processed.
  // Adding ascending values with (closely) representable immediate results can be expected to lead to more precise results.
  // However, without relying on heavy computational effort to enforce the above statement, user-given domain-knowledge is required (by filling `optimisation_problem.objective_functions` in an *optimal* order).
  for (const auto& objective_function : optimisation_problem.objective_functions) {
    objective_value += objective_function.first(parameter);
  }
  
  return objective_value;
}

template <typename T1, typename T2, std::size_t N>
typename std::common_type<T1, T2>::type evaluate(
    const optimisation_problem<T1, N>& optimisation_problem,
    const std::array<T2, N>& parameter) {
  if (std::is_same<T1, T2>::value) {
    return evaluate(optimisation_problem, parameter.cbegin());
  } else {
    std::array<typename std::common_type<T1, T2>::type, N> commonTypedParameter;
    std::copy_n(parameter.cbegin(), N, commonTypedParameter.begin());
    return evaluate(optimisation_problem, commonTypedParameter.cbegin());
  }
  
}