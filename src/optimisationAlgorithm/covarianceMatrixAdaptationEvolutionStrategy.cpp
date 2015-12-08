#include "mantella_bits/optimisationAlgorithm/covarianceMatrixAdaptationEvolutionStrategy.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {

    CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy()
    : OptimisationAlgorithm(),
      lambda_(arma::datum::nan),
      sigma_(arma::datum::nan),
      toleranceFun_(arma::datum::nan),
      toleranceHistFun_(arma::datum::nan),
      toleranceX_(arma::datum::nan),
      toleranceUpX_(arma::datum::nan),
      activeCMA_(arma::datum::nan),
      stopMaxIter_(arma::datum::nan) 
      {
        setNextParametersFunction([this] (const arma::Mat<double>& parameters, const arma::Col<double>& differences) {
            arma::uword numberOfDimensions = parameters.n_rows;

            this->newGenerationValid_ = parameters; //arxvalid
            //TODO: using the differences now, it should have no effect on CMAES as far as i can see
            this->fitnessRaw_ = differences;

            //TODO: this stopflag is more sophisticated in the matlab code.
            bool stopFlag = false;
            //;set internal parameters
            if (this->lambda_ != this->lambda_last_) {
                this->setPopulationSize(this->lambda_,numberOfDimensions);
            }

            this->countiter_++;

            //;Generate and evaluate lambda offspring
            this->fitnessRaw_ = arma::ones(this->lambda_);

            //;----- handle boundaries -----
            //;Assigned penalized fitness
            arma::Col<double> boundaryPenalty = arma::sum(arma::abs(newGenerationValid_ - newGeneration_)).t();
            fitnessSel_ = fitnessRaw_ + boundaryPenalty;
            //;----- end handle boundaries -----

            //;Sort by fitness
            fitnessIdx_ = arma::sort_index(fitnessRaw_);
            fitnessRaw_ = arma::sort(fitnessRaw_);
            fitnessIdxSel_ = arma::sort_index(fitnessSel_);
            fitnessSel_ = arma::sort(fitnessSel_); //;minimization

            //;Calculate new xmean, this is selection and recombination 
            xold_ = xmean_; //;for speed up of Eq. (2) and (3)
            xmean_ = newGeneration_.cols(fitnessIdxSel_.rows(0, mu_ - 1)) * recombinationWeights_;
            arma::Mat<double> zmean = newGenerationRaw_.cols(fitnessIdxSel_.rows(0, mu_ - 1)) * recombinationWeights_; //;==D^-1*B'*(xmean-xold)/sigma

            //;Cumulation: update evolution paths
            ps_ = (1 - cs_) * ps_ + std::sqrt(cs_ * (2 - cs_) * mueff_) * (B_ * zmean); //;Eq. (4)
            double hsig = arma::norm(ps_) / std::sqrt(1 - std::pow(1 - cs_, 2 * countiter_)) / chiN_ < 1.4 + 2.0 / (numberOfDimensions + 1);

            pc_ = (1 - ccum_) * pc_ + hsig * (std::sqrt(ccum_ * (2 - ccum_) * mueff_) / sigma_) * (xmean_ - xold_); //;Eq. (2)

            //;Adapt covariance matrix
            negCcov_ = 0;

            if ((ccov1_ + ccovmu_) > 0) {
                arma::Mat<double> arpos = static_cast<arma::Mat<double>>(newGeneration_.cols(fitnessIdxSel_.rows(0, mu_ - 1))).each_col() - (xold_ / sigma_);
                //;"active" CMA update: negative update, in case controlling pos. definiteness 
                if (activeCMA_ > 0) {
                    //;set parameters
                    double negMu = mu_;
                    double negMueff = mueff_;
                    if (activeCMA_ > 10) {//;flat weights with mu=lambda/2
                        negMu = std::floor(lambda_ / 2);
                        negMueff = negMu;
                    }
                    /*;
                    % ;neg.mu = ceil(min([N, lambda/4, mueff]));  neg.mueff = mu; % i.e. neg.mu <= N 
                    % ;Parameter study: in 3-D lambda=50,100, 10-D lambda=200,400, 30-D lambda=1000,2000 a 
                    % ;three times larger neg.ccov does not work. 
                    % ;  increasing all ccov rates three times does work (probably because of the factor (1-ccovmu))
                    % ;  in 30-D to looks fine
                     */

                    negCcov_ = (1 - ccovmu_) * 0.25 * negMueff / (std::pow(numberOfDimensions + 2, 1.5) + 2 * negMueff);
                    double negMinResidualVariance = 0.66; //;keep at least 0.66 in all directions, small popsize are most critical
                    double negAlphaOld = 0.5; //;where to make up for the variance loss, 0.5 means no idea what to do
                    //;1 is slightly more robust and gives a better "guaranty" for pos. def., 
                    //;but does it make sense from the learning perspective for large ccovmu?

                    double negCcovFinal = negCcov_;

                    //;prepare vectors, compute negative updating matrix Cneg and checking matrix Ccheck
                    //have to do some workaround since you cannot create negative descending sequences with arma
                    arma::Col<arma::uword> negHelper = arma::conv_to<arma::uvec>::from(
                    -arma::linspace(-(lambda_ * 1.0 - 1), -(lambda_ * 1.0 - negMu), std::abs(negMu)));
                    arma::Mat<double> newGenerationRawNeg = newGenerationRaw_.cols(fitnessIdxSel_.rows(negHelper)); //arzneg
                    //;i-th longest becomes i-th shortest

                    arma::Col<arma::uword> ngRawNegNormIndex = arma::sort_index(arma::sqrt(arma::sum(arma::square(newGenerationRawNeg), 0))); //idxnorms
                    arma::Col<double> ngRawNegNorm = arma::sort(arma::sqrt(arma::sum(arma::square(newGenerationRawNeg), 0))).t(); //arnorms
                    ngRawNegNormIndex = arma::sort_index(ngRawNegNormIndex); //;inverse index
                    //another workaround for negative ordering
                    negHelper = arma::conv_to<arma::uvec>::from(
                    -arma::linspace(-(ngRawNegNorm.n_elem * 1.0 - 1), 0, ngRawNegNorm.n_elem));
                    arma::Col<double> ngRawNegNormFacs = ngRawNegNorm(negHelper) / ngRawNegNorm; //arnormfacs
                    ngRawNegNorm = ngRawNegNormFacs; //;for the record
                    if (activeCMA_ < 20) {
                        newGenerationRawNeg = newGenerationRawNeg.each_row() % ngRawNegNormFacs.t(); //;E x*x' is N
                    }
                    arma::Mat<double> Ccheck; //intentional spelling
                    arma::Mat<double> Cneg;
                    if (activeCMA_ < 10 && negMu == mu_) { //;weighted sum
                        if (activeCMA_ % 10 == 1) {
                            Ccheck = newGenerationRawNeg * arma::diagmat(recombinationWeights_) * newGenerationRawNeg.t(); //;in order to check the largest EV
                        }
                        Cneg = BD_ * newGenerationRawNeg * arma::diagmat(recombinationWeights_) * (BD_ * newGenerationRawNeg).t();
                    } else { //; simple sum
                        if (activeCMA_ % 10 == 1) {
                            Ccheck = (1.0 / negMu) * newGenerationRawNeg * newGenerationRawNeg.t(); //;in order to check largest EV
                        }
                        Cneg = (1.0 / negMu) * BD_ * newGenerationRawNeg * (BD_ * newGenerationRawNeg).t();
                    }

                    //;check pos.def. and set learning rate neg.ccov accordingly, 
                    //;this check makes the original choice of neg.ccov extremly failsafe 
                    //;still assuming C == BD*BD', which is only approxim. correct 
                    if (activeCMA_ % 10 == 1 &&
                            arma::all(1 - negCcov_ * arma::square(ngRawNegNorm(ngRawNegNormIndex)).t() * recombinationWeights_
                            < negMinResidualVariance)) {
                        //In matlab there is a long block of comments here about how to calculate these two values
                        //TODO: matlab apparently doesn't care that the eigenvalue could be complex.
                        //Not sure if simply grabbing the real part here is correct.
                        double maxeigenval = arma::max(arma::eig_gen(Ccheck)).real();
                        negCcovFinal = std::min(negCcov_, (1 - ccovmu_)*(1 - negMinResidualVariance) / maxeigenval);
                    }
                    //;update C
                    C_ = (1 - ccov1_ - ccovmu_ + negAlphaOld * negCcovFinal + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
                            + ccov1_ * pc_ * pc_.t() //;plus rank one update
                            + (ccovmu_ + (1 - negAlphaOld) * negCcovFinal) //;plus rank mu update
                            * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_)
                            - negCcovFinal * Cneg; //;minus rank mu update
                } else { //; no active (negative) update
                    C_ = (1 - ccov1_ - ccovmu_ + (1 - hsig) * ccov1_ * ccum_ * (2 - ccum_)) * C_ //;regard old matrix
                            + ccov1_ * pc_ * pc_.t() //;plus rank one update
                            + ccovmu_ * arpos * (static_cast<arma::Mat<double>>(arpos.t()).each_col() % recombinationWeights_); //;plus rank mu update
                }
                diagC_ = arma::diagvec(C_);
            }

            //;Adapt sigma
            sigma_ = sigma_ * std::exp(
                    std::min(1.0,
                    (std::sqrt(arma::accu(arma::square(ps_))) / chiN_ - 1) * cs_ / damping_)); //; Eq. (5)

                    //;Update B and D from C
            if ((ccov1_ + ccovmu_ + negCcov_) > 0 && countiter_ % 1 / ((ccov1_ + ccovmu_ + negCcov_) * numberOfDimensions * 10) < 1) {
                C_ = arma::symmatu(C_); //;enforce symmetry to prevent complex numbers
                arma::Col<double> tmp;
                arma::eig_sym(tmp, B_, C_); //;eigen decomposition, B==normalized eigenvectors
                //;effort: approx. 15*N matrix-vector multiplications
                //Matlab returns eigenvalues as a diagonal matrix, so they do diagD = diag(tmp) here
                diagD_ = tmp;

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
                        diagD_(arma::find(diagD_ < 0)) = arma::zeros(((arma::uvec)(arma::find(diagD_ < 0))).n_elem);
                        tmp = arma::max(diagD_) / 1e14;
                        C_ = C_ + tmp * arma::eye(numberOfDimensions, numberOfDimensions);
                        diagD_ = diagD_ + tmp * arma::ones(numberOfDimensions, 1);
                    }
                }
                if (arma::max(diagD_) > 1e14 * arma::min(diagD_)) {
                    if (stopOnWarnings_) {
                        stopFlag = true;
                    } else {
                        //TODO: warning gets thrown here
                        tmp = arma::max(diagD_) / 1e14 - arma::min(diagD_);
                        C_ = C_ + tmp * arma::eye(numberOfDimensions, numberOfDimensions);
                        diagD_ = diagD_ + tmp * arma::ones(numberOfDimensions, 1);
                    }
                }

                diagC_ = arma::diagvec(C_);
                diagD_ = arma::sqrt(diagD_); //;D contains standard deviations now
                BD_ = B_.each_col() % diagD_;
            }

            //;----- numerical error management -----
            //TODO: control these skips
            //;Adjust maximal coordinate axis deviations
            //skipped since the threshold is infinity
            //; Adjust minimal coordinate axis deviations
            //this also should never happen because of positive definite
            //the threshold here is 0
            //;Adjust too low coordinate axis deviations
            if (arma::any(xmean_ == xmean_ + 0.2 * sigma_ * arma::sqrt(diagC_))) {
                if (stopOnWarnings_) {
                    stopFlag = true;
                } else {
                    //TODO: warning gets thrown here
                    C_ = C_ + (ccov1_ + ccovmu_) * arma::diagmat(diagC_ % (xmean_ == xmean_ + 0.2 * sigma_ * arma::sqrt(diagC_)));
                    sigma_ = sigma_ * std::exp(0.05 + cs_ / damping_);
                }
            }
            //;Adjust step size in case of (numerical) precision problem 
            arma::Mat<double> tmp = 0.1 * sigma_ * BD_.col(std::floor(countiter_ % numberOfDimensions));
            if (arma::all(xmean_ == xmean_ + tmp)) {
                if (stopOnWarnings_) {
                    stopFlag = true;
                } else {
                    sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
                }
            }
            //;Adjust step size in case of equal function values (flat fitness)
            if (fitnessSel_(0) == fitnessSel_(std::ceil(0.1 + lambda_ / 4))) {
                if (stopOnEqualFunctionValues_) {
                    EqualFunctionValues_.shed_row(EqualFunctionValues_.n_elem - 1);
                    EqualFunctionValues_.insert_rows(0, countiter_);
                    if (EqualFunctionValues_(EqualFunctionValues_.n_elem - 1) > countiter_ - 3 * EqualFunctionValues_.n_elem) {
                        stopFlag = true;
                    }
                } else {
                    sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
                }
            }
            //;Adjust step size in case of equal function values
            //replacement of fitnesshist
            if (countiter_ > 2 && arma::all(fitnessRaw_ - fitnessRawPreviousIteration_ == 0)) {
                if (stopOnWarnings_) {
                    stopFlag = true;
                } else {
                    sigma_ = sigma_ * std::exp(0.2 + cs_ / damping_);
                }
            }

            //;----- end numerical error management -----
            //;Set stop flag
            //TODO: how to handle these now?
            if (countiter_ >= stopMaxIter_) {
                stopFlag = true;
            }
            if (arma::all(sigma_ * (arma::abs(pc_), arma::sqrt(diagC_)) < toleranceX_)) {
                stopFlag = true;
            }
            if (arma::any(sigma_ * arma::sqrt(diagC_) > toleranceUpX_)) {
                stopFlag = true;
            }
            if (sigma_ * arma::max(diagD_) == 0) {//;should never happen
                stopFlag = true;
            }

            //TODO: in matlab iterations and funcevals are separated and have an individual limit.
            //there is a separate check here if the number of function evals is higher than the limit

            //matlab does some file writing here
            //also does some output generation and formatting
            //both of these are omitted in the modified version for HCMA and should
            //likewise not interest us

            //TODO: replacement of fitnesshist for plateau detection, so we can do the same checks as cmaes does
            //but in a less refined and data heavy way
            fitnessRawPreviousIteration_ = fitnessSel_;
            
            //TODO: This code was actually at the very beginning of the while loop
            //If HCMA changes lambda inbetween, this might need some handling
            newGenerationRaw_ = arma::randn(numberOfDimensions, lambda_); //arz
            newGeneration_ = static_cast<arma::Mat<double>>(this->sigma_ * (this->BD_ * newGenerationRaw_)).each_col() + this->xmean_; //arx
            
            return newGeneration_;


            //TODO: What to do with this? (see email)
            //        updateBestParameter(newGenerationValid_.col(fitnessIdx_(0)), fitnessRaw_(0));
            //        double xmeanObjValue = getObjectiveValue(std::get<0>(capToBoundary(xmean_)));
            //        numberOfIterations_++;
            //        updateBestParameter(xmean_, xmeanObjValue);
        });



        //HCMA settings for later use:
        /*
         * setStepSize(0.2);
              //originally (getUpperBounds() - getLowerBounds()) / 5.0
              //but since all problems are within 0 to 1 now, we can just put in 0.2
         setToleranceFun(1e-12);
        setToleranceHistFun(1e-12);
        setToleranceX(2e-12);
        setToleranceUpX(arma::datum::inf);
        setIRun(0);
        setActiveCMA(0);
        countiter_ = 0;
        stopOnStagnation_ = true;
        stopOnWarnings_ = true;
        stopOnEqualFunctionValues_ = true;
         * stopMaxIter_ = 100 + 50 * std::pow(optimisationProblem->numberOfDimensions_ + 3, 2) / std::sqrt(lambda_);
         */

    }

    void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
            const std::shared_ptr<OptimisationProblem> optimisationProblem) {
        optimise(optimisationProblem, arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_));
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::optimise(
            const std::shared_ptr<OptimisationProblem> optimisationProblem,
            const arma::Mat<double>& initialParameters) {
        verify(initialParameters.n_cols == 1, "optimise: The cmaes algorithm accepts only a single initial parameter.");

        xmean_ = initialParameters.col(0);
        xold_ = xmean_;

        //TODO: opts.IncPopSize is actually (somewhat) used in HCMA when Bipop is activated. (see xacmes.m)
        //Still completely remove it?
        if (!std::isfinite(lambda_)) {
            setPopulationSize(4 + std::floor(3 * std::log(optimisationProblem->numberOfDimensions_)), optimisationProblem->numberOfDimensions_);
        }
        if (!std::isfinite(sigma_)) {
            setStepSize(0.2);
            //originally (getUpperBounds() - getLowerBounds()) / 5.0
            //but since all problems are within 0 to 1 now, we can just put in 0.2
        }
        if (!std::isfinite(toleranceFun_)) {
            setToleranceFun(1e-12);
        }
        if (!std::isfinite(toleranceHistFun_)) {
            setToleranceHistFun(1e-13);
        }
        if (!std::isfinite(toleranceX_)) {
            setToleranceX(1e-11 * sigma_);
        }
        if (!std::isfinite(toleranceUpX_)) {
            setToleranceUpX(1e3 * sigma_);
        }
        if (!std::isfinite(activeCMA_)) {
            setActiveCMA(0);
        }
        if (!std::isfinite(stopMaxIter_)) {
            stopMaxIter_ = 1e3 * std::pow(optimisationProblem->numberOfDimensions_ + 5, 2) / std::sqrt(lambda_);
        }

        pc_ = arma::zeros(optimisationProblem->numberOfDimensions_);
        ps_ = arma::zeros(optimisationProblem->numberOfDimensions_);

        diagD_ = arma::ones(optimisationProblem->numberOfDimensions_) * sigma_;
        //TODO: "diagonal matrix D defines the scaling" originally this was stepsize / max(stepsize)
        //where stepsize was the vector (which never got used) of stepsizes in all dimensions
        //what actually got used was max(stepsize) - we decided to omit this vector so diagD as a whole
        //might be irrelevant
        diagC_ = arma::square(diagD_);
        B_ = arma::eye(optimisationProblem->numberOfDimensions_, optimisationProblem->numberOfDimensions_); //;B defines the coordinate system
        BD_ = arma::diagmat(diagD_); //;B*D for speed up only
        C_ = arma::diagmat(diagC_); //;covariance matrix == BD*(BD)'

        chiN_ = std::pow(optimisationProblem->numberOfDimensions_, 0.5) *
                (1 - 1.0 / (4 * optimisationProblem->numberOfDimensions_) + 1.0
                / (21 * std::pow(optimisationProblem->numberOfDimensions_, 2)));
        //;expectation of||N(0,I)|| == norm(randn(N,1))

        //miscellaneous inits needed
        EqualFunctionValues_ = arma::zeros(10 + optimisationProblem->numberOfDimensions_);
        stopOnStagnation_ = true;
        stopOnWarnings_ = true;
        stopOnEqualFunctionValues_ = true;
        //more complicated in matlab, defines a number of iterations of equal funcvalues to stop

        //Need to do this here once to get from the initial starting point (given as intiailparameters)
        //to the first round of points to get evaluated
        newGenerationRaw_ = arma::randn(optimisationProblem->numberOfDimensions_, lambda_); //arz
        this->newGeneration_ = static_cast<arma::Mat<double>>(this->sigma_ * (this->BD_ * newGenerationRaw_)).each_col() + this->xmean_; //arx

        //newGeneration will have turned into newGenerationValid_ when nextParameters is called
        OptimisationAlgorithm::optimise(optimisationProblem, this->newGeneration_);
    }

    arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getIRun() {
        return irun_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setIRun(const arma::uword irun) {
        this->irun_ = irun;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setStepSize(const double stepSize) {
        this->sigma_ = stepSize;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getStepSize() {
        return sigma_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setPopulationSize(const arma::uword popSize, const arma::uword numberOfDimensions) {
        this->lambda_ = popSize;
        lambda_last_ = lambda_;
        mu_ = std::floor(lambda_ / 2.0);
        recombinationWeights_ = std::log(mu_ + 0.5) - arma::log(arma::linspace(1, mu_, mu_));

        mueff_ = std::pow(arma::accu(recombinationWeights_), 2) / arma::accu(arma::square(recombinationWeights_)); //;variance-effective size of mu
        recombinationWeights_ = arma::normalise(recombinationWeights_, 1); //;normalize recombination weights array
        //error check omitted, shouldn't happen

        //TODO: following code is dependent on the number of dimensions, so the user has to provide it here
        //(HCMA modifies popsize later, thats why the code cant be moved really)
        ccum_ = std::pow((numberOfDimensions + 2 * mueff_ / numberOfDimensions) / (4 + mueff_ / numberOfDimensions), -1); //;time constant for cumulation for covariance matrix
        cs_ = (mueff_ + 2) / (numberOfDimensions + mueff_ + 3);

        ccov1_ = std::min(2.0, lambda_ / 3.0) / (std::pow(numberOfDimensions + 1.3, 2) + mueff_);
        ccovmu_ = std::min(2.0, lambda_ / 3.0) / (mueff_ - 2 + 1.0 / mueff_) / (std::pow(numberOfDimensions + 2, 2) + mueff_);

        damping_ = 1 + 2 * std::max(0.0, std::sqrt((mueff_ - 1) / (numberOfDimensions + 1)) - 1) + cs_;

    }

    arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getPopulationSize() {
        return this->lambda_;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getCcov1() const {
        return ccov1_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setCcov1(double ccov1) {
        this->ccov1_ = ccov1;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getCcovmu() const {
        return ccovmu_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setCcovmu(double ccovmu) {
        this->ccovmu_ = ccovmu;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getCcum() const {
        return ccum_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setCcum(double ccum) {
        this->ccum_ = ccum;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getCs() const {
        return cs_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setCs(double cs) {
        this->cs_ = cs;
    }

    arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getMu() const {
        return mu_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setMu(arma::uword numberOfParents) {
        this->mu_ = numberOfParents;
    }

    arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getRecombinationWeights() {
        return recombinationWeights_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setRecombinationWeights(arma::Col<double> weights) {
        this->recombinationWeights_ = weights;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceFun() const {
        return toleranceFun_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceFun(double toleranceFun) {
        this->toleranceFun_ = toleranceFun;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceHistFun() const {
        return toleranceHistFun_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceHistFun(double toleranceHistFun) {
        this->toleranceHistFun_ = toleranceHistFun;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceUpX() const {
        return toleranceUpX_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceUpX(double toleranceUpX) {
        this->toleranceUpX_ = toleranceUpX;
    }

    double CovarianceMatrixAdaptationEvolutionStrategy::getToleranceX() const {
        return toleranceX_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setToleranceX(double toleranceX) {
        this->toleranceX_ = toleranceX;
    }

    arma::Col<double> CovarianceMatrixAdaptationEvolutionStrategy::getXmean() const {
        return xmean_;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setXmean(arma::Col<double> xmean) {
        this->xmean_ = xmean;
    }

    void CovarianceMatrixAdaptationEvolutionStrategy::setActiveCMA(arma::uword activeCma) {
        this->activeCMA_ = activeCma;
    }

    arma::uword CovarianceMatrixAdaptationEvolutionStrategy::getActiveCMA() {
        return activeCMA_;
    }

    //returns capped matrix/vector first, indexes of capped values second

    std::tuple<arma::Mat<double>, arma::Mat<double>> CovarianceMatrixAdaptationEvolutionStrategy::capToBoundary(arma::Mat<double> x) {
        arma::Mat<double> indexes = arma::zeros(x.n_rows, x.n_cols);
        for (arma::uword n = 0; n < x.n_cols; ++n) {
            //const arma::Col<arma::uword>& lowerIndex = arma::find(x.col(n) < getLowerBounds());
            //static_cast<arma::Col<double>>(x.col(n)).elem(lowerIndex) = getLowerBounds().elem(lowerIndex);
            //indexes(lowerIndex) += -1;

            //const arma::Col<arma::uword>& upperIndex = arma::find(x.col(n) > getUpperBounds());
            //static_cast<arma::Col<double>>(x.col(n)).elem(upperIndex) = getUpperBounds().elem(upperIndex);
            //indexes(upperIndex) += 1;
        }

        return std::make_tuple(x, indexes);
    }
}
