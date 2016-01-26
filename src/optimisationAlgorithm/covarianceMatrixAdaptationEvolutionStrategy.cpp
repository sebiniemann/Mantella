#include "mantella_bits/optimisationAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy()
      : OptimisationAlgorithm(),
        lambda_(std::numeric_limits<arma::uword>::max()) {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters,
            const arma::Row<double>& objectiveValues,
            const arma::Row<double>& differences) {
          newGenerationValid_ = parameters; //arxvalid
          //std::cout << "newGenerationValid_" << newGenerationValid_ << std::endl;
          fitnessRaw_ = objectiveValues.t();
          //std::cout << "fitnessRaw_" << fitnessRaw_ << std::endl;

          //TODO: this stopflag is more sophisticated in the matlab code.
          if(stopFlag) {
              std::cout << "because errors on unused warnings" << std::endl;
            bestObjectiveValue_ = acceptableObjectiveValue_;
          }
          //;set internal parameters
          if (lambda_ != lambda_last_) {
              setPopulationSize(lambda_,numberOfDimensions);
          }

          countiter_++;

          //;Generate and evaluate lambda offspring

          //;----- handle boundaries -----
          //;Assigned penalized fitness
          arma::Col<double> boundaryPenalty = arma::sum(arma::abs(newGenerationValid_ - newGeneration_)).t();
          fitnessSel_ = fitnessRaw_ + boundaryPenalty;
          //;----- end handle boundaries -----

          //;Sort by fitness
          fitnessIdx_ = arma::sort_index(fitnessRaw_);
          //std::cout << "fitnessIdx_" << fitnessIdx_ << std::endl;
          fitnessRaw_ = arma::sort(fitnessRaw_);
          //std::cout << "fitnessRaw_" << fitnessRaw_ << std::endl;
          fitnessIdxSel_ = arma::sort_index(fitnessSel_);
          //std::cout << "fitnessIdxSel_" << fitnessIdxSel_ << std::endl;
          fitnessSel_ = arma::sort(fitnessSel_); //;minimization
          //std::cout << "fitnessSel_" << fitnessSel_ << std::endl;

          //;Calculate new xmean, this is selection and recombination 
          xold_ = xmean_; //;for speed up of Eq. (2) and (3)
          //std::cout << "xold_" << xold_ << std::endl;
          xmean_ = newGeneration_.cols(fitnessIdxSel_.rows(0, mu_ - 1)) * recombinationWeights_;
          //std::cout << "xmean_" << xmean_ << std::endl;
          arma::Mat<double> zmean = newGenerationRaw_.cols(fitnessIdxSel_.rows(0, mu_ - 1)) * recombinationWeights_; //;==D^-1*B'*(xmean-xold)/sigma
          //std::cout << "zmean" << zmean << std::endl;

          //;Cumulation: update evolution paths
          ps_ = (1 - cs_) * ps_ + std::sqrt(cs_ * (2 - cs_) * mueff_) * (B_ * zmean); //;Eq. (4)
          //std::cout << "ps_" << ps_ << std::endl;
          double hsig = arma::norm(ps_) / std::sqrt(1 - std::pow(1 - cs_, 2 * countiter_)) / chiN_ < 1.4 + 2.0 / (numberOfDimensions + 1);
          //std::cout << "hsig" << hsig << std::endl;

          pc_ = (1 - ccum_) * pc_ + hsig * (std::sqrt(ccum_ * (2 - ccum_) * mueff_) / sigma_) * (xmean_ - xold_); //;Eq. (2)
          //std::cout << "pc_" << pc_ << std::endl;

          //;Adapt covariance matrix
          negCcov_ = 0;

          if ((ccov1_ + ccovmu_) > 0) {
              arma::Mat<double> arpos = (static_cast<arma::Mat<double>>(newGeneration_.cols(fitnessIdxSel_.rows(0, mu_ - 1))).each_col() - xold_) / sigma_;
              //std::cout << "arpos" << arpos << std::endl;
              //;"active" CMA update: negative update, in case controlling pos. definiteness 
              if (activeCMA_) {
                  negCcov_ = (1 - ccovmu_) * 0.25 * mueff_ / (std::pow(numberOfDimensions + 2, 1.5) + 2 * mueff_);
                  double negAlphaOld = 0.5; //;where to make up for the variance loss, 0.5 means no idea what to do
                  //;1 is slightly more robust and gives a better "guaranty" for pos. def., 
                  //;but does it make sense from the learning perspective for large ccovmu?

                  //;prepare vectors, compute negative updating matrix Cneg and checking matrix Ccheck
                  arma::Mat<double> newGenerationRawNeg = newGenerationRaw_.cols(fitnessIdxSel_.rows(range(lambda_ -1,lambda_-mu_,1))); //arzneg
                  arma::Col<double> ngRawNegNorm = arma::sort(arma::sqrt(arma::sum(arma::square(newGenerationRawNeg), 0))).t(); //arnorms
                  arma::Col<double> ngRawNegNormFacs = ngRawNegNorm(range(ngRawNegNorm.n_elem-1,0,1)) / ngRawNegNorm; //arnormfacs
                  ngRawNegNorm = ngRawNegNormFacs; //;for the record
                  newGenerationRawNeg = newGenerationRawNeg.each_row() % ngRawNegNormFacs.t(); //;E x*x' is N
                  arma::Mat<double> Cneg = BD_ * newGenerationRawNeg * arma::diagmat(recombinationWeights_) * (BD_ * newGenerationRawNeg).t();
                  
                  //;update C
                  C_ = (1 - ccov1_ - ccovmu_ + negAlphaOld * negCcov_ + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
                          + ccov1_ * pc_ * pc_.t() //;plus rank one update
                          + (ccovmu_ + (1 - negAlphaOld) * negCcov_) //;plus rank mu update
                          * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_)
                          - negCcov_ * Cneg; //;minus rank mu update
                  
                  //if C isn't positive definite anymore, we do the normal C calculation
                  if(arma::min(arma::eig_gen(C_)).real() <= 0) {
                    //since we act here like activeCMA never happened, negCCov_ must be reset (it's used further down the line)
                    negCcov_ = 0;
                    C_ = (1 - ccov1_ - ccovmu_ + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
                          + ccov1_ * pc_ * pc_.t() //;plus rank one update
                          + ccovmu_ * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_); //;plus rank mu update
                  }
              } else { //; no active (negative) update
                  C_ = (1 - ccov1_ - ccovmu_ + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
                          + ccov1_ * pc_ * pc_.t() //;plus rank one update
                          + ccovmu_ * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_); //;plus rank mu update
                  //std::cout << "C_" << C_ << std::endl;
              }
              diagC_ = arma::diagvec(C_);
              //std::cout << "diagC_" << diagC_ << std::endl;
          }

          //;Adapt sigma
          sigma_ = sigma_ * std::exp(
                  std::min(1.0,
                  (std::sqrt(arma::accu(arma::square(ps_))) / chiN_ - 1) * cs_ / damping_)); //; Eq. (5)
          //std::cout << "sigma_" << sigma_ << std::endl;
          

          //;Update B and D from C
          if ((ccov1_ + ccovmu_ + negCcov_) > 0 && std::fmod(countiter_, (1 / ((ccov1_ + ccovmu_ + negCcov_) * numberOfDimensions * 10))) < 1) {
              C_ = arma::symmatu(C_); //;enforce symmetry to prevent complex numbers
              ////std::cout << "C_" << C_ << std::endl;
              arma::Col<double> tmp;
              //std::pair<arma::Mat<double>, arma::Col<double>> octaveEig = getOctaveEig(C_);
              //tmp = octaveEig.second;
              //B_ = octaveEig.first;
              arma::eig_sym(tmp, B_, C_); //;eigen decomposition, B==normalized eigenvectors
              //std::cout << "tmp" << tmp << std::endl;
              //std::cout << "B_" << B_ << std::endl;
              //;effort: approx. 15*N matrix-vector multiplications
              //Matlab returns eigenvalues as a diagonal matrix, so they do diagD = diag(tmp) here
              diagD_ = tmp;
              //std::cout << "diagD_" << diagD_ << std::endl;

              verify(arma::is_finite(diagD_),"diagD is not finite");
              verify(arma::is_finite(B_),"B is not finite");

              //;limit condition of C to 1e14 + 1
              //TODO: this can actually happen when an eigenvalue gets negative (which afaik can only happen if activeCMA is turned on)
              if (arma::min(diagD_) <= 0) {
                  if (stopOnWarnings_) {
                      stopFlag = true;
                  } else {
                      //TODO: warning gets thrown here
                      //another workaround
//                        //std::cout << "eigenvalue smaller zero" << std::endl;
//                        diagD_(arma::find(diagD_ < 0)) = arma::zeros(((arma::uvec)(arma::find(diagD_ < 0))).n_elem);
//                        double temp = 1.0*arma::max(diagD_) / 1e14;
//                        C_ = C_ + temp * arma::eye(numberOfDimensions, numberOfDimensions);
//                        diagD_ = diagD_ + temp * arma::ones(numberOfDimensions, 1);
                  }
              }
              if (arma::max(diagD_) > 1e14 * arma::min(diagD_)) {
                  if (stopOnWarnings_) {
                      stopFlag = true;
                  } else {
                      //TODO: warning gets thrown here
//                        //std::cout << "condition of c at upper limit" << std::endl;
//                        double temp = 1.0*arma::max(diagD_) / 1e14 - arma::min(diagD_);
//                        C_ = C_ + temp * arma::eye(numberOfDimensions, numberOfDimensions);
//                        diagD_ = diagD_ + temp * arma::ones(numberOfDimensions, 1);
                  }
              }

              diagC_ = arma::diagvec(C_);
              //std::cout << "diagC_" << diagC_ << std::endl;
              diagD_ = arma::sqrt(diagD_); //;D contains standard deviations now
              //std::cout << "diagD_" << diagD_ << std::endl;
              BD_ = B_.each_row() % diagD_.t();
              //std::cout << "BD_" << BD_ << std::endl;
          }
          
          //;----- numerical error management -----
          //;Adjust step size in case of (numerical) precision problem 
          arma::Mat<double> tmp = 0.1 * sigma_ * BD_.col(std::floor(countiter_ % numberOfDimensions));
          if (arma::all(xmean_ == xmean_ + tmp)) {
              if (stopOnWarnings_) {
                  stopFlag = true;
              } else {
                  //sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
              }
          }

          //;----- end numerical error management -----
          //;Set stop flag
          if (sigma_ * arma::max(diagD_) == 0) {//;should never happen
              stopFlag = true;
          }
          
          //TODO: This code was actually at the very beginning of the while loop
          //If HCMA changes lambda inbetween, this might need some handling
          newGenerationRaw_ = arma::randn(numberOfDimensions, lambda_); //arz
          //std::cout << "newGenerationRaw_" << newGenerationRaw_ << std::endl;
          newGeneration_ = static_cast<arma::Mat<double>>(sigma_ * (BD_ * newGenerationRaw_)).each_col() + xmean_; //arx
          //std::cout << "newGeneration_" << newGeneration_ << std::endl;
          
          return newGeneration_;
        });
    
    setInitialStepSize(0.4);
    setActiveCMA(false);
    
    //HCMA settings for later use:
    /*
         * setStepSize(0.2);
              //originally (getUpperBounds() - getLowerBounds()) / 5.0
              //but since all problems are within 0 to 1 now, we can just put in 0.2
        setActiveCMA(0);
        countiter_ = 0;
        stopOnStagnation_ = true;
        stopOnWarnings_ = true;
        stopOnEqualFunctionValues_ = true;
         */
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
      OptimisationProblem& optimisationProblem) {
    OptimisationAlgorithm::optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem.numberOfDimensions_));
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    //verify(initialParameters.n_cols == 1, "optimise: The cmaes algorithm accepts only a single initial parameter.");

    stopFlag = false;

    xmean_ = initialParameters.col(0);
    //std::cout << "xmean_" << xmean_ << std::endl;
    countiter_ = 0;
    //std::cout << "countiter_" << countiter_ << std::endl;
    sigma_ = initialSigma_;
    //std::cout << "sigma_" << sigma_ << std::endl;

    if (lambda_ == std::numeric_limits<arma::uword>::max()) {
      setPopulationSize(4 + std::floor(3 * std::log(numberOfDimensions)), numberOfDimensions);
    }

    pc_ = arma::zeros(numberOfDimensions);
    ps_ = arma::zeros(numberOfDimensions);

    diagD_ = arma::ones(numberOfDimensions);
    //std::cout << "diagD_" << diagD_ << std::endl;
    //TODO: "diagonal matrix D defines the scaling" originally this was stepsize / max(stepsize)
    //where stepsize was the vector (which never got used) of stepsizes in all dimensions
    //what actually got used was max(stepsize) - we decided to omit this vector so diagD as a whole
    //might be irrelevant
    diagC_ = arma::square(diagD_);
    //std::cout << "diagC_" << diagC_ << std::endl;
    B_ = arma::eye(numberOfDimensions, numberOfDimensions); //;B defines the coordinate system
    //std::cout << "B_" << B_ << std::endl;
    BD_ = arma::diagmat(diagD_); //;B*D for speed up only
    //std::cout << "BD_" << BD_ << std::endl;
    C_ = arma::diagmat(diagC_); //;covariance matrix == BD*(BD)'
    //std::cout << "C_" << C_ << std::endl;

    chiN_ = std::pow(numberOfDimensions, 0.5) *
            (1 - 1.0 / (4 * numberOfDimensions) + 1.0 / (21 * std::pow(numberOfDimensions, 2)));
    //std::cout << "chiN_" << chiN_ << std::endl;
    //;expectation of||N(0,I)|| == norm(randn(N,1))

    //miscellaneous inits needed
    stopOnWarnings_ = true;
    //more complicated in matlab, defines a number of iterations of equal funcvalues to stop

    //Need to do this here once to get from the initial starting point (given as intiailparameters)
    //to the first round of points to get evaluated
    newGenerationRaw_ = arma::randn(numberOfDimensions, lambda_); //arz
    //std::cout << "newGenerationRaw_" << newGenerationRaw_ << std::endl;
    newGeneration_ = static_cast<arma::Mat<double>>(sigma_ * (BD_ * newGenerationRaw_)).each_col() + xmean_; //arx
    //std::cout << "newGeneration_" << newGeneration_ << std::endl;

    //newGeneration will have turned into newGenerationValid_ when nextParameters is called
    //TODO: assign newGeneration to initialparameter somehow
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setInitialStepSize(const double stepSize) {
    initialSigma_ = stepSize;
  }
  
  double CovarianceMatrixAdaptationEvolutionStrategy::getInitialStepSize() {
    return initialSigma_;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getStepSize() {
    return sigma_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setPopulationSize(const arma::uword popSize, const arma::uword numberOfDimensions) {
    lambda_ = popSize;
    //std::cout << "lambda_" << lambda_ << std::endl;
    lambda_last_ = lambda_;
    mu_ = std::floor(lambda_ / 2.0);
    //std::cout << "mu_" << mu_ << std::endl;
    recombinationWeights_ = std::log(mu_ + 0.5) - arma::log(arma::linspace(1, mu_, mu_));
    //std::cout << "recombinationWeights_" << recombinationWeights_ << std::endl;
    mueff_ = std::pow(arma::accu(recombinationWeights_), 2) / arma::accu(arma::square(recombinationWeights_)); //;variance-effective size of mu
    //std::cout << "mueff_" << mueff_ << std::endl;
    recombinationWeights_ = arma::normalise(recombinationWeights_, 1); //;normalize recombination weights array
    //std::cout << "recombinationWeights_" << recombinationWeights_ << std::endl;
    //error check omitted, shouldn't happen

    //TODO: following code is dependent on the number of dimensions, so the user has to provide it here
    //(HCMA modifies popsize later, thats why the code cant be moved really)
    //TODO: this is the ccum calculation from hcma, original cmaes is: 4/(N+4)
    ccum_ = 4.0 / (numberOfDimensions + 4.0);
    //std::cout << "ccum_" << ccum_ << std::endl;
    //ccum_ = std::pow((numberOfDimensions + 2.0 * mueff_ / numberOfDimensions) / (4.0 + mueff_*1.0 / numberOfDimensions), -1); //;time constant for cumulation for covariance matrix
    cs_ = (mueff_ + 2) / (numberOfDimensions + mueff_ + 3);
    //std::cout << "cs_" << cs_ << std::endl;

    //TODO: this is the ccov1 calculation from hcma, original cmaes is: 2 / ((N+1.3)^2+mueff)
    ccov1_ = 2.0 / (std::pow(numberOfDimensions + 1.3, 2) + mueff_);
    //std::cout << "ccov1_" << ccov1_ << std::endl;
    //ccov1_ = std::min(2.0, lambda_ / 3.0) / (std::pow(numberOfDimensions + 1.3, 2) + mueff_);
    //TODO: this is the ccovmu calculation from hcma, original cmaes is: 2 * (mueff-2+1/mueff) / ((N+2)^2+mueff)
    ccovmu_ = std::min(1.0 - ccov1_, 2 * (mueff_ - 2 + 1.0 / mueff_) / (std::pow(numberOfDimensions + 2, 2) + mueff_));
    //std::cout << "ccovmu_" << ccovmu_ << std::endl;
    //ccovmu_ = std::min(2.0, lambda_ / 3.0) / (mueff_ - 2 + 1.0 / mueff_) / (std::pow(numberOfDimensions + 2, 2) + mueff_);

    damping_ = 1 + 2 * std::max(0.0, std::sqrt((mueff_ - 1) / (numberOfDimensions + 1)) - 1) + cs_;
    //std::cout << "damping_" << damping_ << std::endl;
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getPopulationSize() {
    return lambda_;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcov1() const {
    return ccov1_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcov1(double ccov1) {
    ccov1_ = ccov1;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcovmu() const {
    return ccovmu_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcovmu(double ccovmu) {
    ccovmu_ = ccovmu;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcum() const {
    return ccum_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcum(double ccum) {
    ccum_ = ccum;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCs() const {
    return cs_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCs(double cs) {
    cs_ = cs;
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getMu() const {
    return mu_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setMu(arma::uword numberOfParents) {
    mu_ = numberOfParents;
  }

  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getRecombinationWeights() {
    return recombinationWeights_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setRecombinationWeights(arma::Col<double> weights) {
    recombinationWeights_ = weights;
  }
  
  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getXmean() const {
    return xmean_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setXmean(arma::Col<double> xmean) {
    xmean_ = xmean;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setActiveCMA(bool activeCma) {
    activeCMA_ = activeCma;
  }

  bool CovarianceMatrixAdaptationEvolutionStrategy::isActiveCMA() {
    return activeCMA_;
  }
}
