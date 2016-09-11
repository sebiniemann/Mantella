#pragma once

namespace bbob {
  template <typename T, std::size_t N>
  struct attractive_sector_function : black_box_optimisation_benchmark<T, N> {
    std::array<T, N> parameter_conditioning;
    std::array<T, N*N> rotation_q;
    
    attractive_sector_function() noexcept;
  };

//
// Implementation
// 

  // This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
  // So don't expect use to explain the unexplained.
  // @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
  template <typename T, std::size_t N>
  attractive_sector_function<T, N>::attractive_sector_function() noexcept
      : black_box_optimisation_benchmark<T, N>() {
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be a `float` or `double`.");
  
    this->parameter_translation = random_parameter_translation<T, N>();
    this->parameter_rotation = random_rotation_matrix<T, N>();
    
    parameter_conditioning = conditioning<T, N>(std::sqrt(10.0));
    rotation_q = random_rotation_matrix<T, N>();
    
    this->objective_functions =
        {{[this](
              const auto iterator) {
            std::array<T, N> conditioned_paramter;
            for (decltype(N) n = 0; n < N; ++n, ++iterator) {
              conditioned_paramter[n] = *iterator * parameter_conditioning;
            }
            
            std::array<T, N> transformed_parameter;
            if (std::is_same<T, float>::value) {
              cblas_sgemm(CblasColMajor, CblasNoTrans, N, N, N, 1, conditioned_paramter.data(), N, rotation_q.data(), N, 0, transformed_parameter.data(), N);
            } else {
              cblas_dgemm(CblasColMajor, CblasNoTrans, N, N, N, 1, conditioned_paramter.data(), N, rotation_q.data(), N, 0, transformed_parameter.data(), N);
            }
            
            // BLAS Matrix-Matrix-Multiplikation mit `rotationQ_`
            T s = 0.0;
            for (decltype(N) n = 0; n < N; ++n) {
              if (std::signbit(transformed_parameter[n]) == std::signbit(this->parameter_translation)) {
                s += 10000.0 * std::pow(transformed_parameter[n], 2.0);
              } else {
                s += std::pow(transformed_parameter[n], 2.0);
              }
            }

            return std::pow(oscillation(s), static_cast<T>(0.9L));
          },
          "BBOB Attractive Sector Function (f6)"}};
  }
}
