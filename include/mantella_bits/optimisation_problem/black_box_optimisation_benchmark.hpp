#pragma once

template <typename T, std::size_t N>
struct black_box_optimisation_benchmark : optimisation_problem<T, N> {
  black_box_optimisation_benchmark() noexcept;
};

namespace bbob {
  template <typename T, std::size_t N>
  std::array<T, N> random_parameter_translation();

  template <typename T>
  T random_objective_value_translation();

  template <typename T, std::size_t N>
  std::array<T, N> conditioning(
      const T condition_number);

  template <typename T, std::size_t N>
  std::array<T, N> condition(
      const typename std::array<T, N>::const_iterator iterator);

  template <typename T, std::size_t N>
  std::array<T, N> asymmetric(
      const T asymmetry,
      const typename std::array<T, N>::const_iterator iterator);

  template <typename T, std::size_t N>
  std::array<T, N> oscillate(
      const typename std::array<T, N>::const_iterator iterator);

  template <typename T>
  T oscillate(
      const T oscilliation);
}

//
// Implementation
//

template <typename T, std::size_t N>
black_box_optimisation_benchmark<T, N>::black_box_optimisation_benchmark() noexcept
    : optimisation_problem<T, N>() {
  this->lower_bounds.fill(-5.0);
  this->upper_bounds.fill(5.0);
  this->objective_value_translation = bbob::random_objective_value_translation<T>();
};

namespace bbob {
  template <typename T, std::size_t N>
  std::array<T, N> random_parameter_translation() {
    std::array<T, N> parameter_translation;
    for (decltype(N) n = 0; n < N; ++n) {
      parameter_translation[n] = std::uniform_real_distribution<T>(-4.0, 4.0)(random_number_generators()[thread_number()]);
      
      if (std::abs(parameter_translation[n]) < static_cast<T>(1e-4L)) {
        parameter_translation[n] = static_cast<T>(-1e-5L);
      } else {
        parameter_translation[n] = std::floor(parameter_translation[n] * 1e4) / 1e4;
      }
    }

    return parameter_translation;
  }

  template <typename T>
  T random_objective_value_translation() {
    return std::fmin(1000.0, std::fmax(-1000.0, std::floor(std::cauchy_distribution<T>(0.0, 100.0)(random_number_generators()[thread_number()]) * 1e2) / 1e2));
  }

  template <typename T, std::size_t N>
  std::array<T, N> conditioning(
      const T condition_number) {
    static_assert(N > 1, "The number of dimensions must be greater than 1.");
        
    std::array<T, N> conditioning;
    for (decltype(N) n = 0; n < N; ++n) {
      conditioning[n] = std::pow(condition_number, static_cast<T>(n) / static_cast<T>(N - 1));
    }

    return conditioning;
  }

  template <typename T, std::size_t N>
  std::array<T, N> condition(
      const typename std::array<T, N>::const_iterator iterator) {
    static_assert(N > 1, "The number of dimensions must be greater than 1.");

    std::array<T, N> conditioned_parameter;
    for (decltype(N) n = 0; n < N; ++n, ++iterator) {
      conditioned_parameter[n] = std::pow(*iterator, static_cast<T>(n) / static_cast<T>(N - 1));
    }

    return conditioned_parameter;
  }

  template <typename T, std::size_t N>
  std::array<T, N> asymmetric(
      const T asymmetry,
      const typename std::array<T, N>::const_iterator iterator) {
    static_assert(N > 1, "The number of dimensions must be greater than 1.");

    std::array<T, N> asymmetric_parameter;
    for (decltype(N) n = 0; n < N; ++n, ++iterator) {
      const auto element = *iterator;

      if (element > 0.0) {
        asymmetric_parameter[n] = std::pow(element, 1.0 + asymmetry * std::sqrt(element) * static_cast<T>(n) / static_cast<T>(N - 1));
      } else {
        asymmetric_parameter[n] = element;
      }
    }

    return asymmetric_parameter;
  }

  template <typename T, std::size_t N>
  std::array<T, N> oscillate(
      const typename std::array<T, N>::const_iterator iterator) {
    std::array<T, N> oscillated_parameter;
    for (decltype(N) n = 0; n < N; ++n, ++iterator) {
      oscillated_parameter[n] = oscillate(*iterator);
    }

    return oscillated_parameter;
  }

  template <typename T>
  T oscillate(
      const T oscilliation) {
    if (oscilliation != 0.0) {
      T c1;
      T c2;
      if (oscilliation > 0.0) {
        c1 = 10.0;
        c2 = static_cast<T>(7.9L);
      } else {
        c1 = 5.5;
        c2 = static_cast<T>(3.1);
      }

      const T logAbsoluteOscilliation = std::log(std::abs(oscilliation));
      return std::copysign(1.0, oscilliation) * std::exp(logAbsoluteOscilliation + static_cast<T>(0.049) * (std::sin(c1 * logAbsoluteOscilliation) + std::sin(c2 * logAbsoluteOscilliation)));
    } else {
      return 0.0;
    }
  }
}
