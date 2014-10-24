#include <hop_bits/optimisationAlgorithm/CovarianceMatrixAdaptationEvolutionStrategy.hpp>

namespace hop {
CovarianceMatrixAdaptationEvolutionStrategy::CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem) : OptimisationAlgorithm(optimisationProblem) {
    //init input parameters
    //TODO iteration number gets multiplied with dimensions² on wiki, also do that here?
    numberOfDimensions_ = optimisationProblem->getNumberOfDimensions();
    objectiveValues_ = arma::randu<arma::vec>(numberOfDimensions_);
    sigma_ = 0.3;
    stopValue_ = 1e-10;
    //init selection parameters
    lambda_ = 4 + floor(3*log(numberOfDimensions_));
    mu_ = lambda_ / 2;
    weights_ = arma::Col<double>(mu_);
    for(int i = 0; i < weights_.n_elem; i++) {
        weights_(i) = log(mu_+1/2)-log(i);
    }
    mu_ = floor(mu_);
    weights_ = weights_/arma::sum(weights_);
    mueff_ = pow(arma::sum(weights_),2)/arma::sum(arma::square(weights_));
    //init adaptation parameters
    cc_ = (4+mueff_/numberOfDimensions_) / (numberOfDimensions_+4 + 2*mueff_/numberOfDimensions_);
    cs_ = (mueff_+2) / (numberOfDimensions_ + mueff_ + 5);
    c1_ = 2 / (pow(numberOfDimensions_+1.3,2)+mueff_);
    cmu_ = arma::min(arma::vec(1-c1_,2*(mueff_ - 2+1/mueff_) / (pow(numberOfDimensions_+2,2)+mueff_)));
    damps_ = 1 + 2*arma::max(arma::vec(0.0,sqrt((mueff_-1)/(numberOfDimensions_+1))-1)) + cs_;
    //init dynamic parameters and constants
    pc_.zeros(numberOfDimensions_);
    ps_.zeros(numberOfDimensions_);
    B_.eye(numberOfDimensions_,numberOfDimensions_);
    D_.ones(numberOfDimensions_,1);
    C_ = B_ * diagmat(square(D_)) * B_.t();
    invsqrtC_ = B_ * diagmat(D_.i()) * B_.t();
    eigeneval_ = 0;
    chiN_ = pow(numberOfDimensions_,0.5)*(1-1/(4*numberOfDimensions_)+1/(21*pow(numberOfDimensions_,2)));
}

void CovarianceMatrixAdaptationEvolutionStrategy::optimiseImplementation() {
    while(!isFinished() && !isTerminated()) {
        //generate and evaluate lambda offspring
        arma::Col<double> arfitness = arma::Col<double>(lambda_);
        for(int k =0; k < lambda_; k++) {
            arfitness(k) = frosenbrock(objectiveValues_ + sigma_ * B_ * (D_ * arma::randn<arma::vec>(numberOfDimensions_)));
            ++numberOfIterations_;
        }

        //
    }
}

std::string CovarianceMatrixAdaptationEvolutionStrategy::to_string() const {
    return "CovarianceMatrixAdaptationEvolutionStrategy";
}

double CovarianceMatrixAdaptationEvolutionStrategy::frosenbrock(arma::Col<double> x) {
    if(x.n_elem<2) {
        throw std::logic_error("The dimension must be greater than one!");
    } else {
        double firstTerm = 100*arma::sum(arma::square(arma::square(x.rows(0,x.n_elem-2))-x.rows(1,x.n_elem-1)));
        arma::Col<double> innerStuff = x.rows(0,x.n_elem-2);
        innerStuff--;
        double secondTerm = arma::sum(arma::square(innerStuff));
        return  firstTerm+secondTerm;
    }
}
}
