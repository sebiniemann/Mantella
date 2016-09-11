#pragma once

namespace bbob {
  template <typename T, std::size_t N>
  struct bent_cigar_function : black_box_optimisation_benchmark<T, N> {
    std::array<T, N*N> rotation_Q;
    
    constexpr bent_cigar_function() noexcept;
  };

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
  template <typename T, std::size_t N>
  bent_cigar_function<T, N>::bent_cigar_function()
      : black_box_optimisation_benchmark<T, N>(),
        rotationQ_(synchronise(randomRotationMatrix(numberOfDimensions_))) {
    assert(numberOfDimensions_ > 1 && "BentCigarFunction: The number of dimensions must be greater than 1.");

    setParameterTranslation(getRandomParameterTranslation());

    setObjectiveFunctions(
        {{[this](
              const arma::vec& parameter_) {
            assert(parameter_.n_elem == numberOfDimensions_);

            const arma::vec& z = arma::square(rotationQ_ * getAsymmetricParameter(0.5, rotationQ_ * parameter_));
            return z(0) + 1000000.0 * arma::accu(z.tail(z.n_elem - 1));
          },
          "BBOB Bent Cigar Function (f12)"}});
  }
}
