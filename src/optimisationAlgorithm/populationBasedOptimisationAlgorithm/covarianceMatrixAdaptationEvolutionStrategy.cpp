#include <mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp>

// C++ standard library
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::uword populationSize)
  : PopulationBasedOptimisationAlgorithm(optimisationProblem, populationSize) {
    //HCMA settings coming from xacmes.m - 
    setStartingPoint(getRandomParameter());
    setStepSize((getUpperBounds() - getLowerBounds()) / 5.0);
    setToleranceFun(1e-12);
    setToleranceHistFun(1e-12);
    setToleranceX(2e-12);
    setToleranceUpX(arma::datum::inf);

    xmean = startingPoint;
    xold = startingPoint;
  }

  CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
  : CovarianceMatrixAdaptationEvolutionStrategy(optimisationProblem,
  4 + std::floor(3 * log(optimisationProblem->numberOfDimensions_))) {
    
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::initializeRun() {
    //TODO: figure out what to do with lambda0 and popsize settings
    //this overrides constructor settings on restart runs
    //it gets split into lambda and popsize, but neither variable is written to ever again in CMAES
    //xacmes also writes these variables again...
    lambda0 = std::floor(populationSize_ * std::pow(incPopSize, irun - 1));
    setPopulationSize(lambda0);

    sigma = arma::max(stepSize);
    pc = arma::zeros(numberOfDimensions_);
    ps = arma::zeros(numberOfDimensions_);

    diagD = stepSize / sigma;
    diagC = arma::pow(diagD, 2);
    B = arma::eye(numberOfDimensions_, numberOfDimensions_); //;B defines the coordinate system
    BD = arma::diagmat(diagD); //;B*D for speed up only
    C = arma::diagmat(diagC); //;covariance matrix == BD*(BD)'

    //TODO: cmaes_initializeRun initialized fitness history here, but we have that already in mantella
    fitnessHist = arma::datum::nan * arma::ones(10 + std::ceil(30.0 * numberOfDimensions_ / populationSize_));
    fitnessHistSel = arma::datum::nan * arma::ones(10 + std::ceil(30.0 * numberOfDimensions_ / populationSize_));

    /////BOUNDARY
    boundaryActive = arma::any(getLowerBounds() > -arma::datum::inf) || arma::any(getUpperBounds() > -arma::datum::inf);
    if (boundaryActive) {
      //TODO: sanity check for initial point?
      boundaryWeights = arma::zeros(numberOfDimensions_);

      boundaryScale = arma::ones(numberOfDimensions_);

      //mark all dimensions which have a boundary
      boundaryExists = arma::Col<arma::uword>(numberOfDimensions_);
      for (arma::uword i = 0; i < getLowerBounds().n_elem; i++) {
        if (getLowerBounds()(i) != arma::datum::inf || getUpperBounds()(i) != arma::datum::inf) {
          boundaryExists(i) = true;
        } else {
          boundaryExists(i) = false;
        }
      }
      //TODO: cmaes modifies sigma(stepsize) here if it is too small or too large, do we want that?
      boundaryDeltaFitHistory = arma::ones(1); //gets elongated later
      boundaryInitialPhase = true;
    }

    //TODO: cmaes evaluates starting point once here for output and caches that in evaluation history,
    //seems unnecessary (?)

    chiN = std::pow(numberOfDimensions_, 0.5) * (1 - 1.0 / (4 * numberOfDimensions_) + 1.0 / (21 * std::pow(numberOfDimensions_, 2)));
    //;expectation of||N(0,I)|| == norm(randn(N,1))

    //miscellaneous inits needed
    runInitialized = true;
    EqualFunctionValues = arma::zeros(10+numberOfDimensions_);
    stopMaxIter = 100 + 50*std::pow(numberOfDimensions_+3,2) / std::sqrt(populationSize_);
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    //do some necessary initialization stuff
    if (!runInitialized) {
      initializeRun();
    }

    //TODO: this stopflag is more sophisticated in the matlab code.
    bool stopFlag = false;
    arma::uword lambda_last = populationSize_;
    while (!stopFlag) {
      //TODO: lambda_last??? see studip
      //;set internal parameters
      if (countiter == 0 || populationSize_ != lambda_last) {
        lambda_last = populationSize_;
        mu = std::floor(populationSize_ / 2.0);
        recombinationWeights = arma::zeros(mu);
        if (recombinationWeightsType == 0) {//equal
          recombinationWeights = arma::ones(mu);
        } else if (recombinationWeightsType == 1) {//linear
          recombinationWeights = mu + 0.5 - arma::linspace(1, mu, mu).t();
        } else if (recombinationWeightsType == 2) {//superlinear
          recombinationWeights = std::log(mu + 0.5) - arma::log(arma::linspace(1, mu, mu)).t();
          //;muXone array for weighted recombination
          //;qqq mu can be non-integer and
          //;should become ceil(mu-0.5) (minor correction)
        } else {
          //TODO: CMAES throws error here cause type not implemented
        }

        mueff = std::pow(arma::accu(recombinationWeights), 2) / arma::accu(arma::pow(recombinationWeights, 2)); //;variance-effective size of mu
        recombinationWeights = recombinationWeights / arma::accu(recombinationWeights); //;normalize recombination weights array
        //error check omitted, shouldn't happen

        //TODO: these values are from HCMA, standard CMAES are different. not sure how to impl
        ccum = std::pow((numberOfDimensions_ + 2 * mueff / numberOfDimensions_) / (4 + mueff / numberOfDimensions_), -1); //;time constant for cumulation for covariance matrix
        cs = (mueff + 2) / (numberOfDimensions_ + mueff + 3);

        ccov1 = std::min(2.0, populationSize_ / 3.0) / (std::pow(numberOfDimensions_ + 1.3, 2) + mueff);
        ccovmu = std::min(2.0, populationSize_ / 3.0) / (mueff - 2 + 1.0 / mueff) / (std::pow(numberOfDimensions_ + 2, 2) + mueff);

        damping = 1 + 2 * std::max(0.0, std::sqrt((mueff - 1) / (numberOfDimensions_ + 1)) - 1) + cs;
      }

      countiter++;

      //;Generate and evaluate lambda offspring
      fitnessRaw = arma::ones(populationSize_);
      fitnessRaw.fill(arma::datum::nan);

      //;parallel evaluation - the if is omitted.

      arma::Mat<double> newGenerationRaw = getRandomPopulation(); //arz
      arma::Mat<double> newGeneration = arma::repmat(xmean, 1, populationSize_) + sigma * (BD * newGenerationRaw); //arx

      newGenerationValid = newGeneration; //arxvalid
      if (boundaryActive) {
        newGenerationValid = std::get<0>(capToBoundary(newGeneration));
      }

      //TODO: cmaes passes the whole matrix into the fitnessfunction, not sure if this is the right way around it
      for (arma::uword i = 0; i < newGenerationValid.n_cols; i++) {
        fitnessRaw(i) = getObjectiveValue(newGenerationValid.col(i));
        numberOfIterations_++;
      }

      //TODO: There is supposed to be a line of code here, but it's nonsensical due to no noise in mantella
      //TODO: Omitted a large code block here. Very sure it's unreachable in our setting
      //(with "parallel" evaluation always on and noise off)

      fitnessSel = fitnessRaw;

      //;----- handle boundaries -----
      //yeap. 1 < 3 is what is actually written in matlab
      if (1 < 3 && boundaryActive) {
        //;Get delta fitness values
        //TODO: ugly, how to initialiser list?
        arma::Col<arma::uword> percs = arma::Col<arma::uword>(2);
        percs << 25 << 75;
        arma::Col<double> vals = percentiles(fitnessRaw, percs);
        vals = (vals(1) - vals(0)) / numberOfDimensions_ / arma::mean(diagC) / std::pow(sigma, 2);
        //;Catch non-sensible values 
        if (!arma::is_finite(vals)) {
          //TODO: console warning here: Non-finite fitness range
          vals = arma::max(boundaryDeltaFitHistory);
        } else if (arma::all(vals == 0)) { //;happens if all points are out of bounds
          vals = arma::min(boundaryDeltaFitHistory(arma::find(boundaryDeltaFitHistory > 0))); //;seems not to make sense, given all solutions are out of bounds
        } else if (boundaryFitnessValid == false) { //;flag that first sensible val was found
          //TODO: boundaryDeltaFitHistory gets set to [] which we can't do. what to do instead?
          boundaryFitnessValid = true;
        }

        //;Store delta fitness values
        if (boundaryDeltaFitHistory.n_elem < 20 + (3 * numberOfDimensions_) / populationSize_) {
          boundaryDeltaFitHistory.insert_rows(boundaryDeltaFitHistory.n_rows, vals);
        } else {
          boundaryDeltaFitHistory.shed_row(0);
          boundaryDeltaFitHistory.insert_rows(boundaryDeltaFitHistory.n_rows, vals);
        }

        std::tuple<arma::Mat<double>, arma::Mat<double>> XmeanTemp = capToBoundary(xmean);
        arma::Mat<double> xmeanCapped = std::get<0>(XmeanTemp);
        arma::Mat<double> xmeanCappedIndexes = std::get<1>(XmeanTemp);

        //;Set initial weights
        if (boundaryInitialPhase) {
          if (arma::any(arma::any(xmeanCappedIndexes))) {
            //TODO: how to do 
            boundaryWeights.elem(arma::find(boundaryExists)).fill(2.0002 * arma::median(boundaryDeltaFitHistory)); //unexplained magic number
            //;scale only initial weights
            arma::Col<double> dd = diagC;
            arma::Col<arma::uword> idx = arma::find(boundaryExists);
            dd = dd(idx) / arma::mean(dd); //;remove mean scaling
            boundaryWeights(idx) = boundaryWeights(idx) / dd;
            if (boundaryFitnessValid && countiter > 2) {
              boundaryInitialPhase = false;
            }
          }
        }

        //;Increase weights
        if (1 < 3 && arma::any(arma::any(xmeanCappedIndexes))) {//;any coordinate of xmean out of bounds
          //;judge distance of xmean to boundary
          xmeanCapped = xmean - xmeanCapped;
          //replaced & with == since it's comparing between 0/1
          arma::Mat<arma::uword> idx = ((xmeanCappedIndexes != 0) == (arma::Mat<arma::uword>)(arma::abs(xmeanCapped) > 3 * std::max(1.0, std::sqrt(numberOfDimensions_) / mueff)
              * sigma * arma::sqrt(diagC)));
          //;only increase if xmean is moving away
          //TODO: xold is actually unassigned at this point, similar to the lambda problem we encountered earlier
          idx = idx == (arma::sign(xmeanCapped) == arma::sign(xmean - xold));
          if (!idx.is_empty()) { //;increase
            boundaryWeights(idx) = std::pow(1.2, std::min(1.0, mueff / 10 / numberOfDimensions_)) * boundaryWeights(idx);
          }
        }

        //;Assigned penalized fitness
        boundaryPenalty = (boundaryWeights / boundaryScale).t() * arma::pow(newGenerationValid - newGeneration, 2);
        fitnessSel = fitnessRaw + boundaryPenalty;
      }
      //;----- end handle boundaries -----

      //;Sort by fitness
      fitnessIdx = arma::sort_index(fitnessRaw);
      fitnessRaw = arma::sort(fitnessRaw);
      fitnessIdxSel = arma::sort_index(fitnessSel);
      fitnessSel = arma::sort(fitnessSel); //;minimization
      fitnessHist.rows(1, fitnessHist.n_elem - 1) = fitnessHist.rows(0, fitnessHist.n_elem - 2); //;record short history of
      fitnessHist(0) = fitnessRaw(0); //;best fitness values
      if (fitnessHistBest.n_elem < 120 + std::ceil(30.0 * numberOfDimensions_ / populationSize_) ||
          ((countiter % 5) == 0 && fitnessHistBest.n_elem < 2e4)) { //;20 percent of 1e5 gen.
        fitnessHistBest.insert_rows(0, fitnessRaw(0)); //;best fitness values
        fitnessHistMedian.insert_rows(0, arma::median(fitnessRaw)); //;median fitness values
      } else {
        fitnessHistBest.rows(1, fitnessHistBest.n_elem - 1) = fitnessHistBest.rows(0, fitnessHistBest.n_elem - 2);
        fitnessHistMedian.rows(1, fitnessHistMedian.n_elem - 1) = fitnessHistMedian.rows(0, fitnessHistMedian.n_elem - 2);
        fitnessHistBest(0) = fitnessRaw(0); //;best fitness values
        fitnessHistMedian(0) = arma::median(fitnessRaw); //;median fitness values
      }
      fitnessHistSel.rows(1, fitnessHistSel.n_elem - 1) = fitnessHistSel.rows(0, fitnessHistSel.n_elem - 2); //;record short history of
      fitnessHistSel(0) = fitnessSel(0); //;best sel fitness values

      //;Calculate new xmean, this is selection and recombination 
      xold = xmean; //;for speed up of Eq. (2) and (3)
      xmean = newGeneration.cols(fitnessIdxSel.rows(0, mu - 1)) * recombinationWeights;
      arma::Mat<double> zmean = newGenerationRaw.cols(fitnessIdxSel.rows(0, mu - 1)) * recombinationWeights; //;==D^-1*B'*(xmean-xold)/sigma

      //;Cumulation: update evolution paths
      ps = (1 - cs) * ps + std::sqrt(cs * (2 - cs) * mueff) * (B * zmean); //;Eq. (4)
      double hsig = arma::norm(ps) / std::sqrt(1 - std::pow(1 - cs, 2 * countiter)) / chiN < 1.4 + 2.0 / (numberOfDimensions_ + 1);

      pc = (1 - ccum) * pc + hsig * (std::sqrt(ccum * (2 - ccum) * mueff) / sigma) * (xmean - xold); //;Eq. (2)

      //;Adapt covariance matrix
      negCcov = 0;
      if ((ccov1 + ccovmu) > 0) {
        arma::Mat<double> arpos = newGeneration.cols(fitnessIdxSel.rows(0, mu - 1)) - arma::repmat(xold, 0, mu - 1) / sigma;
        //;"active" CMA update: negative update, in case controlling pos. definiteness 
        if (activeCMA > 0) {
          //;set parameters
          double negMu = mu;
          double negMueff = mueff;
          if (activeCMA > 10) {//;flat weights with mu=lambda/2
            negMu = std::floor(populationSize_ / 2);
            negMueff = negMu;
          }
          /*;
          % ;neg.mu = ceil(min([N, lambda/4, mueff]));  neg.mueff = mu; % i.e. neg.mu <= N 
          % ;Parameter study: in 3-D lambda=50,100, 10-D lambda=200,400, 30-D lambda=1000,2000 a 
          % ;three times larger neg.ccov does not work. 
          % ;  increasing all ccov rates three times does work (probably because of the factor (1-ccovmu))
          % ;  in 30-D to looks fine
           */

          negCcov = (1 - ccovmu) * 0.25 * negMueff / (std::pow(numberOfDimensions_ + 2, 1.5) + 2 * negMueff);
          double negMinResidualVariance = 0.66; //;keep at least 0.66 in all directions, small popsize are most critical
          double negAlphaOld = 0.5; //;where to make up for the variance loss, 0.5 means no idea what to do
          //;1 is slightly more robust and gives a better "guaranty" for pos. def., 
          //;but does it make sense from the learning perspective for large ccovmu?

          double negCcovFinal = negCcov;

          //;prepare vectors, compute negative updating matrix Cneg and checking matrix Ccheck
          //have to do some workaround since you cannot create negative descending sequences with arma
          arma::Col<arma::uword> negHelper = arma::conv_to<arma::uvec>::from(
              arma::flipud(arma::linspace(populationSize_ - negMu, populationSize_ - 1, std::abs(-negMu + 2))));
          arma::Mat<double> newGenerationRawNeg = newGenerationRaw.cols(fitnessIdxSel.rows(negHelper)); //arzneg
          //;i-th longest becomes i-th shortest

          arma::Col<arma::uword> ngRawNegNormIndex = arma::sort_index(arma::sqrt(arma::sum(arma::pow(newGenerationRawNeg, 2), 1))); //idxnorms
          arma::Col<double> ngRawNegNorm = arma::sort(arma::sqrt(arma::sum(arma::pow(newGenerationRawNeg, 2), 1))); //arnorms
          ngRawNegNormIndex = arma::sort_index(ngRawNegNormIndex); //;inverse index
          //another workaround for negative ordering
          negHelper = arma::conv_to<arma::uvec>::from(
              arma::flipud(arma::linspace(0, ngRawNegNorm.n_elem - 1, ngRawNegNorm.n_elem)));
          arma::Col<double> ngRawNegNormFacs = ngRawNegNorm(negHelper) / ngRawNegNorm; //arnormfacs
          ngRawNegNorm = ngRawNegNorm(negHelper); //;for the record
          if (activeCMA < 20) {
            newGenerationRawNeg = newGenerationRawNeg % arma::repmat(ngRawNegNormFacs(ngRawNegNormIndex), numberOfDimensions_, 1); //;E x*x' is N
          }
          arma::Mat<double> Ccheck; //intentional spelling
          arma::Mat<double> Cneg;
          if (activeCMA < 10 && negMu == mu) { //;weighted sum
            if (activeCMA % 10 == 1) {
              Ccheck = newGenerationRawNeg * arma::diagmat(recombinationWeights) * newGenerationRawNeg.t(); //;in order to check the largest EV
            }
            Cneg = BD * newGenerationRawNeg * arma::diagmat(recombinationWeights) * (BD * newGenerationRawNeg).t();
          } else { //; simple sum
            if (activeCMA % 10 == 1) {
              Ccheck = (1.0 / negMu) * newGenerationRawNeg * newGenerationRawNeg.t(); //;in order to check largest EV
            }
            Cneg = (1.0 / negMu) * BD * newGenerationRawNeg * (BD * newGenerationRawNeg).t();
          }

          //;check pos.def. and set learning rate neg.ccov accordingly, 
          //;this check makes the original choice of neg.ccov extremly failsafe 
          //;still assuming C == BD*BD', which is only approxim. correct 
          if (activeCMA % 10 == 1 &&
              arma::all(1 - negCcov * arma::pow(ngRawNegNorm(ngRawNegNormIndex), 2) * recombinationWeights
              < negMinResidualVariance)) {
            //In matlab there is a long block of comments here about how to calculate these two values
            //TODO: matlab apparently doesn't care that the eigenvalue could be complex.
            //Not sure if simply grabbing the real part here is correct.
            double maxeigenval = arma::max(arma::eig_gen(Ccheck)).real();
            negCcovFinal = std::min(negCcov, (1 - ccovmu)*(1 - negMinResidualVariance)/maxeigenval);
          }
          //;update C
          C = (1 - ccov1 - ccovmu + negAlphaOld * negCcovFinal + (1 - hsig) * ccov1 * ccum * (2 - ccum)) * C //;regard old matrix
              + ccov1 * pc * pc.t() //;plus rank one update
              + (ccovmu + (1 - negAlphaOld) * negCcovFinal) //;plus rank mu update
              * arpos * (arma::repmat(recombinationWeights, 1, numberOfDimensions_) % arpos.t())
              - negCcovFinal * Cneg; //;minus rank mu update
        } else { //; no active (negative) update
          C = (1 - ccov1 - ccovmu + (1 - hsig) * ccov1 * ccum * (2 - ccum)) * C //;regard old matrix
              + ccov1 * pc *pc.t() //;plus rank one update
              + ccovmu * arpos * (arma::repmat(recombinationWeights, 1, numberOfDimensions_) % arpos.t()); //;plus rank mu update
        }
        diagC = arma::diagmat(C);
      }
      
      //;Adapt sigma
      sigma = sigma * std::exp(
          std::min(1.0,
                   (std::sqrt(arma::accu(arma::pow(ps,2)))/chiN - 1) * cs/damping)); //; Eq. (5)
      
      //;Update B and D from C
      if((ccov1+ccovmu+negCcov) > 0 && countiter % 1/((ccov1+ccovmu+negCcov)*numberOfDimensions_*10) < 1) {
        C = arma::symmatu(C); //;enforce symmetry to prevent complex numbers
        arma::Col<double> tmp;
        arma::eig_sym(tmp,B,C); //;eigen decomposition, B==normalized eigenvectors
                              //;effort: approx. 15*N matrix-vector multiplications
        diagD = arma::diagmat(tmp);
        
        //TODO: matlab throws errors here if diagD or B contain non-finite values
        
        //;limit condition of C to 1e14 + 1
        if(arma::min(diagD) <= 0) {
          if(stopOnWarnings) {
            stopFlag = true;
          } else {
            //TODO: warning gets thrown here
            //another workaround
            diagD(arma::find(diagD<0)) = arma::zeros(((arma::uvec)(arma::find(diagD<0))).n_elem);
            tmp = arma::max(diagD)/1e14;
            C = C + tmp*arma::eye(numberOfDimensions_,numberOfDimensions_);
            diagD = diagD + tmp*arma::ones(numberOfDimensions_,1);
          }
        }
        if(arma::max(diagD) > 1e14*arma::min(diagD)) {
          if(stopOnWarnings) {
            stopFlag = true;
          } else {
            //TODO: warning gets thrown here
            tmp = arma::max(diagD)/1e14 - arma::min(diagD);
            C = C + tmp*arma::eye(numberOfDimensions_,numberOfDimensions_);
            diagD = diagD + tmp*arma::ones(numberOfDimensions_,1);
          }
        }
        
        diagC = arma::diagmat(C);
        diagD = arma::sqrt(diagD); //;D contains standard deviations now
        BD = B % arma::repmat(diagD.t(),numberOfDimensions_,1);
      }
      
      //TODO: skipped a code block to "Align/rescale order of magnitude of scales of sigma and C for nicer output"
      //TODO: skipped another code block for flgDiagonalOnly
      
      //;----- numerical error management -----
      //TODO: control these skips
      //;Adjust maximal coordinate axis deviations
      //skipped since the threshold is infinity
      //; Adjust minimal coordinate axis deviations
      //this also should never happen because of positive definite
      //the threshold here is 0
      //;Adjust too low coordinate axis deviations
      if(arma::any(xmean == xmean + 0.2*sigma*arma::sqrt(diagC))) {
        if(stopOnWarnings) {
          stopFlag = true;
        } else {
          //TODO: warning gets thrown here
          C = C + (ccov1+ccovmu) * arma::diagmat(diagC % (xmean == xmean + 0.2*sigma*arma::sqrt(diagC)));
          sigma = sigma * std::exp(0.05+cs/damping);
        }
      }
      //;Adjust step size in case of (numerical) precision problem 
      arma::Mat<double> tmp = 0.1 * sigma*BD.col(std::floor(countiter % numberOfDimensions_));
      if(arma::all(xmean == xmean + tmp)) {
        if(stopOnWarnings) {
          stopFlag = true;
        } else {
          sigma = sigma * std::exp(0.2+cs/damping);
        }
      }
      //;Adjust step size in case of equal function values (flat fitness)
      if(fitnessSel(0) == fitnessSel(std::ceil(0.1+populationSize_/4))) {
        if(stopOnEqualFunctionValues) {
          EqualFunctionValues.shed_row(EqualFunctionValues.n_elem-1);
          EqualFunctionValues.insert_rows(0,countiter);
          if(EqualFunctionValues(EqualFunctionValues.n_elem-1) > countiter - 3 *EqualFunctionValues.n_elem) {
            stopFlag = true;
          }
        } else {
            sigma = sigma * std::exp(0.2+cs/damping);
          }
      }
      //;Adjust step size in case of equal function values
      arma::Col<double> helpVar = fitnessHist;
      helpVar.insert_rows(fitnessHist.n_elem,fitnessSel(0));
      if(countiter > 2 && arma::max(helpVar)-arma::min(helpVar) == 0) {
        if(stopOnWarnings) {
          stopFlag = true;
        } else {
          sigma = sigma * std::exp(0.2+cs/damping);
        }
      }
      
      //;----- end numerical error management -----
      
      //TODO: some output is written here, probably not need for us
      
      //;Set stop flag
      if(fitnessRaw(0) <= getAcceptableObjectiveValue()) {
        stopFlag = true;
      }
      if(getNumberOfIterations() >= getMaximalNumberOfIterations()) {
        stopFlag = true;
      }
      if(countiter >= stopMaxIter) {
        stopFlag = true;
      }
      if(arma::all(arma::all(sigma*(arma::max(arma::abs(pc),arma::sqrt(diagC))) < toleranceX))) {
        stopFlag = true;
      }
      if(arma::any(sigma*arma::sqrt(diagC) > toleranceUpX)) {
        stopFlag = true;
      }
      if(sigma*arma::max(diagD) == 0) {//;should never happen
        stopFlag = true;
      }
      arma::Col<double> helpStop = fitnessHist;
      helpStop.insert_rows(0,fitnessSel);
      if(countiter > 2 && arma::max(helpStop)-arma::min(helpStop) <= toleranceFun) {
        stopFlag = true;
      }
      if(countiter >= fitnessHist.n_elem && arma::max(fitnessHist)-arma::min(fitnessHist) <= toleranceHistFun) {
        stopFlag = true;
      }
      arma::uword l = std::floor(fitnessHistBest.n_elem/3);
      if(1 < 2 && stopOnStagnation &&  //;leads sometimes early stop on ftablet, fcigtab
          countiter > numberOfDimensions_ * (5+100/populationSize_) &&
          fitnessHistBest.n_elem > 100 &&
          arma::median(fitnessHistMedian.rows(0,l-1)) >= arma::median(fitnessHistMedian.rows(fitnessHistMedian.n_elem-1-l,fitnessHistMedian.n_elem-1)) &&
          arma::median(fitnessHistBest.rows(0,l-1)) >= arma::median(fitnessHistBest.rows(fitnessHistBest.n_elem-1-l,fitnessHistBest.n_elem-1))) {
        stopFlag = true;
      }
      
      //TODO: in matlab iterations and funcevals are separated and have an individual limit.
      //there is a separate check here if the number of function evals is higher than the limit
      
      //matlab does some file writing here
      //also does some output generation and formatting
      //both of these are omitted in the modified version for HCMA and should
      //likewise not interest us
    }
    
    //This is not done exactly as in matlab since
      updateBestParameter(newGenerationValid.col(fitnessIdx(0)),fitnessRaw(0));
      double xmeanObjValue = getObjectiveValue(std::get<0>(capToBoundary(xmean)));
      numberOfIterations_++;
      updateBestParameter(xmean,xmeanObjValue);
  }
  
  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getIRun() {
    return irun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setIRun(const arma::uword irun) {
    irun = irun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setLambda0(const double lambda0) {
    lambda0 = lambda0;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStepSize(const arma::Col<double> stepSize) {
    verify(stepSize.n_elem == numberOfDimensions_, "");
    stepSize = stepSize;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setStartingPoint(const arma::Col<double> xStart) {
    //TODO: length check
    startingPoint = xStart;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setPopulationSize(const arma::uword popSize) {
    populationSize_ = popSize;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getIncPopSize() const {
    return incPopSize;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setSingleIteration(const bool DoSingleIteration) {
    singleIteration = DoSingleIteration;
  }

  std::string CovarianceMatrixAdaptationEvolutionStrategy::toString() const noexcept {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcov1() const {
    return ccov1;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcov1(double ccov1) {
    ccov1 = ccov1;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcovmu() const {
    return ccovmu;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcovmu(double ccovmu) {
    ccovmu = ccovmu;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCcum() const {
    return ccum;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCcum(double ccum) {
    ccum = ccum;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getCs() const {
    return cs;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setCs(double cs) {
    cs = cs;
  }

  arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getMu() const {
    return mu;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setMu(arma::uword numberOfParents) {
    mu = numberOfParents;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceFun() const {
    return toleranceFun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceFun(double toleranceFun) {
    toleranceFun = toleranceFun;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceHistFun() const {
    return toleranceHistFun;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceHistFun(double toleranceHistFun) {
    toleranceHistFun = toleranceHistFun;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceUpX() const {
    return toleranceUpX;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceUpX(double toleranceUpX) {
    toleranceUpX = toleranceUpX;
  }

  double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceX() const {
    return toleranceX;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceX(double toleranceX) {
    toleranceX = toleranceX;
  }

  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getXmean() const {
    return xmean;
  }

  void CovarianceMatrixAdaptationEvolutionStrategy::setXmean(arma::Col<double> xmean) {
    xmean = xmean;
  }

  //returns capped matrix/vector first, indexes of capped values second

  std::tuple<arma::Mat<double>, arma::Mat<double>> CovarianceMatrixAdaptationEvolutionStrategy::capToBoundary(arma::Mat<double> x) {
    arma::Mat<double> arbounds = arma::repmat(getLowerBounds(), 1, x.n_cols);
    arma::uvec lowerIndex = arma::find(x < arma::repmat(getLowerBounds(), 1, x.n_cols));
    x(lowerIndex) = arbounds(lowerIndex);

    arbounds = arma::repmat(getUpperBounds(), 1, x.n_cols);
    arma::uvec upperIndex = arma::find(x > arma::repmat(getUpperBounds(), 1, x.n_cols));
    x(upperIndex) = arbounds(upperIndex);

    arma::Mat<double> indexes = arma::zeros(x.n_rows, x.n_cols);
    indexes(lowerIndex) += -1;
    indexes(upperIndex) += 1;

    return std::make_tuple(x, indexes);
  }

  //TODO: not exactly sure what this does. Looks like obtaining a threshold when the lowest percent of values start,
  //but in a weird way that doesn't look right.

  arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::percentiles(arma::Col<double> vector, arma::Col<arma::uword> perc) {
    arma::Col<double> sortedVector = arma::sort(vector);

    arma::uword N = vector.n_elem;
    arma::Col<double> res = arma::Col<double>(perc.n_elem);
    for (arma::uword i = 0; i < perc.n_elem; i++) {
      double p = perc(i);
      if (p <= 100 * (0.5 / N)) {
        res(i) = sortedVector(0);
      } else if (p >= 100 * ((N - 0.5) / N)) {
        res(i) = sortedVector(N - 1);
      } else {
        //TODO: cast to uword
        arma::Col<arma::uword> availablepercentiles = arma::conv_to<arma::Mat < arma::uword>>::from(100 * ((arma::linspace(1, N, N)) - 0.5) / N);
        arma::uword idx = arma::max(arma::find(p > availablepercentiles));
        res(i) = sortedVector(idx) + sortedVector(idx + 1) - sortedVector(idx)*(p - availablepercentiles(idx))
            / (availablepercentiles(idx + 1) - availablepercentiles(i));
      }
    }
    return res;
  }
}
