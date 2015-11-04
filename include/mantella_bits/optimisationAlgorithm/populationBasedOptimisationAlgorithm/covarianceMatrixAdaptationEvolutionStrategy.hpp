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
    arma::Col<double> getRecombinationWeights();
    void setRecombinationWeights(arma::Col<double> weights);
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

    arma::Col<double> startingPoint_; //definput.xstart
    arma::Col<double> stepSize_; //definput.insigma

    double toleranceX_; //stopTolX or defopts.TolX; stop if x-change smaller TolX
    double toleranceUpX_; //stopTolUpX or defopts.TolUpX; stop if x-changes larger TolUpX
    double toleranceFun_; //stopTolFun or defopts.TolFun; stop if fun-changes smaller TolFun
    double toleranceHistFun_; //stopTolHistFun or defopts.TolHistFun; stop if back fun-changes smaller TolHistFun
    bool stopOnStagnation_; //defopts.StopOnStagnation; stop when fitness stagnates for a long time
    bool stopOnWarnings_; //defopts.StopOnWarnings
    bool stopOnEqualFunctionValues_; //defops.stopOnEqualFunctionValues - originally 2 + N/3  % number of iterations
    arma::Col<double> EqualFunctionValues_;

    arma::uword mu_; //defopts.ParentNumber/mu
    double mueff_; //mueff
    arma::Col<double> recombinationWeights_; //weights
    double cs_; //defopts.CMA.cs/cs; cumulation constant for step-size
    double damping_; //defopts.CMA.damps; damping for step-size
    double ccum_; //defopts.CMA.ccum/cc; 
    double ccov1_; //defopts.CMA.ccov1;
    double ccovmu_; //defopts.CMA.ccovmu;
    arma::uword activeCMA_; ////defopts.CMA.active; 0: active CMA 1: neg. updates with pos. def. check, 2: neg. updates
    arma::uword irun_; //irun

    //arxvalid needs to be here so it is available after the loop
    arma::Mat<double> newGenerationValid_; //arxvalid
    arma::Col<double> xmean_; //xmean
    arma::Col<double> xold_; //xold
    arma::uword lambda_last_;
    double sigma_; //sigma
    arma::Col<double> pc_; //pc; evolution path for C
    arma::Col<double> ps_; //ps; evolution path for sigma
    arma::Mat<double> B_; //B
    arma::Mat<double> C_; //C
    arma::Mat<double> D_; //D
    arma::Mat<double> BD_; //BD
    arma::Col<double> diagC_; //diagC
    arma::Col<double> diagD_; //diagD; diagonal matrix D defines the scaling
    double chiN_; //chiN
    double negCcov_; //neg.ccov

    std::tuple<arma::Mat<double>, arma::Mat<double>> capToBoundary(arma::Mat<double> x); //xintobounds.m - returns capped matrix/vector first, indexes of capped values second

    arma::Col<double> fitnessRaw_; //fitness.raw
    arma::Col<double> fitnessSel_; //fitness.sel
    arma::Col<double> fitnessRawPreviousIteration_;//helper variable to replace fitnesshist
    arma::Col<arma::uword> fitnessIdx_; //fitness.idx
    arma::Col<arma::uword> fitnessIdxSel_; //fitness.idxsel

    //HCMA needs to be able to do single iterations of CMAES, for that this bool can be set.
    bool singleIteration_ = false;
    //helper variable to check if run was initialized
    bool runInitialized_ = false;
    arma::uword countiter_; //countiter - counts main loop evaluations, NOT function evaluations
    arma::uword stopMaxIter_; //stopMaxIter or opts.MaxIter

    void optimiseImplementation() override;
  };
}
