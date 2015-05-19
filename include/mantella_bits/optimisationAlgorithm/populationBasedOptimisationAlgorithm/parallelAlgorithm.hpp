namespace mant {
  template <typename T>
  class ParallelAlgorithm : public PopulationBasedOptimisationAlgorithm<T> {
    public:
      explicit ParallelAlgorithm(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const unsigned int populationSize) noexcept;

      unsigned int getRank() const noexcept;
      unsigned int getNumberOfNodes() const noexcept;

    protected:
      int rank_;
      int numberOfNodes_;

      void optimiseImplementation() noexcept final override;

      virtual void parallelOptimiseImplementation() noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename T>
  ParallelAlgorithm<T>::ParallelAlgorithm(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
  }

  template <typename T>
  void ParallelAlgorithm<T>::optimiseImplementation() noexcept {
    unsigned int serialisedOptimisationProblemSize;
    char* serialisedOptimisationProblemBuffer;

    if (rank_ == 0) {
      std::ostringstream output; {
        cereal::JSONOutputArchive archive(output);
        archive(OptimisationAlgorithm<T>::optimisationProblem_);
      };

      std::string serialisedOptimisationProblem = output.str();
      serialisedOptimisationProblemSize = serialisedOptimisationProblem.size();
      serialisedOptimisationProblemBuffer = std::strcpy(new char[serialisedOptimisationProblemSize + 1], serialisedOptimisationProblem.c_str());
    }

    MPI_Bcast(&serialisedOptimisationProblemSize, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    if (rank_ != 0) {
      serialisedOptimisationProblemBuffer = new char[serialisedOptimisationProblemSize + 1];
    }

    MPI_Bcast(serialisedOptimisationProblemBuffer, serialisedOptimisationProblemSize + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank_ != 0) {
      std::istringstream input(serialisedOptimisationProblemBuffer); {
        cereal::JSONInputArchive archive(input);
        archive(OptimisationAlgorithm<T>::optimisationProblem_);
      }
    }

    delete[](serialisedOptimisationProblemBuffer);

    parallelOptimiseImplementation();
  }

  template <typename T>
  unsigned int ParallelAlgorithm<T>::getRank() const noexcept {
    return rank_;
  }

  template <typename T>
  unsigned int ParallelAlgorithm<T>::getNumberOfNodes() const noexcept {
    return numberOfNodes_;
  }
}
