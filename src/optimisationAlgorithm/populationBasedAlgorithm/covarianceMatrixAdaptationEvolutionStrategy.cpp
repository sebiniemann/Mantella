#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp>

namespace hop {
  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize) : PopulationBasedAlgorithm(optimisationProblem, populationSize) {
    // init input parameters
    // TODO: iteration number gets multiplied with dimensions² on wiki, also do that here?
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    unsigned int numberOfDimensions = optimisationProblem_->getNumberOfDimensions();
    
    arma::Col<double> objectiveValues = arma::randu<arma::vec>(numberOfDimensions);
    double sigma = 0.3;
    //init selection parameters
    unsigned int lambda = 4 + std::floor(3 * std::log(numberOfDimensions));
    double mu = lambda / 2;
    arma::Col<double> weights = arma::Col<double>(mu);
    
    for(std::size_t n = 0; n < weights.n_elem; ++n) {
      weights.at(n) = std::log(mu + 0.5) - std::log(n);
    }
    
    mu = std::floor(mu);
    weights = weights / arma::sum(weights);
    double mueff = std::pow(arma::sum(weights), 2) / arma::sum(arma::square(weights));
    //init adaptation parameters
    double cc = (4 + mueff / numberOfDimensions) / (numberOfDimensions + 4 + 2 * mueff / numberOfDimensions);
    double cs = (mueff+2) / (numberOfDimensions + mueff + 5);
    double c1 = 2 / (std::pow(numberOfDimensions + 1.3, 2) + mueff);
    double cmu = std::min(1 - c1, 2 * (mueff - 2 + 1 / mueff) / (std::pow(numberOfDimensions + 2, 2) + mueff));
    double damps = 1 + 2 * std::max(0.0, std::sqrt((mueff - 1) / (numberOfDimensions + 1)) - 1) + cs;
    //init dynamic parameters and constants
    arma::Col<double> pc(numberOfDimensions, arma::fill::zeros);
    arma::Col<double> ps(numberOfDimensions, arma::fill::zeros);
    arma::Mat<double> B(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    arma::Col<double> D(numberOfDimensions, arma::fill::ones);
    arma::Mat<double> C(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    arma::Mat<double> invsqrtC(numberOfDimensions, numberOfDimensions, arma::fill::eye);
    double eigeneval = 0;
    double chiN = std::sqrt(numberOfDimensions) * (1 - 1 / (4 * numberOfDimensions) + 1 / (21 * std::pow(numberOfDimensions, 2)));
  
    while(!isFinished() && !isTerminated()) {
      //generate and evaluate lambda offspring
      arma::Col<double> arfitness = arma::Col<double>(lambda);
      arma::Mat<double> arx = arma::Mat<double>(numberOfDimensions, lambda);
      for(std::size_t n = 0; n < lambda; ++n) {
        arx.col(n) = objectiveValues + sigma * B * (D % arma::randn<arma::Col<double>>(numberOfDimensions));
        arfitness(n) = optimisationProblem_->getObjectiveValue(arx.col(n));
        ++numberOfIterations_;
      }
      //sort by fitness and compute weighted mean into objectiveValues
      arma::Col<arma::uword> arindex = arma::sort_index(arfitness);
      arma::Col<double> oldObjectiveValues = objectiveValues;
      //next line should be unnecessary, depending on if arma returns a copy or not in the line after that
      objectiveValues = arma::Col<double>(numberOfDimensions);
      //TODO: xmean = arx(:,arindex(1:mu))*weights;
      //is the actual matlab syntax for the following line, not a 100% sure if this is correct
      objectiveValues = arx.cols(arindex) * weights;

      //cumulation: update evolution paths
      ps = (1 - cs) * ps + std::sqrt(cs * (2 - cs) * mueff) * invsqrtC * (objectiveValues - oldObjectiveValues) / sigma;
      int hsig = arma::norm(ps) / std::sqrt(std::pow(1 - (1 - cs), 2 * numberOfIterations_ / lambda)) / chiN < 1.4 + 2 / (numberOfDimensions + 1);
      pc = (1 - cc) * pc + hsig * std::sqrt(cc * (2-cc) * mueff) * (objectiveValues - oldObjectiveValues) / sigma;

      //adapt covariance matrix C
      arma::Mat<double> artmp = (1 / sigma) * arx.cols(arindex) - arma::repmat(oldObjectiveValues, 1, mu);
      C = (1-c1-cmu) * C + //old matrix
              c1 * (pc*pc.t() + //rank one update
                     (1-hsig) * cc * (2-cc) * C) + //correction for hsig==0
              cmu * artmp * arma::diagmat(weights) * artmp.t(); // rank mu update

      //adapt step size sigma
      sigma = sigma * std::exp((cs / damps) * (arma::norm(ps) / chiN - 1));

      //decomposition of C into B*diag(D.^2)*B' (diagonalization)
      if((numberOfIterations_ - eigeneval) > (lambda / (c1 + cmu) / numberOfIterations_ / 10)) {
        eigeneval = numberOfIterations_;
        //TODO: C = triu(C) + triu(C,1)';
        //is the actual matlab syntax for the next lines, unfortunately arma doesn't have triu with a second parameter (yet)
        arma::Mat<double> tempC = arma::trimatu(C);
        tempC.diag().zeros();
        C = arma::trimatu(C) + tempC.t(); //enforce symmetry
        //matlab code is [B,D] = eig(C); arma uses switched arguments according to API
        arma::Col<double> tempD = arma::Col<double>();
        arma::eig_sym(tempD, B, C);
        D = arma::sqrt(arma::diagmat(tempD)); // TODO: From wiki "D is a vector of standard deviations now" which it obviously isn't here.
                                              // also not sure what the point of that is, since it's going to be used as a matrix in the next line anyway.
        invsqrtC = B * arma::diagmat(1 / D) * B.t();
      }
    }
    //while loop has ended

    //TODO: matlab returns xmin = arx(:, arindex(1));
    //not sure where to safe our return value?
  }

  std::string CovarianceMatrixAdaptationEvolutionStrategy::to_string() const {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }
}
