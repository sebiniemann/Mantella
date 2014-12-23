#pragma once

#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {

  class MultilevelCoordinateSearch : public PopulationBasedAlgorithm<double> {
  public:
    //lower boundaries are expected in the first col of "boundaries", upper boundaries in the second col of "boundaries.
    //initialPointIndex is the index inside initialPopulation_ which is used as the starting point.
    explicit MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem, const unsigned int& populationSize, arma::Mat<double> boundaries, unsigned int boxDivisions = 50*size*std::pow(boundaries.col(0),2), unsigned int maxLocalSearchSteps = 50, double localStopThreshold = arma::datum::eps, arma::Mat<double> hess = arma::ones(size(boundaries.col(0))), unsigned int initialPointIndex);
    MultilevelCoordinateSearch(const MultilevelCoordinateSearch&) = delete;
    MultilevelCoordinateSearch& operator=(const MultilevelCoordinateSearch&) = delete;

    std::string to_string() const noexcept override;
  protected:
    void optimiseImplementation() override;
    
    unsigned int step1_ = 10000;
    
    unsigned int boxDivisions_;
    arma::Mat<double> boundaries_;
    unsigned int maxLocalSearchSteps_;
    double localStopThreshold_;
    arma::Mat<double> hess_;
    
    arma::Col<double> isplit_;
    arma::Col<double> level_;
    arma::Col<double> ipar_;
    arma::Col<double> ichild_;
    arma::Mat<double> f_;
    arma::Mat<double> z_;
    arma::Col<double> nogain_;
    
    unsigned int initialPointIndex_;
    arma::Mat<double> x0_;
    arma::Mat<double> f0_;
    arma::Col<arma::uword> istar_;
    
  };
}

