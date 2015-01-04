#pragma once

#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {

  class MultilevelCoordinateSearch : public PopulationBasedAlgorithm<double> {
  public:
    //lower boundaries are expected in the first col of "boundaries", upper boundaries in the second col of "boundaries.
    //initialPointIndex is the index inside initialPopulation_ which is used as the starting point.
    explicit MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem, const unsigned int& populationSize, arma::Mat<double> boundaries, unsigned int boxDivisions,arma::Mat<double> hess, arma::Col<arma::uword> initialPointIndex, unsigned int maxLocalSearchSteps = 50, double localStopThreshold = arma::datum::eps);
    MultilevelCoordinateSearch(const MultilevelCoordinateSearch&) = delete;
    MultilevelCoordinateSearch& operator=(const MultilevelCoordinateSearch&) = delete;

    std::string to_string() const noexcept override;
  protected:
    void optimiseImplementation() override;
    
    unsigned int step1_ = 10000;
    
    unsigned int boxDivisions_; //smax
    arma::Mat<double> boundaries_; //u,v - with u=col(0) and v=col(1)
    unsigned int maxLocalSearchSteps_; //local
    double localStopThreshold_; //gamma
    arma::Mat<double> hess_;
    
    arma::Col<double> isplit_;
    arma::Col<arma::uword> level_;
    arma::Col<double> ipar_;
    arma::Col<double> ichild_;
    arma::Mat<double> f_;
    arma::Mat<double> z_;
    arma::Col<double> nogain_;
    
    arma::Col<arma::uword> initialPointIndex_; //l
    arma::Mat<double> x0_;
    arma::Mat<double> initListEvaluations_; //f0
    arma::Col<arma::uword> bestPointIndex_; //istar
    
    arma::Col<double> baseVertex_; //x
    arma::Col<double> oppositeVertex_; //v1
    
    arma::Col<double> record_;
    unsigned int nboxes_ = 1;
    unsigned int nbasket_ = 0;
    unsigned int nbasket0_ = 0;
    unsigned int nsweep_ = 0;
    unsigned int m_;
    unsigned int nloc_ = 0;
    arma::Mat<double> xloc_;
    
    unsigned int nglob_ = 0;
    arma::Mat<double> xglob_;
    
    void initBoxes(); //initbox.m
    arma::Mat<double> boxBaseVertexFunctionValues_; //f
    arma::Col<arma::uword> variabilityRanking_; //p
    
    //helper methods
    void genBox(int nbox, int par, arma::uword level, int nchild, double baseVertexFunctionValue); //genbox.m
    
    arma::Col<double> quadraticPolynomialInterpolation(arma::Col<double> supportPoints, arma::Col<double> functionValues); //polint.m
    double minimumQuadraticPolynomial(double a, double b, arma::Col<double> d, arma::Mat<double> x0_fragment); //quadmin.m
    double quadraticPolynomial(double x, arma::Col<double> d, arma::Mat<double> x0_fragment); //quadpol.m
    double splitGoldenSectionRule(double x1, double x2, double f1, double f2); //split1.m
  };
}

