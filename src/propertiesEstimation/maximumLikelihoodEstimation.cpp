#include <hop_bits/propertiesEstimation/maximumLikelihoodEstimation.hpp>

#include <iostream>

namespace hop {
  void MaximumLikelihoodEstimation::estimateImplementation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept {


//    std::cout << "correlationAnalysis_" << std::endl;
    arma::wall_clock timer;
    timer.tic();
    correlationAnalysis_->analyse(optimisationProblem);
     double n_secs_correlationAnalysis_ = timer.toc();
//    std::cout << "took " << n_secs << " seconds" << std::endl;
    correlationCoefficient_.push_back(correlationAnalysis_->getCorrelationCoefficient());

//    std::cout << "correlationAnalysis_" << std::endl;
    timer.tic();
    linearModelAnalysis_->analyse(optimisationProblem);
    double n_secs_linearModelAnalysis_ = timer.toc();
//    std::cout << "took " << n_secs << " seconds" << std::endl;
    linearModelEstimators_.push_back(linearModelAnalysis_->getLinearModelEstimator());
    isLinear_.push_back(false);


//    std::cout << "correlationAnalysis_" << std::endl;
    timer.tic();
    quadraticModelAnalysis_->analyse(optimisationProblem);
    double n_secs_quadraticModelAnalysis_ = timer.toc();
//    std::cout << "took " << n_secs << " seconds" << std::endl;
    quadraticModelEstimators_.push_back(quadraticModelAnalysis_->getQuadraticModelEstimator());
    isQuadratic_.push_back(false);


//    std::cout << "correlationAnalysis_" << std::endl;
    timer.tic();
    lipschitzContinuityAnalysis_->analyse(optimisationProblem);
    double n_secs_lipschitzContinuityAnalysis_ = timer.toc();
//    std::cout << "took " << n_secs << " seconds" << std::endl;
    lipschitzConstants_.push_back(lipschitzContinuityAnalysis_->getLipschitzConstant());


//    std::cout << "correlationAnalysis_" << std::endl;
    timer.tic();
    additiveSeparabilityAnalysis_->analyse(optimisationProblem);
    double n_secs_additiveSeparabilityAnalysis_ = timer.toc();
//    std::cout << "took " << n_secs << " seconds" << std::endl;

       std::cout << n_secs_correlationAnalysis_ << " \t" << n_secs_linearModelAnalysis_ << " \t" << n_secs_quadraticModelAnalysis_ << " \t" << n_secs_lipschitzContinuityAnalysis_ << " \t" << n_secs_additiveSeparabilityAnalysis_ << std::endl;


    numberOfPropertySets_ = 1;
  }
}
