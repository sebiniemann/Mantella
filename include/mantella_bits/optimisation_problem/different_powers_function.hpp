#pragma once

/**

*/
template <typename T, std::size_t N>
struct different_powers_function : optimisation_problem<T, N> {
  constexpr different_powers_function() noexcept;
}

//
// Implementation
//

template <typename T, std::size_t N>
constexpr different_powers_function<T, N>::different_powers_function() noexcept 
    : optimisation_problem<T, N>() {
  objective_functions = {{
    [this](
        const arma::vec& parameter_) {
      assert(parameter_.n_elem == numberOfDimensions_);

      const arma::vec& z = arma::abs(parameter_);
      return arma::norm(z % getConditionedParameter(arma::square(z)));
    },
    "BBOB Different Powers Function (f14)"}});
}

//
// Unit tests
//
    DifferentPowersFunction::DifferentPowersFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions) {
      assert(numberOfDimensions_ > 1 && "DifferentPowersFunction: The number of dimensions must be greater than 1.");

      setObjectiveFunctions(
          {{[this](
                const arma::vec& parameter_) {
              assert(parameter_.n_elem == numberOfDimensions_);

              const arma::vec& z = arma::abs(parameter_);
              return arma::norm(z % getConditionedParameter(arma::square(z)));
            },
            "BBOB Different Powers Function (f14)"}});
    }
