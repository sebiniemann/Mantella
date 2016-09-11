#pragma once

template <typename T, std::size_t N>
T evaluate(
    const optimisation_problem<T, N>& optimisation_problem,
    const typename std::array<T, N>::const_iterator parameter);
    
template <typename T1, typename T2, std::size_t N>
typename std::common_type<T1, T2>::type evaluate(
    const optimisation_problem<T1, N>& optimisation_problem,
    const std::array<T2, N>& parameter);
    
//
// Implementation
//  

template <typename T, std::size_t N>
T evaluate(
    const optimisation_problem<T, N>& optimisation_problem,
    const typename std::array<T, N>::const_iterator parameter) {
  T objective_value = 0.0;
  // We make no guarantees about `objective_value`'s correctness, as it depends on the order in which `optimisation_problem.objective_functions` is processed.
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