#include <hop_bits/optimisationAlgorithm/parallelOptimisationAlgorithm.hpp>

#include <sstream>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <mpi.h>

namespace hop {
  ParallelOptimisationAlgorithm::ParallelOptimisationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    :  OptimisationAlgorithm(optimisationProblem) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
  }

  void ParallelOptimisationAlgorithm::optimiseImplementation() {

    unsigned int serialisedOptimisationProblemLength;
    char* serialisedOptimisationProblemBuffer;

    if(rank_ == 0) {
      std::ostringstream output; {
        cereal::JSONOutputArchive archive(output);
        archive(optimisationProblem_);
      };

      std::string serialisedOptimisationProblem = output.str();
      serialisedOptimisationProblemLength = serialisedOptimisationProblem.size();
      serialisedOptimisationProblemBuffer = std::strcpy(new char[serialisedOptimisationProblemLength + 1], serialisedOptimisationProblem.c_str());
    }

    MPI_Bcast(&serialisedOptimisationProblemLength, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    if(rank_ != 0) {
      serialisedOptimisationProblemBuffer = new char[serialisedOptimisationProblemLength + 1];
    }

    MPI_Bcast(serialisedOptimisationProblemBuffer, serialisedOptimisationProblemLength + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    if(rank_ != 0) {
      std::istringstream input(serialisedOptimisationProblemBuffer); {
        cereal::JSONInputArchive archive(input);
        archive(optimisationProblem_);
      }
    }

    free(serialisedOptimisationProblemBuffer);

    parallelOptimiseImplementation();
  }

  int ParallelOptimisationAlgorithm::getRank() const {
    return rank_;
  }
}
