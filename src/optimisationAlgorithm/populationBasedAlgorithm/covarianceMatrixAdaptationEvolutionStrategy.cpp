#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp>

namespace hop {

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize)
  : PopulationBasedAlgorithm(optimisationProblem, populationSize) {
    setStepsize(0.3);
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    unsigned int numberOfDimensions = optimisationProblem_->getNumberOfDimensions();
    arma::Col<double> objectiveValues = arma::randu<arma::Col<double>>(numberOfDimensions);

    //init selection parameters
    //unsigned int lambda_ = 4 + std::floor(3 * std::log(numberOfDimensions)); //overwritten by constructor value
    double numberOfParents = populationSize_ / 2;
    arma::Col<double> weights = arma::Col<double>(numberOfParents);

    //starting the second log at 1 is important ;)
    for (std::size_t n = 0; n < weights.n_elem; ++n) {
      weights.at(n) = std::log(numberOfParents + 0.5) - std::log(n + 1);
    }

    numberOfParents = std::floor(numberOfParents);
    double weightSum = arma::sum(weights);
    for (std::size_t n = 0; n < weights.n_elem; ++n) {
      weights.at(n) = weights.at(n) / weightSum;
    }
    double varianceEffectiveness = std::pow(arma::sum(weights), 2) / arma::sum(arma::square(weights));
    //init adaptation parameters
    double cc = (4 + varianceEffectiveness / numberOfDimensions) / (numberOfDimensions + 4 + 2 * varianceEffectiveness / numberOfDimensions); //time constant for cumulation for C
    double cs = (varianceEffectiveness + 2) / (numberOfDimensions + varianceEffectiveness + 5); //t-const for cumulation for sigma control
    double c1 = 2 / (std::pow(numberOfDimensions + 1.3, 2) + varianceEffectiveness); //learning rate for rank-one update of C
    double cmu = std::min(1 - c1, 2 * (varianceEffectiveness - 2 + 1 / varianceEffectiveness) / (std::pow(numberOfDimensions + 2, 2) + varianceEffectiveness)); //and for rank-mu update
    double damps = 1 + 2 * std::max(0.0, std::sqrt((varianceEffectiveness - 1) / (numberOfDimensions + 1)) - 1) + cs; //damping for sigma 
    //init dynamic parameters and constants
    arma::Col<double> pc(numberOfDimensions, arma::fill::zeros);
    arma::Col<double> ps(numberOfDimensions, arma::fill::zeros);
    arma::Mat<double> B(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    arma::Col<double> D(numberOfDimensions, arma::fill::ones);
    arma::Mat<double> C(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    arma::Mat<double> invsqrtC(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    double chiN = std::sqrt(numberOfDimensions) * (1.0 - (1.0 / (4.0 * numberOfDimensions)) + (1.0 / (21.0 * std::pow(numberOfDimensions, 2))));

    //LOOP START
    while (!isFinished() && !isTerminated()) {
      //generate and evaluate lambda offspring
      arma::Col<double> arfitness = arma::Col<double>(populationSize_);
      arma::Mat<double> arx = arma::Mat<double>(numberOfDimensions, populationSize_);
      for (std::size_t n = 0; n < populationSize_; ++n) {
        arx.col(n) = objectiveValues + stepSize_ * B * (D % arma::randn<arma::Col<double>>(numberOfDimensions));
        arfitness(n) = optimisationProblem_->getObjectiveValue(arx.col(n));
        ++numberOfIterations_;

        //TODO: For performance, this could be done in another loop after the best have been selected. Not sure if really worth it
        if (arfitness(n) < bestObjectiveValue_) {
          bestObjectiveValue_ = arfitness(n);
          bestParameter_ = arx.col(n);
          //otherwise it's always infinity, so isFinished() doesn't work ;)
          bestSoftConstraintValue_ = 0;
        }

        if (isFinished() || isTerminated()) {
          break;
        }
      }
      
      //sort by fitness and compute weighted mean into objectiveValues
      arma::Col<arma::uword> arindex = arma::sort_index(arfitness);
      arma::Col<double> oldObjectiveValues = objectiveValues;
      //TODO: i don't know how to do a "cool" matlab style select like 1:lambda over arindex, so this is it
      // mind usage later on in adapt covariance
      arma::Col<arma::uword> shortArindex = arma::Col<arma::uword>(numberOfParents);
      for (int i = 0; i < numberOfParents; i++) {
        shortArindex.at(i) = arindex.at(i);
      }
      
      objectiveValues = arx.cols(shortArindex) * weights;
      
      //cumulation: update evolution paths
      ps = (1 - cs) * ps + std::sqrt(cs * (2 - cs) * varianceEffectiveness) * invsqrtC * (objectiveValues - oldObjectiveValues) / stepSize_;
      //original matlab line: hsig = norm(ps)/sqrt(1-(1-cs)^(2*numberOfIterations_/lambda))/chiN < 1.4 + 2/(numberOfDimensions+1);
      double hsigLeftSide = (arma::norm(ps) / std::sqrt(1 - std::pow((1 - cs), 2 * numberOfIterations_ / populationSize_))) / chiN;
      double hsigRightSide = 1.4 + 2 / (numberOfDimensions + 1.0);
      //TODO: hsig cannot be an int (will always be zero), although its a bool operation happening. Must be an arma thing. Maybe even a bug?
      double hsig = hsigLeftSide < hsigRightSide;

      pc = (1 - cc) * pc + hsig * std::sqrt(cc * (2 - cc) * varianceEffectiveness) * (objectiveValues - oldObjectiveValues) / stepSize_;

      //adapt covariance matrix C
      arma::Mat<double> artmp = (1 / stepSize_) * arx.cols(shortArindex) - arma::repmat(oldObjectiveValues, 1, numberOfParents);
      C = (1 - c1 - cmu) * C + //old matrix
          c1 * (pc * pc.t() + //rank one update
          (1 - hsig) * cc * (2 - cc) * C) + //correction for hsig==0
          cmu * artmp * arma::diagmat(weights) * artmp.t(); // rank mu update

      //adapt stepsize sigma
      stepSize_ = stepSize_ * std::exp((cs / damps) * (arma::norm(ps) / chiN - 1));

      //decomposition of C into B*diag(D.^2)*B' (diagonalization)
      //the first comparator element is actually numberOfIterations - eigeneval, but eigeneval only ever holds NOI - NOI last turn, which is the same as popSize
      //this only might get calculated when it shouldn't is if a acceptable value is found before we reach popSize. But this gets called every time with atleast dim>=30 anyway
      if (populationSize_ > (populationSize_ / (c1 + cmu) / (numberOfDimensions*1.0) / 10.0)) {
        C = arma::symmatu(C);
        //matlab code is [B,D] = eig(C); arma uses switched arguments
        arma::Col<double> tempD;
        arma::eig_sym(tempD, B, C);
        D = arma::sqrt(tempD);
        invsqrtC = B * arma::diagmat(1 / D) * B.t();
      }
    }
    //LOOP END
  }

  std::string CovarianceMatrixAdaptationEvolutionStrategy::to_string() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStepsize(double sigma) {
    stepSize_ = sigma;
  }
}
