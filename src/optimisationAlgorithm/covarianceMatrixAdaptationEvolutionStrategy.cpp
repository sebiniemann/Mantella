#include "mantella_bits/optimisationAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp"

// C++ standard library
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy()
      : OptimisationAlgorithm(),
        populationSize_(std::numeric_limits<arma::uword>::max()) {
    setIsStagnatingFunction(
        [this](
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          assert(objectiveValues_.n_elem == parameters_.n_cols);
          assert(differences_.n_elem == parameters_.n_cols);
          assert(differences_.has_inf() || arma::all(objectiveValues_ - differences_ - arma::min(objectiveValues_) + arma::min(differences_) < 1e-12 * arma::max(arma::ones<arma::Row<double>>(arma::size(objectiveValues_)), arma::abs(objectiveValues_))));
          
          //encountered a hard error, like eigenwert < 0
          if(stepSize_ <= 0) {
            return true;
          }
          //1e14 is a magic threshold decided by Hansen to limit the condition by
          if (arma::max(diagD_) > 1e14 * arma::min(diagD_)) {
            return true;
          }
          //0.1 is a similar number as above
          if (arma::all(xmean_ == xmean_ + 0.1 * stepSize_ * BD_.col((numberOfIterations_+1) % objectiveValues_.n_elem))) {
            return true;
          }
          
          return false;
        });

    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters,
            const arma::Row<double>& objectiveValues,
            const arma::Row<double>& differences) {
          newGenerationValid_ = parameters; //arxvalid
          arma::Col<double> fitnessRaw = objectiveValues.t();

          //;set internal parameters
          if (populationSize_ != populationSizeLast_) {
            setPopulationSize(populationSize_,numberOfDimensions);
          }

          //;----- handle boundaries -----
          //;Assigned penalized fitness
          arma::Col<double> boundaryPenalty = arma::sum(arma::abs(newGenerationValid_ - newGeneration_)).t();
          arma::Col<double> fitnessSel = fitnessRaw + boundaryPenalty;
          //;----- end handle boundaries -----

          //;Sort by fitness
          arma::Col<arma::uword> fitnessIdx = arma::sort_index(fitnessRaw);
          fitnessRaw = arma::sort(fitnessRaw);
          arma::Col<arma::uword> fitnessIdxSel = arma::sort_index(fitnessSel);
          fitnessSel = arma::sort(fitnessSel); //;minimization
          
          //;Calculate new xmean, this is selection and recombination 
          xold_ = xmean_; //;for speed up of Eq. (2) and (3)
          xmean_ = newGeneration_.cols(fitnessIdxSel.rows(0, mu_ - 1)) * recombinationWeights_;
          arma::Mat<double> zmean = newGenerationRaw_.cols(fitnessIdxSel.rows(0, mu_ - 1)) * recombinationWeights_; //;==D^-1*B'*(xmean-xold)/sigma
          
          //;Cumulation: update evolution paths
          ps_ = (1 - cs_) * ps_ + std::sqrt(cs_ * (2 - cs_) * mueff_) * (B_ * zmean); //;Eq. (4)
          double hsig = arma::norm(ps_) / std::sqrt(1 - std::pow(1 - cs_, 2 * (numberOfIterations_+1))) / chiN_ < 1.4 + 2.0 / (numberOfDimensions + 1);
          pc_ = (1 - ccum_) * pc_ + hsig * (std::sqrt(ccum_ * (2 - ccum_) * mueff_) / stepSize_) * (xmean_ - xold_); //;Eq. (2)
          
          //;Adapt covariance matrix
          negCcov_ = 0;
          if ((ccov1_ + ccovmu_) > 0) {
            arma::Mat<double> arpos = (static_cast<arma::Mat<double>>(newGeneration_.cols(fitnessIdxSel.rows(0, mu_ - 1))).each_col() - xold_) / stepSize_;
            negCcov_ = (1 - ccovmu_) * 0.25 * mueff_ / (std::pow(numberOfDimensions + 2, 1.5) + 2 * mueff_);
            double negAlphaOld = 0.5; //;where to make up for the variance loss, 0.5 means no idea what to do
            //;1 is slightly more robust and gives a better "guaranty" for pos. def., 
            //;but does it make sense from the learning perspective for large ccovmu?

            //;prepare vectors, compute negative updating matrix Cneg
            arma::Mat<double> newGenerationRawNeg = newGenerationRaw_.cols(fitnessIdxSel.rows(range(populationSize_ -1,populationSize_-mu_,1))); //arzneg
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
            diagC_ = arma::diagvec(C_);
          }

          //;Adapt sigma
          stepSize_ = stepSize_ * std::exp(std::min(1.0,
                  (std::sqrt(arma::accu(arma::square(ps_))) / chiN_ - 1) * cs_ / damping_)); //; Eq. (5)
          

          //;Update B and D from C
          if ((ccov1_ + ccovmu_ + negCcov_) > 0 && std::fmod((numberOfIterations_+1), (1 / ((ccov1_ + ccovmu_ + negCcov_) * numberOfDimensions * 10))) < 1) {
            C_ = arma::symmatu(C_); //;enforce symmetry to prevent complex numbers
            arma::Col<double> tmp;
            arma::eig_sym(tmp, B_, C_); //;eigen decomposition, B==normalized eigenvectors
            diagD_ = tmp;

            verify(arma::is_finite(diagD_),"diagD is not finite");
            verify(arma::is_finite(B_),"B is not finite");

            //negative eigenvalue handling, usually means algorithm is stuck
            if (arma::min(diagD_) <= 0) {
              stepSize_ = 0;
              return static_cast<arma::Mat<double>>(arma::randu(numberOfDimensions, populationSize_));
            }

            diagC_ = arma::diagvec(C_);
            diagD_ = arma::sqrt(diagD_); //;D contains standard deviations now
            BD_ = B_.each_row() % diagD_.t();
          }

          if (stepSize_ * arma::max(diagD_) <= 0) {//;should never happen
            stepSize_ = 0;
            return static_cast<arma::Mat<double>>(arma::randu(numberOfDimensions, populationSize_));
          }
          
          //TODO: This code was actually at the very beginning of the while loop
          //If HCMA changes lambda inbetween, this might need some handling
          newGenerationRaw_ = arma::randn(numberOfDimensions, populationSize_); //arz
          newGeneration_ = static_cast<arma::Mat<double>>(stepSize_ * (BD_ * newGenerationRaw_)).each_col() + xmean_; //arx
          
          return newGeneration_;
        });

    setInitialStepSize(0.4);
  }

  //nothing provided
  void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
      OptimisationProblem& optimisationProblem) {
    arma::uword numberOfDimensions = optimisationProblem.numberOfDimensions_;
    stepSize_ = initialStepSize_;
    if (populationSize_ == std::numeric_limits<arma::uword>::max()) {
      setPopulationSize(4 + static_cast<arma::uword>(std::floor(3 * std::log(numberOfDimensions))), numberOfDimensions);
    }
    xmean_ = arma::randu(numberOfDimensions);
    newGenerationRaw_ = arma::randn<arma::Mat<double>>(numberOfDimensions, populationSize_);
    newGeneration_ = static_cast<arma::Mat<double>>(stepSize_ * (arma::eye(numberOfDimensions, numberOfDimensions) * newGenerationRaw_)).each_col() + xmean_;

    OptimisationAlgorithm::optimise(optimisationProblem, newGeneration_);
  }

  //popSize provided
  void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::uword populationSize) {
    arma::uword numberOfDimensions = optimisationProblem.numberOfDimensions_;
    stepSize_ = initialStepSize_;
    setPopulationSize(populationSize, numberOfDimensions);
    xmean_ = arma::randu(numberOfDimensions);
    newGenerationRaw_ = arma::randn<arma::Mat<double>>(numberOfDimensions, populationSize_);
    newGeneration_ = static_cast<arma::Mat<double>>(stepSize_ * (arma::eye(numberOfDimensions, numberOfDimensions) * newGenerationRaw_)).each_col() + xmean_;

    OptimisationAlgorithm::optimise(optimisationProblem, newGeneration_);
  }

  //mean provided
  void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Col<double>& meanParameter) {
    arma::uword numberOfDimensions = optimisationProblem.numberOfDimensions_;
    stepSize_ = initialStepSize_;
    if (populationSize_ == std::numeric_limits<arma::uword>::max()) {
      setPopulationSize(4 + static_cast<arma::uword>(std::floor(3 * std::log(numberOfDimensions))), numberOfDimensions);
    }
    xmean_ = meanParameter;
    newGenerationRaw_ = arma::randn<arma::Mat<double>>(numberOfDimensions, populationSize_);
    newGeneration_ = static_cast<arma::Mat<double>>(stepSize_ * (arma::eye(numberOfDimensions, numberOfDimensions) * newGenerationRaw_)).each_col() + xmean_;

    OptimisationAlgorithm::optimise(optimisationProblem, newGeneration_);
  }

  //initialParameters provided
  void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    stepSize_ = initialStepSize_;
    populationSize_ = initialParameters.n_cols;
    xmean_ = arma::mean(initialParameters).t();
    newGeneration_ = initialParameters;

    OptimisationAlgorithm::optimise(optimisationProblem, newGeneration_);
  }

  //mean and popsize provided
  void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::Col<double>& meanParameter,
      const arma::uword populationSize) {
    arma::uword numberOfDimensions = optimisationProblem.numberOfDimensions_;
    stepSize_ = initialStepSize_;
    setPopulationSize(populationSize, numberOfDimensions);
    xmean_ = meanParameter;
    newGenerationRaw_ = arma::randn<arma::Mat<double>>(numberOfDimensions, populationSize_);
    newGeneration_ = static_cast<arma::Mat<double>>(stepSize_ * (arma::eye(numberOfDimensions, numberOfDimensions) * newGenerationRaw_)).each_col() + xmean_;

    OptimisationAlgorithm::optimise(optimisationProblem, newGeneration_);
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    if (populationSize_ == std::numeric_limits<arma::uword>::max()) {
      setPopulationSize(4 + static_cast<arma::uword>(std::floor(3 * std::log(numberOfDimensions))), numberOfDimensions);
    }

    pc_ = arma::zeros(numberOfDimensions);
    ps_ = arma::zeros(numberOfDimensions);

    diagD_ = arma::ones(numberOfDimensions);
    diagC_ = arma::square(diagD_);
    B_ = arma::eye(numberOfDimensions, numberOfDimensions); //;B defines the coordinate system
    BD_ = arma::diagmat(diagD_); //;B*D for speed up only
    C_ = arma::diagmat(diagC_); //;covariance matrix == BD*(BD)'

    chiN_ = std::pow(numberOfDimensions, 0.5) *
            (1 - 1.0 / (4 * numberOfDimensions) + 1.0 / (21 * std::pow(numberOfDimensions, 2)));
    //;expectation of||N(0,I)|| == norm(randn(N,1))

    newGenerationRaw_ = (initialParameters.each_col() - xmean_) / stepSize_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setInitialStepSize(const double initialStepSize) {
    initialStepSize_ = initialStepSize;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getInitialStepSize() {
    return initialStepSize_;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getStepSize() {
    return stepSize_;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setPopulationSize(const arma::uword populationSize, const arma::uword numberOfDimensions) {
    populationSize_ = populationSize;
    populationSizeLast_ = populationSize_;
    mu_ = static_cast<arma::uword>(std::floor(populationSize_ / 2.0));
    recombinationWeights_ = std::log(mu_ + 0.5) - arma::log(arma::linspace(1, mu_, mu_));
    mueff_ = std::pow(arma::accu(recombinationWeights_), 2) / arma::accu(arma::square(recombinationWeights_)); //;variance-effective size of mu
    recombinationWeights_ = arma::normalise(recombinationWeights_, 1); //;normalize recombination weights array
    ccum_ = 4.0 / (numberOfDimensions + 4.0);
    cs_ = (mueff_ + 2) / (numberOfDimensions + mueff_ + 3);

    ccov1_ = 2.0 / (std::pow(numberOfDimensions + 1.3, 2) + mueff_);
    ccovmu_ = std::min(1.0 - ccov1_, 2 * (mueff_ - 2 + 1.0 / mueff_) / (std::pow(numberOfDimensions + 2, 2) + mueff_));

    damping_ = 1 + 2 * std::max(0.0, std::sqrt((mueff_ - 1) / (numberOfDimensions + 1)) - 1) + cs_;
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getPopulationSize() {
    return populationSize_;
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
}
