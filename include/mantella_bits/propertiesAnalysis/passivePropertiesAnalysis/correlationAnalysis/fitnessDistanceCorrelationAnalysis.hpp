namespace mant {
  class FitnessDistanceCorrelationAnalysis : public CorrelationAnalysis<double, EuclideanDistance> {
    public:
      using CorrelationAnalysis::CorrelationAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem)  override;
      void analyseImplementation(
          const std::unordered_map<arma::Col<double>, double, Hash, IsKeyEqual>& parameterToObjectiveValueMappings)  override;
      void analyseImplementation(
          const std::pair<arma::Col<double>, double>& parameterToObjectiveValueMapping)  override;
  };
}
