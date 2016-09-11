#pragma once

namespace mant {
  template <typename T1, typename T2, std::size_t N>
  struct OptimisationProblem {
    std::vector<std::pair<
        std::function<T2(
            const std::array<T1, N>& parameter)>,
        std::string>>
        objectiveFunctions;

    std::array<T1, N> lowerBounds;
    std::array<T1, N> upperBounds;

    std::array<std::size_t, N> parameterPermutation;
    std::array<T1, N> parameterScaling;
    std::array<T1, N> parameterTranslation;
    std::array<T1, N*N> parameterRotation;

    T2 objectiveValueScaling;
    T2 objectiveValueTranslation;
  };
}
