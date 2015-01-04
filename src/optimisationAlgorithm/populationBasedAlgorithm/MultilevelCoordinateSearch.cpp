#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/MultilevelCoordinateSearch.hpp>


////COMMENTS
//step1_ is used at a lot of places where matrix/column sizes arent calculable in advance (since size is dynamic in matlab).
//some variables have been renamed, there original name is noted in the header file (if they are renamed).
//there are a LOT of unexplained numbers in matlab where it's not clear if they have to be decremented to fit starting at 0...
namespace hop {

  MultilevelCoordinateSearch::MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int& populationSize, arma::Mat<double> boundaries, unsigned int boxDivisions, arma::Mat<double> hess, arma::Col<arma::uword> initialPointIndex, unsigned int maxLocalSearchSteps, double localStopThreshold)
  : PopulationBasedAlgorithm<double>(optimisationProblem, populationSize), boxDivisions_(boxDivisions), boundaries_(boundaries),
  maxLocalSearchSteps_(maxLocalSearchSteps), localStopThreshold_(localStopThreshold), hess_(hess), initialPointIndex_(initialPointIndex) {

    //assigning standard values for variables. Can't do in header-file since dependend on input variable "boundaries"
    hess_ = arma::ones(boundaries.col(0).n_elem);
    boxDivisions_ = 50 * boundaries.col(0).n_elem + 10;

    //for convenience
    unsigned int numberOfDimensions = optimisationProblem_->getNumberOfDimensions();

    //length of u or v and thus length of hess should equal numberOfDimensions
    if (numberOfDimensions != boundaries.col(0).n_elem) {
      std::cout << "lower boundaries dimensions don't match!" << std::endl;
    }
    if (numberOfDimensions != boundaries.col(1).n_elem) {
      std::cout << "upper boundaries dimensions don't match!" << std::endl;
    }
    if (numberOfDimensions != hess.row(0).n_elem && optimisationProblem_->getNumberOfDimensions() != hess.col(0).n_elem) {
      std::cout << "hess dimensions don't match!" << std::endl;
    }
    //Check if bounds are malformed
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      if (boundaries(i, 0) >= boundaries(i, 1)) {
        std::cout << "boundary malformed! u: " << boundaries(i, 0) << ", v:" << boundaries(i, 1) << std::endl;
      }
    }

    //init of large arrays
    //TODO: type completely unclear right now, pdf page 6f
    isplit_ = arma::Col<double>(step1_, arma::fill::zeros);
    level_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);
    ipar_ = arma::Col<double>(step1_, arma::fill::zeros);
    ichild_ = arma::Col<double>(step1_, arma::fill::zeros);
    f_ = arma::Col<double>(step1_, arma::fill::zeros);
    z_ = arma::Col<double>(step1_, arma::fill::zeros);
    nogain_ = arma::Col<double>(step1_, arma::fill::zeros);

    //initialization list
    //this is the equivalent of iinit = 0 in matlab
    //TODO: this is basically initialpopulation
    x0_.col(0) = boundaries_.col(0);
    x0_.col(1) = (boundaries_.col(0) + boundaries_.col(1)) / 2.0;
    x0_.col(2) = boundaries_.col(1);
    ////    initialPointIndex_(numberOfDimensions, 2);

    //TODO: for custom initialisation lists there is a check here to see if they violate the boundaries


    //l_ L and x0_ are the custom initialisation list variables
    //l_ is supposed to point to the initial point x^0 in x0_ 
    //l_ also never gets changed in matlab as far as i could see
    //L gives the amount of predefined values per dimension (basically populationSize_ with more finetuning possible)
    //TODO: mcs.m does infinity check on x0_ here, not sure if needed

    arma::Col<double> initialPoint(numberOfDimensions, arma::fill::zeros);
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      initialPoint(i) = x0_(i, initialPointIndex_(i));
    }

    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(initialPoint);
    bestParameter_ = initialPoint;
    initListEvaluations_(0, initialPointIndex_(0)) = bestObjectiveValue_;

    for (std::size_t r = 0; r < numberOfDimensions; r++) {
      bestPointIndex_(r) = initialPointIndex_(r);
      for (std::size_t c = 0; c < populationSize_; c++) {
        if (c == initialPointIndex_(r)) {
          if (r != 1) {
            initListEvaluations_(r, c) = initListEvaluations_(r - 1, bestPointIndex_(r - 1));
          }
        } else {
          initialPoint(r) = x0_(r, c);
          initListEvaluations_(r, c) = optimisationProblem_->getObjectiveValue(initialPoint);
          numberOfIterations_++;
          if (initListEvaluations_(r, c) < bestObjectiveValue_) {
            bestObjectiveValue_ = initListEvaluations_(r, c);
            bestParameter_ = initialPoint;
            bestPointIndex_(r) = c;
          }
        }
      }
      initialPoint(r) = x0_(r, bestPointIndex_(r));
    }
    //in init.m all operations are done transposed (reason unknown yet), we simply do it at the end
    //TODO: About halfway through the code this still seems to make no sense
    initListEvaluations_ = initListEvaluations_.t();

    //base vertex and opposite vertex init
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      baseVertex_(i) = x0_(i, initialPointIndex_(i));

      //if true, use u, else use v
      if (std::abs(baseVertex_(i) - boundaries.col(0)(i)) > std::abs(baseVertex_(i) - boundaries.col(1)(i))) {
        oppositeVertex_(i) = boundaries.col(0)(i);
      } else {
        oppositeVertex_(i) = boundaries.col(1)(i);
      }
    }

    //init of record list, nboxes, nbasket,nbasket0,nsweep, m, nloc, xloc
    //"flag" is not needed since it is equal to optProblem->isFinished()
    //values not listed here are defined in header
    record_ = arma::Col<double>(boxDivisions_ - 1, arma::fill::zeros);
    m_ = numberOfDimensions;
    record_(0) = 1;
    xloc_ = arma::Mat<double>(maxLocalSearchSteps_, step1_);

    //generate boxes
    initBoxes();
  }

  void MultilevelCoordinateSearch::optimiseImplementation() {

  }

  void MultilevelCoordinateSearch::initBoxes() {
    //for convenience
    unsigned int numberOfDimensions = optimisationProblem_->getNumberOfDimensions();

    //init variables that are dynamic in matlab
    boxBaseVertexFunctionValues_ = arma::Mat<double> (2, step1_);

    //parameter values of box 1
    ipar_(0) = 0;
    level_(0) = 1;
    ichild_(0) = 1;
    boxBaseVertexFunctionValues_(0, 0) = initListEvaluations_(initialPointIndex_(0), 0);

    int par = 0;

    arma::Col<double> var = arma::Col<double>(step1_);

    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      isplit_(par) = -i; //boxes split in the init. procedure get a negative splitting index
      int nchild = 0;
      if (x0_(i, 1) > boundaries_.col(0)(i)) {
        nboxes_++;
        nchild++;
        genBox(nboxes_, par, level_(par) + 1, -nchild, initListEvaluations_(0, i));
      }
      //TODO: in matlab this is L(i) instead of populationSize,
      //seems kind of pointless since we don't have dynamic popSize per dimension so this always or never true
      //TODO: also, this might need to be 2 instead of 3
      double v1 = 0;
      if (populationSize_ == 3) {
        v1 = boundaries_.col(1)(i);
      } else {
        v1 = x0_(i, 2);
      }
      arma::Col<double> d = quadraticPolynomialInterpolation(x0_.submat(i, 0, i, 2), initListEvaluations_.submat(0, i, 2, i));
      double xl = minimumQuadraticPolynomial(boundaries_.col(0)(i), v1, d, x0_.submat(i, 0, i, 2));
      double fl = quadraticPolynomial(xl, d, x0_.submat(i, 0, i, 2));
      double xu = minimumQuadraticPolynomial(boundaries_.col(0)(i), v1, -d, x0_.submat(i, 0, i, 2));
      double fu = quadraticPolynomial(xu, d, x0_.submat(i, 0, i, 2));

      int par1 = 0;
      int j1 = 0; //auxiliary index
      if (bestPointIndex_(i) == 0) {
        if (xl < x0_(i, 0)) {
          par1 = nboxes_; //label of the current box for the next coordinate
        } else {
          par1 = nboxes_ + 1;
          j1 = 1;
        }
      }
      for (std::size_t j = 0; j < populationSize_ - 1; j++) {
        nboxes_++;
        nchild++;
        int s = 0;
        if (initListEvaluations_(j, i) <= initListEvaluations_(j + 1, i)) {
          s = 1;
        } else {
          s = 2;
        }
        genBox(nboxes_, par, level_(par) + s, -nchild, initListEvaluations_(j, i));
        if (j >= 1) {
          if (bestPointIndex_(i) == j) {
            if (xl <= x0_(i, j)) {
              par1 = nboxes_ - 1;
              j1 = j - 1;
            } else {
              par1 = nboxes_;
              j1 = j + 1;
            }
          }
          if (j <= populationSize_ - 2) {
            d = quadraticPolynomialInterpolation(x0_.submat(i, j, i, j + 2), initListEvaluations_.submat(j, i, j + 2, i));
            double u1 = 0;
            if (j < populationSize_ - 2) {
              u1 = x0_(i, j + 2);
            } else {
              u1 = boundaries_.col(1)(i);
            }
            xl = minimumQuadraticPolynomial(x0_(i, j), u1, d, x0_.submat(i, j, i, j + 2));
            fl = std::min(quadraticPolynomial(xl, d, x0_.submat(i, j, i, j + 2)), fl);
            xu = minimumQuadraticPolynomial(x0_(i, j), u1, -d, x0_.submat(i, j, i, j + 2));
            fu = std::max(quadraticPolynomial(xu, d, x0_.submat(i, j, i, j + 2)), fu);
          }
        }
        nboxes_++;
        nchild++;
        genBox(nboxes_, par, level_(par) + 3 - s, -nchild, initListEvaluations_(j + 1, i));
      }
      if (x0_(i, populationSize_) < boundaries_.col(0)(i)) {
        nboxes_++;
        nchild++;
        genBox(nboxes_, par, level_(par) + 1, -nchild, initListEvaluations_(populationSize_, i));
      }
      if (bestPointIndex_(i) == populationSize_) {
        if (x0_(i, populationSize_) < boundaries_.col(0)(i)) {
          if (xl <= x0_(i, populationSize_)) {
            par1 = nboxes_ - 1;
            j1 = populationSize_ - 1;
          } else {
            par1 = nboxes_;
            j1 = populationSize_ + 1;
          }
        } else {
          par1 = nboxes_;
          j1 = populationSize_ - 1;
        }
      }
      var(i) = fu - fl; // the quadratic model is taken as a crude measure of the variability in the ith component
      level_(par) = 0; //box is marked as split
      par = par1;
      //TODO: no idea what this splval in this next section is for. it is never used...
      double splval = 0;
      if (j1 == 0) {
        splval = boundaries_.col(0)(i);
      } else if(j1 == populationSize_ +1) {
        splval = boundaries_.col(1)(i);
      } else {
        if( j1<bestPointIndex_(i)) {
          splval = splitGoldenSectionRule(x0_(i,j1),x0_(i,bestPointIndex_(i)),initListEvaluations_(j1,i),initListEvaluations_(bestPointIndex_(i),i));
        } else {
          splval = splitGoldenSectionRule(x0_(i,bestPointIndex_(i)),x0_(i,j1),initListEvaluations_(bestPointIndex_(i),i),initListEvaluations_(j1,i));
        }
      }
    }
    //from matlab: best function value after the init. procedure
    //doesnt make much sense to me since we never changed initListEvaluations
    bestObjectiveValue_ = initListEvaluations_(bestPointIndex_(numberOfDimensions),numberOfDimensions);
    double var0 = 0;
    for(std::size_t i = 0; i < numberOfDimensions; i++) {
      //TODO: next two lines should equal [var0,p(i)] = max(var); not sure if correct
      var0 = arma::max(var);
      variabilityRanking_(i) = var0;
      var(var0) = -1;
      bestParameter_(i) = x0_(i,bestPointIndex_(i)); //from matlab: best point after the init. procedure
    }
  }

  std::string MultilevelCoordinateSearch::to_string() const noexcept {
    return "MultilevelCoordinateSearch";
  }

  void MultilevelCoordinateSearch::genBox(int nbox, int par, arma::uword level, int nchild, double baseVertexFunctionValue) {
    ipar_(nbox) = par;
    level_(nbox) = level;
    ichild_(nbox) = nchild;
    boxBaseVertexFunctionValues_(nbox) = baseVertexFunctionValue;
  }

  arma::Col<double> MultilevelCoordinateSearch::quadraticPolynomialInterpolation(arma::Col<double> supportPoints, arma::Col<double> functionValues) {
    arma::Col<double> d(3);
    d(0) = functionValues(0);
    d(1) = (functionValues(1) - functionValues(0)) / (supportPoints(1) - supportPoints(0));
    double f23 = (functionValues(2) - functionValues(1)) / (supportPoints(2) - supportPoints(1));
    d(2) = (f23 - d(1)) / (supportPoints(2) - supportPoints(0));
    return d;
  }

  double MultilevelCoordinateSearch::minimumQuadraticPolynomial(double a, double b, arma::Col<double> d, arma::Mat<double> x0_fragment) {
    double x = 0;
    if (d(2) == 0) {
      if (d(1) == 0) {
        x = a;
      } else {
        x = b;
      }
      return x;
    } else if (d(2) > 0) {
      double x1 = 0.5 * (x0_fragment(0)) + x0_fragment(1) - 0.5 * (d(1) / d(2));
      if (a <= x1 && x1 <= b) {
        x = x1;
        return x;
      }
    }
    if (quadraticPolynomial(a, d, x0_fragment) < quadraticPolynomial(b, d, x0_fragment)) {
      x = a;
    } else {
      x = b;
    }
    return x;
  }

  double MultilevelCoordinateSearch::quadraticPolynomial(double x, arma::Col<double> d, arma::Mat<double> x0_fragment) {
    return d(0) + d(1)*(x - x0_fragment(0)) + d(2)*(x - x0_fragment(0))*(x - x0_fragment(1));
  }

  double MultilevelCoordinateSearch::splitGoldenSectionRule(double x1, double x2, double f1, double f2) {
    if (f1 <= f2) {
      return x1 + 0.5 * (-1 + std::sqrt(5))*(x2 - x1);
    } else {
      return x1 + 0.5 * (3 - std::sqrt(5))*(x2 - x1);
    }
  }
}