#pragma once

namespace mant {
  double evaluate(
  
  optimisation_result optimise(
    const optimisation_problem& optimisation_problem,
    const optimisation_algorithm& optimisation_algorithm);
  
  
  
  template <typename T1, typename T2, std::size_t N>
  struct OptimisationAlgorithm {
    std::vector<std::pair<
        std::function<arma::mat(
            const std::vector<std::array<T1, N>>& initialParameters)>,
        std::string>>
        initialisingFunctions;

    std::vector<std::pair<
        std::function<arma::mat(
            const std::vector<std::array<T1, N>>& parameters,
            const std::vector<T2>& objectiveValues,
            const std::vector<T2>& differences)>,
        std::string>>
        nextParametersFunctions;

    std::vector<std::pair<
        std::function<arma::mat(
            const std::vector<std::array<T1, N>>& parameters,
            const std::vector<std::bitset<N>>& isBelowLowerBound,
            const std::vector<std::bitset<N>>& isAboveUpperBound)>,
        std::string>>
        boundariesHandlingFunctions;

    std::vector<std::pair<
        std::function<bool(
            const std::vector<std::array<T1, N>>& parameters,
            const std::vector<T2>& objectiveValues,
            const std::vector<T2>& differences)>,
        std::string>>
        isStagnatingFunctions;

    std::vector<std::pair<
        std::function<arma::mat(
            const std::vector<std::array<T1, N>>& parameters,
            const std::vector<T2>& objectiveValues,
            const std::vector<T2>& differences)>,
        std::string>>
        restartingFunctions;

    std::vector<std::pair<
        std::function<arma::mat(
            const std::vector<std::array<T1, N>>& parameters,
            const std::vector<T2>& objectiveValues,
            const std::vector<T2>& differences)>,
        std::string>>
        communicationFunctions;

    T2 acceptableObjectiveValue;
    std::intmax_t maximalNumberOfIterations;
    std::chrono::microseconds maximalDuration;

    std::size_t populationSize;
    std::intmax_t numberOfCommunicationStalls;
  };
}
