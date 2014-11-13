#include <hop_bits/optimisationAlgorithm/CovarianceMatrixAdaptationEvolutionStrategy.hpp>

namespace hop {
CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem) : OptimisationAlgorithm(optimisationProblem) {
    // init input parameters
    // TODO: iteration number gets multiplied with dimensions² on wiki, also do that here?
    numberOfDimensions_ = optimisationProblem->getNumberOfDimensions();
    objectiveValues_ = arma::randu<arma::vec>(numberOfDimensions_);
    sigma_ = 0.3;
    stopValue_ = 1e-10;
    //init selection parameters
    lambda_ = 4 + std::floor(3 * std::log(numberOfDimensions_));
    mu_ = lambda_ / 2;
    weights_ = arma::Col<double>(mu_);
    
    for(std::size_t n = 0; n < weights_.n_elem; ++n) {
        weights_(n) = log(mu_ + 0.5) - log(n);
    }
    
    mu_ = std::floor(mu_);
    weights_ = weights_ / arma::sum(weights_);
    mueff_ = std::pow(arma::sum(weights_), 2) / arma::sum(arma::square(weights_));
    //init adaptation parameters
    cc_ = (4 + mueff_ / numberOfDimensions_) / (numberOfDimensions_ + 4 + 2 * mueff_ / numberOfDimensions_);
    cs_ = (mueff_+2) / (numberOfDimensions_ + mueff_ + 5);
    c1_ = 2 / (std::pow(numberOfDimensions_ + 1.3, 2) + mueff_);
    cmu_ = std::min(1 - c1_, 2 * (mueff_ - 2 + 1 / mueff_) / (std::pow(numberOfDimensions_ + 2, 2) + mueff_));
    damps_ = 1 + 2 * std::max(0.0, std::sqrt((mueff_ - 1) / (numberOfDimensions_ + 1)) - 1) + cs_;
    //init dynamic parameters and constants
    pc_.zeros(numberOfDimensions_);
    ps_.zeros(numberOfDimensions_);
    B_.eye(numberOfDimensions_, numberOfDimensions_);
    D_.ones(numberOfDimensions_,1);
    C_ = B_ * arma::diagmat(square(D_)) * B_.t();
    invsqrtC_ = B_ * arma::diagmat(D_.i()) * B_.t();
    eigeneval_ = 0;
    chiN_ = std::sqrt(numberOfDimensions_) * (1 - 1 / (4 * numberOfDimensions_) + 1 / (21 * std::pow(numberOfDimensions_, 2)));
}

void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    while(!isFinished() && !isTerminated()) {
        //generate and evaluate lambda offspring
        arma::Col<double> arfitness = arma::Col<double>(lambda_);
        arma::Mat<double> arx = arma::Mat<double>(numberOfDimensions_, lambda_);
        for(std::size_t n = 0; n < lambda_; ++n) {
            arx.col(n) = objectiveValues_ + sigma_ * B_ * (D_ * arma::randn<arma::Col<double>>(numberOfDimensions_));
            arfitness(n) = frosenbrock(arx.col(n));
            ++numberOfIterations_;
        }
        //sort by fitness and compute weighted mean into objectiveValues_
        arma::Col<arma::uword> arindex = arma::sort_index(arfitness);
        arma::Col<double> oldObjectiveValues = objectiveValues_;
        //next line should be unnecessary, depending on if arma returns a copy or not in the line after that
        objectiveValues_ = arma::Col<double>(numberOfDimensions_);
        //TODO: xmean = arx(:,arindex(1:mu))*weights;
        //is the actual matlab syntax for the following line, not a 100% sure if this is correct
        objectiveValues_ = arx.cols(arindex) * weights_;

        //cumulation: update evolution paths
        ps_ = (1 - cs_) * ps_ + std::sqrt(cs_ * (2 - cs_) * mueff_) * invsqrtC_ * (objectiveValues_ - oldObjectiveValues) / sigma_;
        int hsig = arma::norm(ps_) / std::sqrt(std::pow(1 - (1 - cs_), 2 * numberOfIterations_ / lambda_)) / chiN_ < 1.4 + 2 / (numberOfDimensions_ + 1);
        pc_ = (1 - cc_) * pc_ + hsig * std::sqrt(cc_ * (2-cc_) * mueff_) * (objectiveValues_ - oldObjectiveValues) / sigma_;

        //adapt covariance matrix C
        arma::Mat<double> artmp = (1 / sigma_) * arx.cols(arindex) - arma::repmat(oldObjectiveValues, 1, mu_);
        C_ = (1-c1_-cmu_) * C_ + //old matrix
                c1_ * (pc_*pc_.t() + //rank one update
                       (1-hsig) * cc_ * (2-cc_) * C_) + //correction for hsig==0
                cmu_ * artmp * arma::diagmat(weights_) * artmp.t(); // rank mu update

        //adapt step size sigma
        sigma_ = sigma_ * std::exp((cs_ / damps_) * (arma::norm(ps_) / chiN_ - 1));

        //decomposition of C into B*diag(D.^2)*B' (diagonalization)
        if((numberOfIterations_ - eigeneval_) > (lambda_ / (c1_ + cmu_) / numberOfIterations_ / 10)) {
            eigeneval_ = numberOfIterations_;
            //TODO: C = triu(C) + triu(C,1)';
            //is the actual matlab syntax for the next lines, unfortunately arma doesn't have triu with a second parameter (yet)
            arma::Mat<double> tempC = arma::trimatu(C_);
            tempC.diag().zeros();
            C_ = arma::trimatu(C_) + tempC.t(); //enforce symmetry
            //matlab code is [B,D] = eig(C); arma uses switched arguments according to API
            arma::Col<double> tempD = arma::Col<double>();
            arma::eig_sym(tempD, B_, C_);
            D_ = arma::sqrt(arma::diagmat(tempD)); //TODO: From wiki "D is a vector of standard deviations now" which it obviously isn't here.
                                                //also not sure what the point of that is, since it's going to be used as a matrix in the next line anyway.
            invsqrtC_ = B_ * arma::diagmat(D_.i()) * B_.t();
        }

        //break, if fitness is good enough or condition exceeds 1e14, better termination methods are advisable
        if(arfitness.at(1) <= stopValue_ || D_.max() / D_.min() > 1e7) {
            break;
        }
    }
    //while loop has ended

    //TODO: matlab returns xmin = arx(:, arindex(1));
    //not sure where to safe our return value?
}

std::string CovarianceMatrixAdaptationEvolutionStrategy::to_string() const {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
}

double CovarianceMatrixAdaptationEvolutionStrategy::frosenbrock(arma::Col<double> x) {
    if(x.n_elem < 2) {
        throw std::logic_error("The dimension must be greater than one!");
    } else {
        double firstTerm = 100 * arma::sum(arma::square(arma::square(x.rows(0, x.n_elem - 2)) - x.rows(1, x.n_elem - 1)));
        arma::Col<double> innerStuff = x.rows(0, x.n_elem - 2);
        double secondTerm = arma::sum(arma::square(innerStuff - 1));
        return  firstTerm + secondTerm;
    }
}
}
