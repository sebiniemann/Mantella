#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp>

namespace mant {

  class CovarianceMatrixAdaptationEvolutionStrategy : public PopulationBasedOptimisationAlgorithm {
  public:
    explicit CovarianceMatrixAdaptationEvolutionStrategy(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::uword populationSize);

    explicit CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem);

    //used (i.e. by HCMA) to compute certain values after changing them after instantiation
    void initializeRun();

    arma::uword getIRun();
    void setIRun(const arma::uword irun); //irun
    void setLambda0(const double lambda0); //lambda0
    void setStepSize(const arma::Col<double> sigma); //insigma
    void setStartingPoint(const arma::Col<double> xStart); //xstart
    void setPopulationSize(const arma::uword popSize);
    double getIncPopSize() const; //IncPopSize

    double getCcov1() const;
    void setCcov1(double ccov1);
    double getCcovmu() const;
    void setCcovmu(double ccovmu);
    double getCcum() const;
    void setCcum(double ccum);
    double getCs() const;
    void setCs(double cs);
    arma::uword getMu() const;
    void setMu(arma::uword numberOfParents);
    double getToleranceFun() const;
    void setToleranceFun(double toleranceFun);
    double getToleranceHistFun() const;
    void setToleranceHistFun(double toleranceHistFun);
    double getToleranceUpX() const;
    void setToleranceUpX(double toleranceUpX);
    double getToleranceX() const;
    void setToleranceX(double toleranceX);
    arma::Col<double> getXmean() const;
    void setXmean(arma::Col<double> xmean);

    void setSingleIteration(const bool DoSingleIteration);

    std::string toString() const noexcept override;


  protected:
    //Notation: comments always start with matlab name of variable
    //          comments after ';' are from matlab code
    //          comments after '-' are from mantella

    arma::Col<double> startingPoint; //definput.xstart
    arma::Col<double> stepSize; //definput.insigma

    double toleranceX; //stopTolX or defopts.TolX; stop if x-change smaller TolX
    double toleranceUpX; //stopTolUpX or defopts.TolUpX; stop if x-changes larger TolUpX
    double toleranceFun; //stopTolFun or defopts.TolFun; stop if fun-changes smaller TolFun
    double toleranceHistFun; //stopTolHistFun or defopts.TolHistFun; stop if back fun-changes smaller TolHistFun
    bool stopOnStagnation = true; //defopts.StopOnStagnation; stop when fitness stagnates for a long time
    bool stopOnWarnings = true; //defopts.StopOnWarnings
    bool stopOnEqualFunctionValues = true; //defops.stopOnEqualFunctionValues - originally 2 + N/3  % number of iterations
    arma::Col<double> EqualFunctionValues;
    //seems strange since it's never used in an integer check, just a bool check

    bool evalParallel = true; //defopts.EvalParallel; objective function FUN accepts NxM matrix, with M>1?
    arma::uword restarts = 9; //defopts.Restarts - HCMA default is 9
    double incPopSize; //defopts.IncPopSize; multiplier for population size before each restart
    arma::uword mu; //defopts.ParentNumber/mu
    double mueff; //mueff
    arma::uword recombinationWeightsType = 2; //defotps.RecombinationWeights - 0 = equal, 1 = linear, 2 = superlinear decrease
    arma::Col<double> recombinationWeights; //weights
    double cs; //defopts.CMA.cs/cs; cumulation constant for step-size
    double damping; //defopts.CMA.damps; damping for step-size
    double ccum; //defopts.CMA.ccum/cc; 
    double ccov1; //defopts.CMA.ccov1;
    double ccovmu; //defopts.CMA.ccovmu;
    arma::uword activeCMA; ////defopts.CMA.active; active CMA 1: neg. updates with pos. def. check, 2: neg. updates
    arma::uword irun = 0; //irun
    //opts.EvalParallel - left out. not sure what this is supposed to do??
    //flgDiagonalOnly - left out. Afaik we never do diagonal matrices. If needed all code for this can easily be added.

    //arxvalid needs to be here so it is available after the loop
    arma::Mat<double> newGenerationValid; //arxvalid
    arma::Col<double> xmean; //xmean
    arma::Col<double> xold; //xold
    double lambda0; //lambda0
    double sigma; //sigma
    arma::Col<double> pc; //pc; evolution path for C
    arma::Col<double> ps; //ps; evolution path for sigma
    arma::Mat<double> B; //B
    arma::Mat<double> C; //C
    arma::Mat<double> D; //D
    arma::Mat<double> BD; //BD
    arma::Col<double> diagC; //diagC
    arma::Col<double> diagD; //diagD; diagonal matrix D defines the scaling
    double chiN; //chiN
    double negCcov; //neg.ccov

    bool boundaryActive = false; //bnd.isactive
    arma::Col<double> boundaryWeights; //bnd.weights
    arma::Col<double> boundaryScale; //bnd.scale
    arma::Col<arma::uword> boundaryExists; //bnd.isbounded
    arma::Col<double> boundaryDeltaFitHistory; //bnd.dfithist; delta fit for setting weights
    bool boundaryFitnessValid = false;; //bnd.validfitval; 
    bool boundaryInitialPhase = true; //bnd.iniphase
    arma::Col<double> boundaryPenalty; //bnd.arpenalty
    //bnd.flgscale not used, always 0 in matlab
    std::tuple<arma::Mat<double>, arma::Mat<double>> capToBoundary(arma::Mat<double> x); //xintobounds.m - returns capped matrix/vector first, indexes of capped values second

    arma::Col<double> fitnessRaw; //fitness.raw
    arma::Col<double> fitnessSel; //fitness.sel
    arma::Col<arma::uword> fitnessIdx; //fitness.idx
    arma::Col<arma::uword> fitnessIdxSel; //fitness.idxsel
    arma::Col<double> fitnessHist; //fitness.hist
    arma::Col<double> fitnessHistSel; //fitness.histsel
    arma::Col<double> fitnessHistBest; //fitness.histbest
    arma::Col<double> fitnessHistMedian; //fitness.histmedian
    
    arma::Col<double> percentiles(arma::Col<double> vector, arma::Col<arma::uword> perc); //myprctile

    //HCMA needs to be able to do single iterations of CMAES, for that this bool can be set.
    bool singleIteration = false;
    //helper variable to check if run was initialized
    bool runInitialized = false;
    arma::uword countiter = 0; //countiter - counts main loop evaluations, NOT function evaluations
    arma::uword stopMaxIter; //stopMaxIter or opts.MaxIter

    void optimiseImplementation() override;
  };
}