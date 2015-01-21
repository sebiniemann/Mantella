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

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class ParallelAlgorithm : public PopulationBasedAlgorithm<ParameterType, DistanceFunction> {
    public:
      explicit ParallelAlgorithm(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
          const unsigned int& populationSize) ;

      unsigned int getRank() const ;
      unsigned int getNumberOfNodes() const ;

    protected:
      int rank_;
      int numberOfNodes_;

      void optimiseImplementation()  final override;

      virtual void parallelOptimiseImplementation()  = 0;
  };

  template <typename ParameterType, class DistanceFunction>
  ParallelAlgorithm<ParameterType, DistanceFunction>::ParallelAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem,
      const unsigned int& populationSize) 
    : PopulationBasedAlgorithm<ParameterType, DistanceFunction>(optimisationProblem, populationSize) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
  }

  template <typename ParameterType, class DistanceFunction>
  void ParallelAlgorithm<ParameterType, DistanceFunction>::optimiseImplementation()  {
    unsigned int serialisedOptimisationProblemSize;
    char* serialisedOptimisationProblemBuffer;

    if (rank_ == 0) {
      std::ostringstream output; {
        cereal::JSONOutputArchive archive(output);
        archive(OptimisationAlgorithm<ParameterType, DistanceFunction>::optimisationProblem_);
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
        archive(OptimisationAlgorithm<ParameterType, DistanceFunction>::optimisationProblem_);
      }
    }

    delete[](serialisedOptimisationProblemBuffer);

    parallelOptimiseImplementation();
  }

  template <typename ParameterType, class DistanceFunction>
  unsigned int ParallelAlgorithm<ParameterType, DistanceFunction>::getRank() const  {
    return rank_;
  }

  template <typename ParameterType, class DistanceFunction>
  unsigned int ParallelAlgorithm<ParameterType, DistanceFunction>::getNumberOfNodes() const  {
    return numberOfNodes_;
  }
}
