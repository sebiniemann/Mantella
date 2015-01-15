#pragma once

// C++ Standard Library
#include <sstream>

// Cereal
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

// MPI
#include <mpi.h>

// Mantella
#include <mantella_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  template <typename ParameterType, class EuclideanDistance>
  class ParallelAlgorithm : public PopulationBasedAlgorithm<ParameterType, EuclideanDistance> {
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

  template <typename ParameterType, class EuclideanDistance>
  ParallelAlgorithm<ParameterType, EuclideanDistance>::ParallelAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : PopulationBasedAlgorithm<ParameterType, EuclideanDistance>(optimisationProblem, populationSize) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
  }

  template <typename ParameterType, class EuclideanDistance>
  void ParallelAlgorithm<ParameterType, EuclideanDistance>::optimiseImplementation() noexcept {
    unsigned int serialisedOptimisationProblemSize;
    char* serialisedOptimisationProblemBuffer;

    if (rank_ == 0) {
      std::ostringstream output; {
        cereal::JSONOutputArchive archive(output);
        archive(OptimisationAlgorithm<ParameterType, EuclideanDistance>::optimisationProblem_);
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
        archive(OptimisationAlgorithm<ParameterType, EuclideanDistance>::optimisationProblem_);
      }
    }

    delete[](serialisedOptimisationProblemBuffer);

    parallelOptimiseImplementation();
  }

  template <typename ParameterType, class EuclideanDistance>
  unsigned int ParallelAlgorithm<ParameterType, EuclideanDistance>::getRank() const noexcept {
    return rank_;
  }

  template <typename ParameterType, class EuclideanDistance>
  unsigned int ParallelAlgorithm<ParameterType, EuclideanDistance>::getNumberOfNodes() const noexcept {
    return numberOfNodes_;
  }
}
