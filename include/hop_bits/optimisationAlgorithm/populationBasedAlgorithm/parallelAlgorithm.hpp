#pragma once

// C++ Standard Library
#include <sstream>

// Cereal
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

// MPI
#include <mpi.h>

// HOP
#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  template <typename ParameterType>
  class ParallelAlgorithm : public PopulationBasedAlgorithm<ParameterType> {
    public:
      explicit ParallelAlgorithm(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      unsigned int getRank() const noexcept;
      unsigned int getNumberOfNodes() const noexcept;

    protected:
      int rank_;
      int numberOfNodes_;

      void optimiseImplementation() noexcept final override;

      virtual void parallelOptimiseImplementation() noexcept = 0;
  };

  template <typename ParameterType>
  ParallelAlgorithm<ParameterType>::ParallelAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : PopulationBasedAlgorithm<ParameterType>(optimisationProblem, populationSize) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
  }

  template <typename ParameterType>
  void ParallelAlgorithm<ParameterType>::optimiseImplementation() noexcept {
    unsigned int serialisedOptimisationProblemSize;
    char* serialisedOptimisationProblemBuffer;

    if (rank_ == 0) {
      std::ostringstream output; {
        cereal::JSONOutputArchive archive(output);
        archive(OptimisationAlgorithm<ParameterType>::optimisationProblem_);
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
        archive(OptimisationAlgorithm<ParameterType>::optimisationProblem_);
      }
    }

    delete[](serialisedOptimisationProblemBuffer);

    parallelOptimiseImplementation();
  }

  template <typename ParameterType>
  unsigned int ParallelAlgorithm<ParameterType>::getRank() const noexcept {
    return rank_;
  }

  template <typename ParameterType>
  unsigned int ParallelAlgorithm<ParameterType>::getNumberOfNodes() const noexcept {
    return numberOfNodes_;
  }
}
