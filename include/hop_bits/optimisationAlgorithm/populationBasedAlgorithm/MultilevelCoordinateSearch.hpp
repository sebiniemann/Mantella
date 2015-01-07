#pragma once

#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {

  class MultilevelCoordinateSearch : public PopulationBasedAlgorithm<double> {
  public:
    //lower boundaries are expected in the first col of "boundaries", upper boundaries in the second col of "boundaries.
    //initialPointIndex is the index inside initialPopulation_ which is used as the starting point.
    explicit MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem, const unsigned int& populationSize, arma::Mat<double> boundaries, unsigned int boxDivisions, arma::Mat<double> hess, arma::Col<arma::uword> initialPointIndex, unsigned int maxLocalSearchSteps = 50, double localStopThreshold = arma::datum::eps);
    MultilevelCoordinateSearch(const MultilevelCoordinateSearch&) = delete;
    MultilevelCoordinateSearch& operator=(const MultilevelCoordinateSearch&) = delete;

    std::string to_string() const noexcept override;
  protected:
    void optimiseImplementation() override;

    unsigned int step1_ = 10000;
    unsigned int step = 1000; //TODO: could be moved to constructor as a definable variable
    unsigned int dim = 10000;

    unsigned int boxDivisions_; //smax
    arma::Mat<double> boundaries_; //u,v - with u=col(0) and v=col(1)
    unsigned int maxLocalSearchSteps_; //local
    double localStopThreshold_; //gamma
    arma::Mat<double> hess_;

    arma::Col<arma::uword> isplit_;
    arma::Col<arma::uword> level_;
    arma::Col<arma::uword> ipar_;
    arma::Col<arma::uword> ichild_;
    arma::Mat<double> boxBaseVertexFunctionValues_; //f
    arma::Mat<double> z_;
    arma::Col<arma::uword> nogain_;

    arma::Col<arma::uword> initialPointIndex_; //l
    arma::Mat<double> x0_;
    arma::Mat<double> initListEvaluations_; //f0
    arma::Col<arma::uword> bestPointIndex_; //istar

    arma::Col<double> baseVertex_; //x
    arma::Col<double> oppositeVertex_; //v1, sometimes called y in subfunctions

    arma::Col<arma::uword> record_;
    unsigned int nboxes_ = 1;
    unsigned int nbasket_ = 0;
    unsigned int nbasket0_ = 0;
    unsigned int m_;
    unsigned int nloc_ = 0;
    arma::Mat<double> xloc_;

    unsigned int nglob_ = 0;
    arma::Mat<double> xglob_;

    void initBoxes(); //initbox.m
    arma::Col<arma::uword> variabilityRanking_; //p

    //helper methods
    void genBox(int nbox, int par, int level, int nchild, double baseVertexFunctionValue); //genbox.m

    arma::Col<double> quadraticPolynomialInterpolation(arma::Col<double> supportPoints, arma::Col<double> functionValues); //polint.m

    double minimumQuadraticPolynomial(double a, double b, arma::Col<double> d, arma::Mat<double> x0_fragment); //quadmin.m

    double quadraticPolynomial(double x, arma::Col<double> d, arma::Mat<double> x0_fragment); //quadpol.m

    double splitByGoldenSectionRule(double x1, double x2, double f1, double f2); //split1.m

    void splitByRank(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0); //splrnk.m

    //split2.m 
    //determines a value x1 for splitting the interval [min(x,y),max(x,y)] 
    //is modeled on the function subint with safeguards for infinite y
    double splitBySubint(double x, double y);

    //splinit.m 
    //splits box # par at level s according to the initialization list in the 
    //ith coordinate and inserts its children and their parameters in the list.
    //returns isFinished() || isTerminated()
    bool splitByInitList(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Mat<double> pointsInBasket, arma::Col<double> pointsInBasketValue);

    //split.m 
    //splits box # par at level s in its ith coordinate into two or three 
    //children and inserts its children and their parameters in the list
    //returns isFinished() || isTerminated()
    bool split(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Mat<double> pointsInBasket, arma::Col<double> pointsInBasketValue);
    
    //exgain.m 
    //determines the splitting index, the splitting value and the expected 
    //gain vector e for (potentially) splitting a box by expected gain
    arma::Col<double> expectedGainOfSplit(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2);

    unsigned int startSweep(); //strtsw.m

    void vert1(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2); //vert1.m

    void vert2(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2); //vert2.m

    void vert3(int updateIndex, unsigned int j, unsigned int m, unsigned int f0Index, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2); //vert3.m

    void updtf(unsigned int numberOfDimensions, unsigned int splittingIndex, double fold, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2, double baseVertexValueCurrentBox); //updtf.m
    
    //subint.m 
    //computes for real x and real or infinite y two points x1 and x2 in 
    //[min(x,y),max(x,y)] that are neither too close nor too far away from x
    arma::Col<double> subint(double x, double y);
    
    //updtrec.m 
    //updates the pointer record(s) to the best non-split box at level s.
    //f=vector containing the base vertex function values of the already defined boxes
    void updateRecord(unsigned int label, int level, arma::Col<double> f);
    
    //chkloc.m 
    //checks whether a point has already been used as starting point for a local search
    bool checkLocationNotUsed();
    
    //addloc.m 
    //adds a new point to the list of starting points for local search
    void addLocation(arma::Col<double> loc);
  };
}

