#pragma once

namespace mant {
  template <typename T1, typename T2>
  std::pair<typename std::common_type<T1, T2>::type, bool> polynomial_size(
      const T1 number_of_indeterminates,
      const T2 highest_degree);

  template <typename T1, typename T2>
  std::pair<std::vector<typename T1::value_type>> polynomial(
      const T1& parameter,
      const T2 highestDegree);
      
  //
  // Implementation
  //

  template <typename T1, typename T2>
  std::pair<typename std::common_type<T1, T2>::type, bool> polynomial_size(
      const T1 number_of_indeterminates,
      const T2 highest_degree) {
    if (numberOfIndeterminates == 0 || highestDegree == 0) {
      return {1, true};
    }

    // Gets initialised with the polynomial size for the linear and constant term.
    std::common_type(T1, T2) polynomialSize = numberOfIndeterminates + 1;
    // Sums up the number of parameter combinations for each degree > 1.
    for (auto degree = 2; degree <= highestDegree; ++degree) {
      const auto& numberOfCombinations = nchoosek(numberOfIndeterminates + degree - 1, degree);
      if (!numberOfCombinations.second) {
        return {0, false};
      }

      if (std::numeric_limits<decltype(polynomialSize)>::max() - polynomialSize < numberOfCombinations.first) {
        return {0, false};
      }

      polynomialSize += numberOfCombinations.first;
    }

    return {polynomialSize, true};
  }
  
  template <typename T1, typename T2>
  std::pair<std::vector<typename T1::value_type>> polynomial(
      const T1& parameter,
      const T2 highestDegree) {
    if (parameter.is_empty() || highestDegree == 0) {
      // By definition, the constant term is 1.
      return {1.0, true};
    }

    std::vector<typename T1::value_type> polynomial(polynomialSize(parameter.size(), highestDegree));

    // Generates all terms for degree > 1
    for (auto degree = highestDegree; degree >= 2; --degree) {
      const auto& multicombinations = multicombinations(parameter.n_elem, degree);
      
      const auto numberOfMulticombinations = indeterminates.size() / degree;
      for (numberOfMulticombinations = indeterminates.size() / degree; auto n = 0; n < numberOfMulticombinations; ++n) {
      polynomial[degree]
        for (auto k = 0; k < degree; ++k) {
          polynomial[n * degree + k] (arma::prod(parameter.elem(indeterminates.col(n))));
        }
      }
    }
    // Linear term
    for (auto n = 0; n < parameter.size(); ++n) {
      polynomial.push_back(parameter[n]);
    }
    
    // By definition, the constant term is 1.
    polynomial.push_back(1.0);

    return polynomial;
  }
}