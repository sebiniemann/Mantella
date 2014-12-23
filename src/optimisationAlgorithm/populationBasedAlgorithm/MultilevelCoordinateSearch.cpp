#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/MultilevelCoordinateSearch.hpp>

namespace hop {

  MultilevelCoordinateSearch::MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int& populationSize, arma::Mat<double> boundaries, unsigned int boxDivisions, unsigned int maxLocalSearchSteps, double localStopThreshold = arma::datum::eps, arma::Mat<double> hess = arma::ones(size(boundaries.row(0))), unsigned int initialPointIndex)
  : PopulationBasedAlgorithm<double>(optimisationProblem, populationSize), boxDivisions_(boxDivisions), boundaries_(boundaries),
  maxLocalSearchSteps_(maxLocalSearchSteps), localStopThreshold_(localStopThreshold), hess_(hess), initialPointIndex_(initialPointIndex) {

    unsigned int numberOfDimensions = optimisationProblem_->getNumberOfDimensions();
    //length of u or v and thus length of hess should equal numberOfDimensions
    if (numberOfDimensions != boundaries.col(0).n_elem) {
      std::cout << "lower boundaries dimensions dont match!" << std::endl;
    }
    if (numberOfDimensions != boundaries.col(1).n_elem) {
      std::cout << "upper boundaries dimensions dont match!" << std::endl;
    }
    if (numberOfDimensions != hess.row(0).n_elem && optimisationProblem_->getNumberOfDimensions() != hess.col(0).n_elem) {
      std::cout << "hess dimensions dont match!" << std::endl;
    }
    //Check if bounds are malformed
    for (int i = 0; i < numberOfDimensions; i++) {
      if (boundaries(i, 0) >= boundaries(i, 1)) {
        std::cout << "boundary malformed! u: " << boundaries(i, 0) << ", v:" << boundaries(i, 1) << std::endl;
      }
    }

    //init of large arrays
    //TODO: type completely unclear right now, pdf page 6f
    isplit_(step1_, arma::fill::zeros);
    level_(step1_, arma::fill : zeros);
    ipar_(step1_, arma::fill : zeros);
    ichild_(step1_, arma::fill : zeros);
    f_(step1_, 2, arma::fill : zeros);
    z_(step1_, 2, arma::fill : zeros);
    nogain_(step1_, arma::fill : zeros);

    //initialization list
    //TODO: not sure i fully understand what's happening here yet. see pdf page 6f, mcs.m at 225, runmcs.m at 160, init0.m and init.m
    //this is the equivalent of iinit = 0 in matlab
////    x0_.col(0) = boundaries_.col(0);
////    x0_.col(1) = (boundaries_.col(0) + boundaries_.col(1)) / 2.0;
////    x0_.col(2) = boundaries_.col(1);
////    initialPointIndex_(numberOfDimensions, 2);

    //TODO: for custom initialization lists there is a check here to see if they violate the boundaries


    //l_ L and x0_ are the custom initilization list variables
    //l_ is supposed to point to x^0 in x0_ - it seems overkill to use a list instead of a fixed index
    //l_ also never gets changed in matlab as far as i could see
    //L gives the amount of predefined values per dimension (basically populationSize_ with more finetuning possible)
    //TODO: mcs.m does infinity check on x0_ here, not sure if needed


  }

  void MultilevelCoordinateSearch::optimiseImplementation() {

  }

  std::string MultilevelCoordinateSearch::to_string() const noexcept {
    return "MultilevelCoordinateSearch";
  }
}