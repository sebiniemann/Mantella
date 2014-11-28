#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp>

namespace hop {

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize, const double stepSize)
  : PopulationBasedAlgorithm(optimisationProblem, populationSize), sigma_(stepSize) {
    // init input parameters
    // TODO: iteration number gets multiplied with dimensions² on wiki, also do that here?
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    unsigned int numberOfDimensions = optimisationProblem_->getNumberOfDimensions();
    std::cout << "numberOfDims set\n";
    arma::Col<double> objectiveValues = arma::randu<arma::vec>(numberOfDimensions);
    std::cout << "objectiveValues set\n";

    //init selection parameters
    unsigned int lambda = 4 + std::floor(3 * std::log(numberOfDimensions));
    double mu = lambda / 2;
    arma::Col<double> weights = arma::Col<double>(mu);

    //starting the second log at 1 is important ;)
    for (std::size_t n = 0; n < weights.n_elem; ++n) {
      weights.at(n) = std::log(mu + 0.5) - std::log(n+1);
    }

    mu = std::floor(mu);
    double weightSum = arma::sum(weights);
    for (std::size_t n = 0; n < weights.n_elem; ++n) {
      weights.at(n) = weights.at(n) / weightSum;
    }
    double mueff = std::pow(arma::sum(weights), 2) / arma::sum(arma::square(weights));
    std::cout << "selection parameters set\n";
    //init adaptation parameters
    double cc = (4 + mueff / numberOfDimensions) / (numberOfDimensions + 4 + 2 * mueff / numberOfDimensions);
    double cs = (mueff + 2) / (numberOfDimensions + mueff + 5);
    double c1 = 2 / (std::pow(numberOfDimensions + 1.3, 2) + mueff);
    double cmu = std::min(1 - c1, 2 * (mueff - 2 + 1 / mueff) / (std::pow(numberOfDimensions + 2, 2) + mueff));
    double damps = 1 + 2 * std::max(0.0, std::sqrt((mueff - 1) / (numberOfDimensions + 1)) - 1) + cs;
    std::cout << "adaptation parameters set\n";
    //init dynamic parameters and constants
    arma::Col<double> pc(numberOfDimensions, arma::fill::zeros);
    arma::Col<double> ps(numberOfDimensions, arma::fill::zeros);
    arma::Mat<double> B(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    arma::Col<double> D(numberOfDimensions, arma::fill::ones);
    arma::Mat<double> C(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    arma::Mat<double> invsqrtC(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    double eigeneval = 0;
    double chiN = std::sqrt(numberOfDimensions) * (1 - 1 / (4 * numberOfDimensions) + 1 / (21 * std::pow(numberOfDimensions, 2)));
    std::cout << "dynamic parameters set\n";

    while (!isFinished() && !isTerminated()) {
      //generate and evaluate lambda offspring
      std::cout << "generating offspring\n";
      arma::Col<double> arfitness = arma::Col<double>(lambda);
      arma::Mat<double> arx = arma::Mat<double>(numberOfDimensions, lambda);
      for (std::size_t n = 0; n < lambda; ++n) {
        arx.col(n) = objectiveValues + sigma_ * B * (D % arma::randn<arma::Col<double>>(numberOfDimensions));
        arfitness(n) = optimisationProblem_->getObjectiveValue(arx.col(n));
        ++numberOfIterations_;
        
        if (arfitness(n) < bestObjectiveValue_) {
          bestObjectiveValue_ = arfitness(n);
          bestParameter_ = arx.col(n);
        }
        
        if (isFinished() || isTerminated()) {
          break;
        }
      }
      
      std::cout << arfitness << std::endl;
      std::cout << arx << std::endl;
      
      std::cout << "sort fitness\n";
      //sort by fitness and compute weighted mean into objectiveValues
      arma::Col<arma::uword> arindex = arma::sort_index(arfitness);
      arma::Col<double> oldObjectiveValues = objectiveValues;
      //next line should be unnecessary, depending on if arma returns a copy or not in the line after that
      objectiveValues = arma::Col<double>(numberOfDimensions);
      //TODO: xmean = arx(:,arindex(1:mu))*weights;
      //is the actual matlab syntax for the following line, not a 100% sure if this is correct
      std::cout << "critical matrix\n";
      std::cout << "arx: " << arx.n_cols << "," << arx.n_rows << "\n";
      std::cout << "weights: " << weights.n_cols << "," << weights.n_rows << "\n";
      std::cout << "arindex: " << arindex.n_elem << "\n";
      std::cout << "xmean: " << objectiveValues.n_cols << "," << objectiveValues.n_rows << "\n";
      std::cout << "lambda: " << lambda << "\n";
      std::cout << "mu: " << mu << "\n";
      std::cout << "mueff: " << mueff << "\n";
      for (int i = 0; i < arindex.n_elem; i++) {
        std::cout << arindex(i) << ", ";
      }
      
      std::cout << weights << std::endl;
      
      //TODO: i don't know how to do a "cool" matlab style select like 1:lambda over arindex, so this is it
      // mind usage later on in adapt covariance
      arma::Col<arma::uword> shortArindex = arma::Col<arma::uword>(mu);
      for(int i = 0; i < mu; i++) {
        shortArindex.at(i) = arindex.at(i);
      }
      
      arma::Mat<double> temp = arx.cols(shortArindex);
      
      std::cout << "shortArindex: " << shortArindex.n_cols << "," << shortArindex.n_rows << "\n";
      
      std::cout << "temp: " << temp.n_cols << "," << temp.n_rows << "\n";
      
      objectiveValues = arx.cols(shortArindex) * weights;

      std::cout << "update evo paths\n";
      //cumulation: update evolution paths
      ps = (1 - cs) * ps + std::sqrt(cs * (2 - cs) * mueff) * invsqrtC * (objectiveValues - oldObjectiveValues) / sigma_;
      int hsig = arma::norm(ps) / std::sqrt(std::pow(1 - (1 - cs), 2 * numberOfIterations_ / lambda)) / chiN < 1.4 + 2 / (numberOfDimensions + 1);
      
      std::cout << pc << std::endl;
      std::cout << sigma_ << std::endl;
      
      pc = (1 - cc) * pc + hsig * std::sqrt(cc * (2 - cc) * mueff) * (objectiveValues - oldObjectiveValues) / sigma_;
      
      std::cout << "adapt covariance\n";
      //adapt covariance matrix C
      arma::Mat<double> artmp = (1 / sigma_) * arx.cols(shortArindex) - arma::repmat(oldObjectiveValues, 1, mu);
      std::cout << "first works" << std::endl;
      C = (1 - c1 - cmu) * C + //old matrix
          c1 * (pc * pc.t() + //rank one update
          (1 - hsig) * cc * (2 - cc) * C) + //correction for hsig==0
          cmu * artmp * arma::diagmat(weights) * artmp.t(); // rank mu update

      std::cout << "adapt stepsize\n";
      //adapt stepsize sigma
      sigma_ = sigma_ * std::exp((cs / damps) * (arma::norm(ps) / chiN - 1));

      std::cout << "decomp c into b\n";
      //decomposition of C into B*diag(D.^2)*B' (diagonalization)
      if ((numberOfIterations_ - eigeneval) > (lambda / (c1 + cmu) / numberOfIterations_ / 10)) {
        eigeneval = numberOfIterations_;
        //TODO: C = triu(C) + triu(C,1)';
        //is the actual matlab syntax for the next lines, unfortunately arma doesn't have triu with a second parameter (yet)
        C = arma::symmatu(C);
        //matlab code is [B,D] = eig(C); arma uses switched arguments according to API
        arma::Col<double> tempD;
        //TODO: which eig is right? eig_sym failed to converge - eigs_gen and eigs_sym need a number of eigenvalues to produce...
        //also eigs_sym/eigs_gen fail because no matching function found when adding numberofDims as k. not sure why?
        std::cout << C << std::endl;
        arma::eig_sym(tempD, B, C);
        std::cout << "Test A" << std::endl;
        D = arma::sqrt(tempD); // TODO: From wiki "D is a vector of standard deviations now" which it obviously isn't here.
        std::cout << "Test B" << std::endl;
        // also not sure what the point of that is, since it's going to be used as a matrix in the next line anyway.
        invsqrtC = B * arma::diagmat(1 / D) * B.t();
        std::cout << "Test C" << std::endl;
      }
    }
    //while loop has ended
    std::cout << "loop end\n";

    //TODO: matlab returns xmin = arx(:, arindex(1));
    //safe best objective value, etc
  }

  std::string CovarianceMatrixAdaptationEvolutionStrategy::to_string() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }
}
