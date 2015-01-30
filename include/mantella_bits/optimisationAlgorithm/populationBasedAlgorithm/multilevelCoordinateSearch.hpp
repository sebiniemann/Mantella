namespace mant {

  template<class DistanceFunction>
  class MultilevelCoordinateSearch : public PopulationBasedAlgorithm<double, DistanceFunction> {
  public:
    //lower boundaries are expected in the first col of "boundaries", upper boundaries in the second col of "boundaries.
    //initialPointIndex is the index inside initialPopulation_ which is used as the starting point.
    explicit MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem, const unsigned int& populationSize, arma::Col<arma::uword> initialPointIndex, unsigned int boxDivisions = 0, unsigned int maxLocalSearchSteps = 50) noexcept;
    MultilevelCoordinateSearch(const MultilevelCoordinateSearch&) = delete;
    MultilevelCoordinateSearch& operator=(const MultilevelCoordinateSearch&) = delete;

    void setLocalSearch(const std::shared_ptr<OptimisationAlgorithm<double, DistanceFunction>> localSearch) noexcept;

    std::string to_string() const noexcept override;
  protected:
    void optimiseImplementation() override;

    std::shared_ptr<OptimisationAlgorithm<double, DistanceFunction>> localSearch_;

    unsigned int step1_ = 10000;
    unsigned int step = 1000; //TODO: could be moved to constructor as a definable variable
    unsigned int dim = 10000;

    unsigned int boxDivisions_; //smax
    arma::Mat<double> boundaries_; //u,v - with u=col(0) and v=col(1)
    unsigned int maxLocalSearchSteps_; //local

    arma::Col<arma::sword> isplit_;
    arma::Col<arma::uword> level_;
    arma::Col<arma::uword> ipar_;
    arma::Col<arma::uword> ichild_;
    arma::Mat<double> boxBaseVertexFunctionValues_; //f
    arma::Mat<double> z_;
    arma::Col<arma::uword> nogain_;

    arma::Col<arma::uword> initialPointIndex_; //l
    arma::Mat<double> x0_;
    arma::Mat<double> initListValues_; //f0
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

    void initBoxes(); //initbox.m
    arma::Col<arma::uword> variabilityRanking_; //p

    arma::Mat<double> pointsInBasket_; //xmin
    arma::Col<double> pointsInBasketValue_; //fmi

    //helper methods
    void genBox(int nbox, int par, int level, int nchild, double baseVertexFunctionValue); //genbox.m

    arma::Col<double> quadraticPolynomialInterpolation(arma::Col<double> supportPoints, arma::Col<double> functionValues); //polint.m

    double minimumQuadraticPolynomial(double a, double b, arma::Col<double> d, arma::Mat<double> x0_fragment); //quadmin.m

    double quadraticPolynomial(double x, arma::Col<double> d, arma::Mat<double> x0_fragment); //quadpol.m

    //split1.m 
    //splits the interval [x1,x2] according to the golden section rule
    double splitByGoldenSectionRule(double x1, double x2, double f1, double f2);

    //splrnk.m 
    //determines the splitting index and splitting value for splitting a box by rank
    void splitByRank(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0);

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

    //vertex.m 
    void vertex(unsigned int par, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2, arma::Col<arma::uword> n0);

    void vert1(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2); //vert1.m

    void vert2(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2); //vert2.m

    void vert3(int updateIndex, unsigned int j, arma::Col<double> f0col, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2); //vert3.m

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
    bool checkLocationNotUsed(arma::Col<double> location);

    //addloc.m 
    //adds a new point to the list of starting points for local search
    void addLocation(arma::Col<double> loc);

    //basket.m 
    //checks whether a candidate for local search lies in the 'domain of 
    //attraction' of a point in the 'shopping basket'
    bool pointInsideDomainOfAttraction(arma::Col<double> loc, std::shared_ptr<double> valueAtLoc, double nbasket);

    //basket1.m 
    //checks whether a candidate for local search lies in the 'domain of 
    //attraction' of a point in the 'shopping basket'
    bool candidateInsideDomainOfAttraction(arma::Col<double> candidate, double valueAtCandidate, double nbasket);
  };

  //
  // Implementation
  //
  // <editor-fold>

  template<class DistanceFunction>
  MultilevelCoordinateSearch<DistanceFunction>::MultilevelCoordinateSearch(const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int& populationSize, arma::Col<arma::uword> initialPointIndex, unsigned int boxDivisions, unsigned int maxLocalSearchSteps) noexcept
  : PopulationBasedAlgorithm<double, DistanceFunction>(optimisationProblem, populationSize), boxDivisions_(boxDivisions),
  maxLocalSearchSteps_(maxLocalSearchSteps), initialPointIndex_(initialPointIndex) {
    std::cout << "starting constructor" << std::endl;
    //for convenience
    unsigned int numberOfDimensions = optimisationProblem->getNumberOfDimensions();

    std::shared_ptr<OptimisationAlgorithm<double, DistanceFunction >> test(new HillClimbing<double, DistanceFunction>(optimisationProblem));

    setLocalSearch(test);

    std::cout << "assigning boundary" << std::endl;

    boundaries_ = arma::Mat<double>(numberOfDimensions, 2);
    boundaries_.col(0) = optimisationProblem->getLowerBounds();
    boundaries_.col(1) = optimisationProblem->getUpperBounds();

    //assigning standard values for variables. Can't do in header-file since dependent on input variable "boundaries"
    if (boxDivisions_ == 0) {
      boxDivisions_ = 50 * numberOfDimensions + 10;
      std::cout << "boxDivisions_ = " << boxDivisions_ << std::endl;
    }

    std::cout << "initting large arrays" << std::endl;
    //init of large arrays
    isplit_ = arma::Col<arma::sword>(step1_, arma::fill::zeros);
    level_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);
    ipar_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);
    ichild_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);
    boxBaseVertexFunctionValues_ = arma::Mat<double>(2, step1_, arma::fill::zeros);
    z_ = arma::Mat<double>(2, step1_, arma::fill::zeros);
    nogain_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);

    //initialization list
    x0_ = arma::Mat<double>(numberOfDimensions, 3);
    x0_.col(0) = boundaries_.col(0);
    x0_.col(1) = (boundaries_.col(0) + boundaries_.col(1)) / 2.0;
    x0_.col(2) = boundaries_.col(1);
    initListValues_ = arma::Mat<double>(this->populationSize_, step1_); //need to do large allocation cause matlab code expands automatically later
    bestPointIndex_ = arma::Col<arma::uword>(numberOfDimensions);
    variabilityRanking_ = arma::Col<arma::uword>(numberOfDimensions);

    //l_ L and x0_ are the custom initialisation list variables
    //l_ is supposed to point to the initial point x^0 in x0_ 
    //l_ also never gets changed in matlab as far as i could see
    //L gives the amount of predefined values per dimension (basically this->populationSize_ with more finetuning possible)

    arma::Col<double> initialPoint(numberOfDimensions, arma::fill::zeros);
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      initialPoint(i) = x0_(i, initialPointIndex_(i));
    }

    this->bestObjectiveValue_ = this->optimisationProblem_->getObjectiveValue(initialPoint);
    this->bestParameter_ = initialPoint;
    initListValues_(0, initialPointIndex_(0)) = this->bestObjectiveValue_;

    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      bestPointIndex_(i) = initialPointIndex(i);
      for (std::size_t j = 0; j < this->populationSize_; j++) {
        if (j == initialPointIndex_(i)) {
          if (i != 0) {
            initListValues_(j, i) = initListValues_(bestPointIndex_(i - 1), i - 1);
          }
        } else {
          initialPoint(i) = x0_(i, j);
          initListValues_(j, i) = this->optimisationProblem_->getObjectiveValue(initialPoint);
          this->numberOfIterations_++;
          if (initListValues_(j, i) < this->bestObjectiveValue_) {
            this->bestObjectiveValue_ = initListValues_(j, i);
            this->bestParameter_ = initialPoint;
            bestPointIndex_(i) = j;
          }
        }
      }
      initialPoint(i) = x0_(i, bestPointIndex_(i));
    }

    //base vertex and opposite vertex init
    baseVertex_ = arma::Col<double>(numberOfDimensions);
    oppositeVertex_ = arma::Col<double>(numberOfDimensions);
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      baseVertex_(i) = x0_(i, initialPointIndex_(i));

      //if true, use u, else use v
      if (std::abs(baseVertex_(i) - boundaries_.col(0)(i)) > std::abs(baseVertex_(i) - boundaries_.col(1)(i))) {
        oppositeVertex_(i) = boundaries_.col(0)(i);
      } else {
        oppositeVertex_(i) = boundaries_.col(1)(i);
      }
    }

    //init of record list, nboxes, nbasket,nbasket0, m, nloc, xloc
    //values not listed here are defined in header
    record_ = arma::Col<arma::uword>(boxDivisions_ - 1, arma::fill::zeros);
    m_ = numberOfDimensions;
    record_(0) = 1;
    xloc_ = arma::Mat<double>(numberOfDimensions, step1_);

    //generate boxes
    std::cout << "initting boxes" << std::endl;
    initBoxes();
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::optimiseImplementation() {
    //for convenience
    unsigned int numberOfDimensions = this->optimisationProblem_->getNumberOfDimensions();

    //TODO: find better value than step1_...
    pointsInBasket_ = arma::Mat<double>(numberOfDimensions, step1_); //xmin
    pointsInBasketValue_ = arma::Col<double>(step1_); //fmi

    // s- the vector record is updated, and the minimal level s containing non-split boxes is computed
    unsigned int minimalLevel = startSweep();

    //TODO: mcs.m checks for "minimalLevel < boxDivisions" as a while condition.
    //Which makes sense, since we cannot calculate anything if we cannot divide further.
    std::cout << "start of while, running until " << boxDivisions_ << std::endl;
    int rofl = 0;
    //while (!this->isFinished() && !this->isTerminated() && minimalLevel < boxDivisions_) {
    while (!this->isFinished() && !this->isTerminated() && rofl < 10) {
      rofl++;
      std::cout << "minimallevel " << minimalLevel << std::endl;
      unsigned int par = record_(minimalLevel - 1); //the best box at level s is the current box

      arma::Col<double> x1 = arma::Col<double>(numberOfDimensions);
      x1.fill(arma::datum::inf);
      arma::Col<double> x2 = arma::Col<double>(numberOfDimensions);
      x2.fill(arma::datum::inf);
      arma::Col<double> f1 = arma::Col<double>(numberOfDimensions, arma::fill::zeros);
      arma::Col<double> f2 = arma::Col<double>(numberOfDimensions, arma::fill::zeros);
      arma::Col<arma::uword> n0 = arma::Col<arma::uword>(numberOfDimensions, arma::fill::zeros);
      std::cout << "before vertex" << std::endl;
      vertex(par, x1, x2, f1, f2, n0);
      std::cout << "after vertex" << std::endl;

      bool doSplit = false; //splt
      if (minimalLevel > 2 * numberOfDimensions * (arma::min(n0) + 1)) {
        splitByRank(par, numberOfDimensions, n0);
        doSplit = true;
      } else {
        //TODO: this if should be unnecessary in c++. else be !if
        if (nogain_(par)) {
          doSplit = false;
        } else {
          arma::Col<double> expectedGain = expectedGainOfSplit(par, numberOfDimensions, n0, x1, x2, f1, f2); //e
          //index again so use 0, matlab=1
          double fexp = boxBaseVertexFunctionValues_(0, par) + arma::min(expectedGain);
          if (fexp < this->bestObjectiveValue_) {
            doSplit = true;
          } else {
            doSplit = false;
            nogain_(par) = 1;
          }
        }
      }
      std::cout << "split is " << doSplit << std::endl;
      if (doSplit) {
        int i = isplit_(par);
        level_(par) = 0;
        //index again so use 1, matlab=2
        if (z_(1, par) == arma::datum::inf) {
          m_++;
          //index again so use 1, matlab=2
          z_(1, par) = m_;
          //little different than matlab, if this returns true = isFinished() | isTerminated()
          std::cout << "before splitbyinitlist" << std::endl;
          if (splitByInitList(i, minimalLevel, par, n0, x1, x2, pointsInBasket_, pointsInBasketValue_)) {
            break; //should break out of major while loop
          }
          std::cout << "after splitbyinitlist" << std::endl;
        } else {
          //index again so use 0, matlab=1
          z_(0, par) = baseVertex_(i);
          //little different than matlab, if this returns true = isFinished() | isTerminated()
          std::cout << "before split" << std::endl;
          if (split(i, minimalLevel, par, n0, x1, x2, pointsInBasket_, pointsInBasketValue_)) {
            break;
          }
          std::cout << "after split" << std::endl;
        }
        std::cout << "making arrays larger " << (nboxes_ > dim) << std::endl;
        //if the pre-assigned size of the `large' arrays has already been exceeded, these arrays are made larger
        //first if is custom to accomodate the automatically enlarge by assign
        //in matlab at the start of the while-loop
        if ((m_ + 1) == initListValues_.n_cols) {
          initListValues_.resize(initListValues_.n_rows, initListValues_.n_cols + step);
        }
        if (nboxes_ > dim) {
          //TODO: are the additional elements automatically set to zero? if not, need to do that
          isplit_.resize(nboxes_ + step);
          level_.resize(nboxes_ + step);
          ipar_.resize(nboxes_ + step);
          ichild_.resize(nboxes_ + step);
          z_.resize(z_.n_rows, nboxes_ + step);
          nogain_.resize(nboxes_ + step);
          boxBaseVertexFunctionValues_.resize(z_.n_rows, nboxes_ + step);
          dim = nboxes_ + step;
        }
        std::cout << "done making arrays larger" << std::endl;
        if (this->isFinished() || this->isTerminated()) {
          break;
        }
      } else {//no splitting, increase the level by 1
        std::cout << "no splitting" << std::endl;
        if (minimalLevel + 1 < boxDivisions_) {
          level_(par) = minimalLevel + 1;
          //index again so use 0, matlab=1
          std::cout << "updating record" << std::endl;
          updateRecord(par, minimalLevel + 1, boxBaseVertexFunctionValues_.row(0).t());
        } else {
          level_(par) = 0;
          nbasket_++;
          pointsInBasket_.col(nbasket_) = baseVertex_;
          //index again so use 0, matlab=1
          pointsInBasketValue_(nbasket_) = boxBaseVertexFunctionValues_(0, par);
        }
      } //end of prepare for splitting
      std::cout << "splitting logic finished" << std::endl;
      minimalLevel++;
      while (minimalLevel < boxDivisions_ - 1) {
        if (record_(minimalLevel) == 0) {
          minimalLevel++;
        } else {
          break;
        }
      }
      //if smax is reached, a new sweep is started 
      std::cout << "smax reached " << (minimalLevel == boxDivisions_) << std::endl;
      if (minimalLevel == boxDivisions_) {
        if (maxLocalSearchSteps_ > 0) {
          //original matlab sort: [fmi(nbasket0+1:nbasket),j] = sort(fmi(nbasket0+1:nbasket));
          //the j is never used, so we don't retrieve the sort-index
          pointsInBasketValue_.rows(nbasket0_ + 1, nbasket_) = arma::sort(pointsInBasketValue_.rows(nbasket0_ + 1, nbasket_));

          for (int j = nbasket0_ + 1; j < nbasket_; j++) {
            //programmatically it seems dumb to overwrite a global variable, but they do it in matlab.
            baseVertex_ = pointsInBasket_.col(j);

            if (checkLocationNotUsed(baseVertex_)) {
              //unfortunately multiple return values of primitives are needed later, so we have to use pointers
              //TODO: Something is strange/wrong here.
              //f1 is already a vector earlier. Why is he overwriting it in matlab??
              std::shared_ptr<double> f1(new double(pointsInBasketValue_(j)));

              addLocation(baseVertex_);

              bool isInside = pointInsideDomainOfAttraction(baseVertex_, f1, nbasket0_);
              if (this->isFinished() || this->isTerminated()) {
                return;
              }
              if (isInside) {
                ///////////////////////////TODO: TEMPORARY WORKAROUND FOR LSEARCH///////////////////////////
                arma::Col<double> xminTemp(numberOfDimensions, arma::fill::zeros);
                double fmiTemp = 10;

                if (fmiTemp < this->bestObjectiveValue_) {
                  this->bestObjectiveValue_ = fmiTemp;
                  this->bestParameter_ = xminTemp;
                  if (this->isFinished() || this->isTerminated()) {
                    nbasket0_++;
                    nbasket_ = nbasket0_;
                    pointsInBasket_.col(nbasket_) = xminTemp;
                    pointsInBasketValue_(nbasket_) = fmiTemp;
                    return;
                  }
                }
                bool candidateInside = candidateInsideDomainOfAttraction(xminTemp, fmiTemp, nbasket0_);
                if (candidateInside) {
                  nbasket0_++;
                  pointsInBasket_.col(nbasket0_) = xminTemp;
                  pointsInBasketValue_(nbasket0_) = fmiTemp;
                  //TODO: this check seems redundant, should check if it ever gets called
                  if (pointsInBasketValue_(nbasket0_) < this->bestObjectiveValue_) {
                    std::cout << "lol if you see this" << std::endl;
                    this->bestObjectiveValue_ = pointsInBasketValue_(nbasket0_);
                    this->bestParameter_ = pointsInBasket_.col(nbasket0_);
                    if (this->isFinished() || this->isTerminated()) {
                      nbasket_ = nbasket0_;
                      return;
                    }
                  }
                }
              }
            }
          }
          nbasket_ = nbasket0_;
          if (this->isFinished() || this->isTerminated()) {
            return;
          }
        }
        minimalLevel = startSweep();
      }
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::initBoxes() {
    //for convenience
    unsigned int numberOfDimensions = this->optimisationProblem_->getNumberOfDimensions();

    //parameter values of box 1
    ipar_(0) = 0;
    level_(0) = 1;
    ichild_(0) = 1;
    boxBaseVertexFunctionValues_(0, 0) = initListValues_(initialPointIndex_(0), 0);

    int parent = 0;

    arma::Col<double> var = arma::Col<double>(numberOfDimensions);

    for (arma::sword i = 0; i < numberOfDimensions; i++) {
      std::cout << "\n" << "i = " << i << std::endl;
      isplit_(parent) = -i; //boxes split in the init. procedure get a negative splitting index
      std::cout << "isplit_(" << parent << ") = " << -i << std::endl;
      int nchild = 0;
      if (x0_(i, 0) > boundaries_.col(0)(i)) {
        nboxes_++;
        nchild++;
        genBox(nboxes_ - 1, parent, level_(parent) + 1, -nchild, initListValues_(0, i));
      }

      //in matlab there is more logic applied to v1, but since popSize is fixed for all dimensions, it doesn't matter
      double v1 = x0_(i, 2);
      std::cout << "v1 = " << v1 << std::endl;

      arma::Col<double> d = quadraticPolynomialInterpolation(x0_.row(i).subvec(0, 2).t(), initListValues_.col(i).subvec(0, 2));
      double xl = minimumQuadraticPolynomial(boundaries_.col(0)(i), v1, d, x0_.submat(i, 0, i, 2));
      double fl = quadraticPolynomial(xl, d, x0_.submat(i, 0, i, 2));
      double xu = minimumQuadraticPolynomial(boundaries_.col(0)(i), v1, -d, x0_.submat(i, 0, i, 2));
      double fu = quadraticPolynomial(xu, d, x0_.submat(i, 0, i, 2));

      int par1 = 0; //label of the current box for the next coordinate
      if (bestPointIndex_(i) == 0) {
        if (xl < x0_(i, 0)) {
          par1 = nboxes_;
        } else {
          par1 = nboxes_ + 1;
        }
      }
      for (std::size_t j = 0; j < this->populationSize_ - 1; j++) {
        nboxes_++;
        nchild++;
        int s = 0;
        if (initListValues_(j, i) <= initListValues_(j + 1, i)) {
          s = 1;
        } else {
          s = 2;
        }
        genBox(nboxes_ - 1, parent, level_(parent) + s, -nchild, initListValues_(j, i));
        if (j >= 1) {
          if (bestPointIndex_(i) == j) {
            if (xl <= x0_(i, j)) {
              par1 = nboxes_ - 1;
            } else {
              par1 = nboxes_;
            }
          }
          if (j <= this->populationSize_ - 3) {

            d = quadraticPolynomialInterpolation(x0_.submat(i, j, i, j + 2), initListValues_.submat(j, i, j + 2, i));
            double u1 = 0;
            if (j < this->populationSize_ - 3) {
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
        genBox(nboxes_ - 1, parent, level_(parent) + 3 - s, -nchild, initListValues_(j + 1, i));
      }
      if (x0_(i, this->populationSize_ - 1) < boundaries_.col(0)(i)) {
        nboxes_++;
        nchild++;
        genBox(nboxes_ - 1, parent, level_(parent) + 1, -nchild, initListValues_(this->populationSize_ - 1, i));
      }
      if (bestPointIndex_(i) == this->populationSize_ - 1) {

        if (x0_(i, this->populationSize_ - 1) < boundaries_.col(0)(i)) {
          if (xl <= x0_(i, this->populationSize_ - 1)) {
            par1 = nboxes_ - 1;
          } else {
            par1 = nboxes_;
          }
        } else {
          par1 = nboxes_;
        }
      }
      var(i) = fu - fl; // the quadratic model is taken as a crude measure of the variability in the ith component
      level_(parent) = 0; //box is marked as split
      parent = par1;
    }
    //from matlab: best function value after the init. procedure
    //doesnt make much sense to me since we never changed initListEvaluations
    this->bestObjectiveValue_ = initListValues_(bestPointIndex_(numberOfDimensions - 1), numberOfDimensions - 1);
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      //TODO: next two lines should equal [var0,p(i)] = max(var); not sure if correct
      var.max(variabilityRanking_(i));
      var(variabilityRanking_(i)) = -1;
      this->bestParameter_(i) = x0_(i, bestPointIndex_(i)); //from matlab: best point after the init. procedure
    }
    std::cout << "initbox done" << std::endl;
  }

  template<class DistanceFunction>
  std::string MultilevelCoordinateSearch<DistanceFunction>::to_string() const noexcept {
    return "MultilevelCoordinateSearch";
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::genBox(int nbox, int par, int level, int nchild, double baseVertexFunctionValue) {
    std::cout << "inside genbox, nbox = " << nbox << std::endl;
    ipar_(nbox) = par;
    std::cout << "ipar_(" << nbox << ") = " << par << std::endl;
    level_(nbox) = level;
    std::cout << "level_(" << nbox << ") = " << level << std::endl;
    //TODO: nchild probably needs to be decremented by 1, or all calls to genBox's nchild
    ichild_(nbox) = nchild;
    std::cout << "ichild_(" << nbox << ") = " << nchild << std::endl;
    boxBaseVertexFunctionValues_(0, nbox) = baseVertexFunctionValue;
    std::cout << "bbvf_(0, " << nbox << ") = " << baseVertexFunctionValue << std::endl;
    std::cout << "genbox done" << std::endl;
  }

  template<class DistanceFunction>
  arma::Col<double> MultilevelCoordinateSearch<DistanceFunction>::quadraticPolynomialInterpolation(arma::Col<double> supportPoints, arma::Col<double> functionValues) {
    std::cout << "qpi: before init d" << std::endl;
    arma::Col<double> d(3);
    std::cout << "qpi: init d" << std::endl;
    d(0) = functionValues(0);
    d(1) = (functionValues(1) - functionValues(0)) / (supportPoints(1) - supportPoints(0));
    double f23 = (functionValues(2) - functionValues(1)) / (supportPoints(2) - supportPoints(1));
    d(2) = (f23 - d(1)) / (supportPoints(2) - supportPoints(0));
    std::cout << "qpi: return" << std::endl;
    return d;
  }

  template<class DistanceFunction>
  double MultilevelCoordinateSearch<DistanceFunction>::minimumQuadraticPolynomial(double a, double b, arma::Col<double> d, arma::Mat<double> x0_fragment) {
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

  template<class DistanceFunction>
  double MultilevelCoordinateSearch<DistanceFunction>::quadraticPolynomial(double x, arma::Col<double> d, arma::Mat<double> x0_fragment) {
    return d(0) + d(1)*(x - x0_fragment(0)) + d(2)*(x - x0_fragment(0))*(x - x0_fragment(1));
  }

  template<class DistanceFunction>
  double MultilevelCoordinateSearch<DistanceFunction>::splitByGoldenSectionRule(double x1, double x2, double f1, double f2) {
    if (f1 <= f2) {
      return x1 + 0.5 * (-1 + std::sqrt(5))*(x2 - x1);
    } else {
      return x1 + 0.5 * (3 - std::sqrt(5))*(x2 - x1);
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::splitByRank(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0) {
    //index again so use 0, matlab=1 for all 3 variables
    int isplit = 0;
    int n1 = n0(0);
    int p1 = variabilityRanking_(0);
    //matlab starts at 2 obviously
    for (int i = 1; i < numberOfDimensions; i++) {
      if (n0(i) < n1 || (n0(i) == n1 && variabilityRanking_(i) < p1)) {
        isplit = i;
        n1 = n0(i);
        p1 = variabilityRanking_(i);
      }
    }
    if (n1 > 0) {
      //index again so use 1, matlab=2
      z_(1, par) = splitBySubint(baseVertex_(isplit), oppositeVertex_(isplit));
    } else {
      z_(1, par) = arma::datum::inf;
    }
  }

  template<class DistanceFunction>
  double MultilevelCoordinateSearch<DistanceFunction>::splitBySubint(double x, double y) {
    double x2 = y;
    if (x == 0 && std::abs(y) > 1000) {
      x2 = std::copysign(1.0, y);
    } else if (x != 0 && std::abs(y) > 100 * std::abs(x)) {
      //TODO: c++ standardlibraries have no signum. wat. using copysign instead...
      //original matlab: x2 = 10.*sign(y)*abs(x);
      x2 = 10 * std::copysign(x, y);
    }
    return x + 2 * (x2 - x) / 3.0;
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::splitByInitList(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Mat<double> pointsInBasket, arma::Col<double> pointsInBasketValue) {
    initListValues_.col(m_ - 1).zeros();
    for (int j = 0; j < this->populationSize_; j++) {
      std::cout << "j = " << j << std::endl;
      if (j != initialPointIndex_(splittingIndex)) {
        //TODO: why are we writing into the baseVertex? oO
        baseVertex_(splittingIndex) = x0_(splittingIndex, j);
        initListValues_.col(m_ - 1)(j) = this->optimisationProblem_->getObjectiveValue(baseVertex_);
        this->numberOfIterations_++;
        std::cout << "first if" << std::endl;
        if (initListValues_.col(m_ - 1)(j) < this->bestObjectiveValue_) {
          this->bestObjectiveValue_ = initListValues_.col(m_ - 1)(j);
          this->bestParameter_ = baseVertex_;
        }
        //In matlab this if is a little different and inside the if directly before this.
        //Our stopping conditions are a little different so it's here.
        if (this->isFinished() || this->isTerminated()) {
          return true;
        }
      } else {
        //index again so use 0, matlab=1
        initListValues_.col(m_ - 1)(j) = boxBaseVertexFunctionValues_(0, par);
      }
    }
    std::cout << "first loop done" << std::endl;
    if (minimalLevel + 1 < boxDivisions_) {
      int nchild = 0;
      //index again so use 0, matlab=1
      if (boundaries_.col(0)(splittingIndex) < x0_(splittingIndex, 0)) {//in that case the box at the boundary gets level s + 1
        std::cout << "updating boundaries" << std::endl;
        nchild++;
        nboxes_++;
        //index again so use 0, matlab=1
        genBox(nboxes_ - 1, par, minimalLevel + 1, -nchild, initListValues_.col(m_ - 1)(0));
        //index again so use 0, matlab=1
        updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
      }
      for (int j = 0; j < this->populationSize_ - 1; j++) {
        std::cout << "j = " << j << std::endl;
        nchild++;
        int level0 = 0;
        if (initListValues_.col(m_ - 1)(j) <= initListValues_.col(m_ - 1)(j + 1) || minimalLevel + 2 < boxDivisions_) {
          std::cout << "first if" << std::endl;
          nboxes_++;
          if (initListValues_.col(m_ - 1)(j) <= initListValues_.col(m_ - 1)(j)) {
            level0 = minimalLevel + 1;
          } else {
            level0 = minimalLevel + 2;
          }
          std::cout << "first genbox" << std::endl;
          genBox(nboxes_ - 1, par, level0, -nchild, initListValues_.col(m_ - 1)(j));
          //index again so use 0, matlab=1
          std::cout << level_.n_elem << std::endl;
          std::cout << nboxes_ << std::endl;
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          baseVertex_(splittingIndex) = x0_(splittingIndex, j);
          nbasket_++;
          pointsInBasket.col(nbasket_) = baseVertex_;
          pointsInBasketValue_(nbasket_) = initListValues_.col(m_ - 1)(j);
        }
        nchild++;
        if (initListValues_.col(m_ - 1)(j + 1) < initListValues_.col(m_ - 1)(j) || minimalLevel + 2 < boxDivisions_) {
          std::cout << "second if" << std::endl;
          nboxes_++;
          if (initListValues_.col(m_ - 1)(j + 1) < initListValues_.col(m_ - 1)(j)) {
            level0 = minimalLevel + 1;
          } else {
            level0 = minimalLevel + 2;
          }
          std::cout << "second genbox" << std::endl;
          genBox(nboxes_ - 1, par, level0, -nchild, initListValues_.col(m_ - 1)(j + 1));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          baseVertex_(splittingIndex) = x0_(splittingIndex, j + 1);
          nbasket_++;
          pointsInBasket.col(nbasket_) = baseVertex_;
          pointsInBasketValue_(nbasket_) = initListValues_.col(m_ - 1)(j + 1);
        }
      }
      std::cout << "second loop done" << std::endl;
      if (x0_(splittingIndex, this->populationSize_ - 1) < boundaries_.col(1)(splittingIndex)) {//in that case the box at the boundary gets level s + 1
        std::cout << "updating boundaries" << std::endl;
        nchild++;
        nboxes_++;
        genBox(nboxes_ - 1, par, minimalLevel + 1, -nchild, initListValues_.col(m_ - 1)(this->populationSize_ - 1));
        //index again so use 0, matlab=1
        updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
      }
    } else {
      for (int j = 0; j < this->populationSize_; j++) {
        std::cout << "j = " << j << std::endl;
        baseVertex_(splittingIndex) = x0_(splittingIndex, j);
        nbasket_++;
        pointsInBasket.col(nbasket_) = baseVertex_;
        pointsInBasketValue_(nbasket_) = initListValues_.col(m_ - 1)(j);
      }
      std::cout << "third loop done" << std::endl;
    }
    std::cout << "splitbyinitlist done" << std::endl;
    return this->isFinished() || this->isTerminated();
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::split(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Mat<double> pointsInBasket, arma::Col<double> pointsInBasketValue) {
    //index again so use 1, matlab=2
    baseVertex_(splittingIndex) = z_(1, par);
    std::cout << "VERTEX : " << baseVertex_ << std::endl;
    //index again so use 1, matlab=2
    boxBaseVertexFunctionValues_(1, par) = this->optimisationProblem_->getObjectiveValue(baseVertex_);
    this->numberOfIterations_++;
    //index again so use 1, matlab=2
    if (boxBaseVertexFunctionValues_(1, par) < this->bestObjectiveValue_) {
      //index again so use 1, matlab=2
      this->bestObjectiveValue_ = boxBaseVertexFunctionValues_(1, par);
      this->bestParameter_ = baseVertex_;
      //Our stopping conditions are a little different
      if (this->isFinished() || this->isTerminated()) {
        return true;
      }
    }

    if (minimalLevel + 1 < boxDivisions_) {
      //index again, all decremented by 1
      if (boxBaseVertexFunctionValues_(0, par) < boxBaseVertexFunctionValues_(1, par)) {
        nboxes_++;
        //index again so use 0, matlab=1
        genBox(nboxes_ - 1, par, minimalLevel + 1, 1, boxBaseVertexFunctionValues_(0, par));
        //index again so use 0, matlab=1
        updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        if (minimalLevel + 2 < boxDivisions_) {
          nboxes_++;
          //index again so use 1, matlab=2
          genBox(nboxes_ - 1, par, minimalLevel + 2, 2, boxBaseVertexFunctionValues_(1, par));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          //index again so use 1, matlab=2
          baseVertex_(splittingIndex) = z_(1, par);
          nbasket_ = nbasket_ + 1;
          pointsInBasket.col(nbasket_) = baseVertex_;
          //index again so use 1, matlab=2
          pointsInBasketValue_(nbasket_) = boxBaseVertexFunctionValues_(1, par);
        }

      } else {
        if (minimalLevel + 2 < boxDivisions_) {
          nboxes_++;
          //index again so use 0, matlab=1
          genBox(nboxes_ - 1, par, minimalLevel + 2, 1, boxBaseVertexFunctionValues_(0, par));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          //index again so use 0, matlab=1
          baseVertex_(splittingIndex) = z_(0, par);
          nbasket_ = nbasket_ + 1;
          pointsInBasket.col(nbasket_) = baseVertex_;
          //index again so use 0, matlab=1
          pointsInBasketValue_(nbasket_) = boxBaseVertexFunctionValues_(0, par);
        }
        nboxes_++;
        //index again so use 1, matlab=2
        genBox(nboxes_ - 1, par, minimalLevel + 1, 2, boxBaseVertexFunctionValues_(1, par));
        //index again so use 0, matlab=1
        updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
      }

      // if the third box is larger than the smaller of the other two boxes,
      // it gets level s + 1; otherwise it gets level s + 2
      //index again so use 1, matlab=2
      if (z_(1, par) != oppositeVertex_(splittingIndex)) {
        //index again so use 1, matlab=2
        if (std::abs(z_(par, 1) - oppositeVertex_(splittingIndex)) > std::abs(z_(1, par))*(3 - std::sqrt(5)*0.5)) {
          nboxes_++;
          //index again so use 1, matlab=2
          genBox(nboxes_ - 1, par, minimalLevel + 1, 3, boxBaseVertexFunctionValues_(1, par));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          if (minimalLevel + 2 < boxDivisions_) {
            nboxes_++;
            //index again so use 1, matlab=2
            genBox(nboxes_ - 1, par, minimalLevel + 2, 3, boxBaseVertexFunctionValues_(1, par));
            //index again so use 0, matlab=1
            updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
          } else {
            //index again so use 1, matlab=2
            baseVertex_(splittingIndex) = z_(1, par);
            nbasket_ = nbasket_ + 1;
            pointsInBasket.col(nbasket_) = baseVertex_;
            //index again so use 1, matlab=2
            pointsInBasketValue_(nbasket_) = boxBaseVertexFunctionValues_(1, par);
          }
        }
      }
    } else {
      //index again so use 0, matlab=1
      baseVertex_(splittingIndex) = z_(0, par);
      nbasket_ = nbasket_ + 1;
      pointsInBasket.col(nbasket_) = baseVertex_;
      //index again so use 0, matlab=1
      pointsInBasketValue_(nbasket_) = boxBaseVertexFunctionValues_(0, par);

      //index again so use 1, matlab=2
      baseVertex_(splittingIndex) = z_(1, par);
      nbasket_ = nbasket_ + 1;
      pointsInBasket.col(nbasket_) = baseVertex_;
      //index again so use 1, matlab=2
      pointsInBasketValue_(nbasket_) = boxBaseVertexFunctionValues_(1, par);
    }
    return this->isFinished() || this->isTerminated();
  }

  template<class DistanceFunction>
  arma::Col<double> MultilevelCoordinateSearch<DistanceFunction>::expectedGainOfSplit(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2) {
    double emin = arma::datum::inf;
    arma::Col<double> expectedGain = arma::Col<double>(numberOfDimensions);
    for (int i = 0; i < numberOfDimensions; i++) {
      if (n0(i) == 0) {
        //expected gain for splitting according to the initialization list
        expectedGain(i) = arma::min(initListValues_.col(i)) - initListValues_(initialPointIndex_(i), i);
        if (expectedGain(i) < emin) {
          emin = expectedGain(i);
          isplit_(par) = i;
          //index again so use 1, matlab=2
          z_(1, par) = arma::datum::inf;
        }
      } else {
        arma::Col<double> z1;
        z1 << baseVertex_(i) << x1(i) << x2(i) << arma::endr;
        //index again at "boxBaseVertexFunctionValues_(0,par)" so use 0, matlab=1
        arma::Col<double> z2;
        z2 << 0 << f1(i) - boxBaseVertexFunctionValues_(0, par) << f2(i) - boxBaseVertexFunctionValues_(0, par) << arma::endr;
        arma::Col<double> d = quadraticPolynomialInterpolation(z1, z2);
        arma::Col<double> eta = subint(baseVertex_(i), oppositeVertex_(i));
        //safeguard against splitting too close to x(i)
        double xi1 = arma::min(eta);
        double xi2 = arma::max(eta);
        double z = minimumQuadraticPolynomial(xi1, xi2, d, z1);
        expectedGain(i) = quadraticPolynomial(z, d, z1);
        if (expectedGain(i) < emin) {
          emin = expectedGain(i);
          isplit_(par) = i;
          //index again so use 1, matlab=2
          z_(1, par) = z;
        }
      }
    }
    return expectedGain;
  }

  template<class DistanceFunction>
  unsigned int MultilevelCoordinateSearch<DistanceFunction>::startSweep() {
    std::cout << "startsweep inside" << std::endl;
    record_ = arma::Col<arma::uword>(boxDivisions_ - 1, arma::fill::zeros);
    unsigned int s = boxDivisions_;
    std::cout << "s = " << s << std::endl;
    std::cout << level_.subvec(0, 10) << std::endl;
    for (unsigned int i = 0; i < nboxes_; i++) {
      if (level_(i) > 0) {
        if (level_(i) < s) {
          s = level_(i);
          std::cout << "s = " << s << std::endl;
        }
        if (!record_(level_(i))) {
          record_(level_(i)) = i;
        } else if (boxBaseVertexFunctionValues_(0, i) < boxBaseVertexFunctionValues_(0, record_(level_(i)))) {
          record_(level_(i)) = i;
        }
      }
    }
    std::cout << "startsweep done, s = " << s << std::endl;
    return s;
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vertex(unsigned int par, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2, arma::Col<arma::uword> n0) {
    //for convenience
    unsigned int numberOfDimensions = this->optimisationProblem_->getNumberOfDimensions();
    std::cout << "starting vertex" << std::endl;

    //TODO: check if this->populationSize_ comparisons need to be '-1'd
    baseVertex_ = arma::Col<double>(numberOfDimensions);
    baseVertex_.fill(arma::datum::inf);
    oppositeVertex_ = arma::Col<double>(numberOfDimensions);
    oppositeVertex_.fill(arma::datum::inf);

    double fold = boxBaseVertexFunctionValues_(0, par);
    unsigned int mVertex = par;
    while (mVertex > 1) {
      std::cout << "mVertex = " << mVertex << std::endl;
      std::cout << ipar_.subvec(0, 10) << std::endl;
      std::cout << isplit_.subvec(0, 10) << std::endl;
      unsigned int i = std::abs(isplit_(ipar_(mVertex)));
      std::cout << i << std::endl;
      std::cout << n0.n_elem << std::endl;
      n0(i) = n0(i) + 1;
      if (ichild_(mVertex) == 1) {
        if (baseVertex_(i) == arma::datum::inf || baseVertex_(i) == z_(0, ipar_(mVertex))) {
          //matlab passes 2, but it's used as an index so we need to use 1
          vert1(i, 1, mVertex, baseVertex_, x1, x2, f1, f2);
        } else {
          updtf(numberOfDimensions, i, fold, x1, x2, f1, f2, boxBaseVertexFunctionValues_(0, ipar_(mVertex)));
          fold = boxBaseVertexFunctionValues_(0, ipar_(mVertex));
          //matlab passes 1, but it's used as an index so we need to use 0
          vert2(i, 0, mVertex, baseVertex_, x1, x2, f1, f2);
        }
      } else if (ichild_(mVertex) >= 2) {
        updtf(numberOfDimensions, i, fold, x1, x2, f1, f2, boxBaseVertexFunctionValues_(0, ipar_(mVertex)));
        fold = boxBaseVertexFunctionValues_(0, ipar_(mVertex));
        if (baseVertex_(i) == arma::datum::inf || baseVertex_(i) == z_(1, ipar_(mVertex))) {
          //matlab passes 1, but it's used as an index so we need to use 0
          vert1(i, 0, mVertex, baseVertex_, x1, x2, f1, f2);
        } else {
          //matlab passes 2, but it's used as an index so we need to use 1
          vert2(i, 1, mVertex, baseVertex_, x1, x2, f1, f2);
        }
      }
      //matlab checks for 1/2, since it's index use 0/1
      //original matlab code: 1 <= ichild(m) & ichild(m) <= 2 & y(i) == Inf
      if ((ichild_(mVertex) == 1 || ichild_(mVertex) == 2) && oppositeVertex_(i) == arma::datum::inf) {
        oppositeVertex_(i) = splitByGoldenSectionRule(z_(0, ipar_(mVertex)), z_(1, ipar_(mVertex)), boxBaseVertexFunctionValues_(0, ipar_(mVertex)), boxBaseVertexFunctionValues_(1, ipar_(mVertex)));
      }
      //box m was generated by splitting according to the init. list
      if (ichild_(mVertex) < 0) {
        int j1 = 0;
        int j2 = 0;
        int j3 = 0;
        int k = 0;
        if (boundaries_.col(0)(i) < x0_(i, 0)) {
          j1 = std::ceil(std::abs(ichild_(mVertex)) / 2.0);
          j2 = std::floor(std::abs(ichild_(mVertex)) / 2.0);
          if ((std::abs(ichild_(mVertex)) / 2.0 < j1 && j1 > 1) || j1 == this->populationSize_) {
            j3 = -1;
          } else {
            j3 = 1;
          }
        } else {
          j1 = std::floor(std::abs(ichild_(mVertex)) / 2.0) + 1;
          j2 = std::ceil(std::abs(ichild_(mVertex)) / 2.0);
          if ((std::abs(ichild_(mVertex)) / 2.0 + 1) > j1 && j1 < this->populationSize_) {
            j3 = 1;
          } else {
            j3 = -1;
          }
        }
        //box m was generated in the init. procedure
        if (isplit_(ipar_(mVertex)) < 0) {
          k = i;
          //box m was generated by a later split according to the init.list
          //k points to the corresponding function values  
        } else {
          //matlab passes 2, but it's used as an index so we need to use 1
          k = z_(1, ipar_(mVertex));
        }
        //Decrementing all indices except j2
        j1--;
        j3--;
        k--;

        if (j1 != initialPointIndex_(i) || (baseVertex_(i) != arma::datum::inf && baseVertex_(i) != x0_(i, (initialPointIndex_(i))))) {
          updtf(numberOfDimensions, i, fold, x1, x2, f1, f2, initListValues_(initialPointIndex_(i), k));
          fold = initListValues_(initialPointIndex_(i), k);
        }
        if (baseVertex_(i) == arma::datum::inf || baseVertex_(i) == x0_(i, j1)) {
          baseVertex_(i) = x0_(i, j1);
          if (x1(i) == arma::datum::inf) {
            vert3(i, j1, initListValues_.col(k), x1, x2, f1, f2);
          } else if (x2(i) == arma::datum::inf && x1(i) != x0_(i, j1 + j3)) {
            x2(i) = x0_(i, j1 + j3);
            f2(i) = f2(i) + initListValues_(j1 + j3, k);
          } else if (x2(i) == arma::datum::inf) {
            //matlab checks for 1, since it's index use 0
            if (j1 != 0 && j1 != this->populationSize_-1) {
              x2(i) = x0_(i, j1 - j3);
              f2(i) = f2(i) + initListValues_(j1 - j3, k);
            } else {
              x2(i) = x0_(i, j1 + 2 * j3);
              f2(i) = f2(i) + initListValues_(j1 + 2 * j3, k);
            }
          }
        } else {
          if (x1(i) == arma::datum::inf) {
            x1(i) = x0_(i, j1);
            f1(i) = f1(i) + initListValues_(j1, k);
            if (baseVertex_(i) != x0_(i, j1 + j3)) {
              x2(i) = x0_(i, j1 + j3);
              f2(i) = f2(i) + initListValues_(j1 + j3, k);
            }
          } else if (x2(i) == arma::datum::inf) {
            if (x1(i) != x0_(i, j1)) {
              x2(i) = x0_(i, j1);
              f2(i) = f2(i) + initListValues_(j1, k);
            } else if (baseVertex_(i) != x0_(i, j1 + j3)) {
              x2(i) = x0_(i, j1 + j3);
              f2(i) = f2(i) + initListValues_(j1 + j3, k);
            } else {
              //matlab checks for 1, since it's index use 0
              if (j1 != 0 && j1 != this->populationSize_-1) {
                x2(i) = x0_(i, j1 - j3);
                f2(i) = f2(i) + initListValues_(j1 - j3, k);
              } else {
                x2(i) = x0_(i, j1 + 2 * j3);
                f2(i) = f2(i) + initListValues_(j1 + 2 * j3, k);
              }
            }
          }
        }
        if (oppositeVertex_(i) == arma::datum::inf) {
          //TODO: why is there an index check for 0 in matlab?!!?
          if (j2 == 0) {
            oppositeVertex_(i) = boundaries_.col(0)(i);
          } else if (j2 == this->populationSize_) {
            oppositeVertex_(i) = boundaries_.col(1)(i);
          } else {
            //added -1 to all j2 since it's calculated as a matlab index.
            oppositeVertex_(i) = splitByGoldenSectionRule(x0_(i, j2-1), x0_(i, j2 + 1-1), initListValues_(j2-1, k), initListValues_(j2 + 1-1, k));
          }
        }
      }
      mVertex = ipar_(mVertex);
    }
    std::cout << "finishing loop" << std::endl;
    for (int i = 0; i < numberOfDimensions; i++) {
      std::cout << "i = " << i << std::endl;
      if (baseVertex_(i) == arma::datum::inf) {
        baseVertex_(i) = x0_(i, initialPointIndex_(i));
        vert3(i, initialPointIndex_(i), initListValues_.col(i), x1, x2, f1, f2);
      }
      if (oppositeVertex_(i) == arma::datum::inf) {
        oppositeVertex_(i) = oppositeVertex_(i);
      }
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vert1(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2) {
    int j1 = 0;
    //matlab checks for 1, since it's index use 0
    if (j == 0) {
      //also index again
      j1 = 1;
    } else {
      j1 = 0;
    }
    x(updateIndex) = z_(j1, ipar_(m));
    if (x1(updateIndex) == arma::datum::inf) {
      x1(updateIndex) = z_(j, ipar_(m));
      f1(updateIndex) = f1(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m));
    } else if (x2(updateIndex) == arma::datum::inf && x1(updateIndex) != z_(j, ipar_(m))) {
      x2(updateIndex) = z_(j, ipar_(m));
      f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m));
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vert2(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2) {
    int j1 = 0;
    //matlab checks for 1, since it's index use 0
    if (j == 0) {
      //also index again
      j1 = 1;
    } else {
      j1 = 0;
    }
    if (x1(updateIndex) == arma::datum::inf) {
      x1(updateIndex) = z_(j, ipar_(m));
      f1(updateIndex) = f1(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m));
      if (x(updateIndex) != z_(j1, ipar_(m))) {
        x2(updateIndex) = z_(j1, ipar_(m));
        f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j1, ipar_(m));
      }
    } else if (x2(updateIndex) == arma::datum::inf && x1(updateIndex) != z_(j, ipar_(m))) {
      x2(updateIndex) = z_(j, ipar_(m));
      f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m));
    } else if (x2(updateIndex) == arma::datum::inf) {
      x2(updateIndex) = z_(j1, ipar_(m));
      f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j1, ipar_(m));
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vert3(int updateIndex, unsigned int j, arma::Col<double> f0col, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2) {
    int k1 = 0;
    int k2 = 0;
    //matlab checks for 1, since it's index use 0
    if (j == 0) {
      //also index again
      k1 = 1;
      k2 = 2;
      //decremented cause index
    } else if (j == this->populationSize_ - 1) {
      //decremented cause index
      k1 = this->populationSize_ - 3;
      k2 = this->populationSize_ - 2;
    } else {
      k1 = j - 1;
      k2 = j + 1;
    }
    x1(updateIndex) = x0_(updateIndex, k1);
    x2(updateIndex) = x0_(updateIndex, k2);
    f1(updateIndex) = f1(updateIndex) + f0col(k1);
    f2(updateIndex) = f2(updateIndex) + f0col(k2);
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::updtf(unsigned int numberOfDimensions, unsigned int splittingIndex, double fold, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2, double baseVertexValueCurrentBox) {
    for (int i = 0; i < numberOfDimensions; i++) {
      if (i != splittingIndex) {
        if (x1(i) == arma::datum::inf) {
          f1(i) = f1(i) + fold - baseVertexValueCurrentBox;
        }
        if (x2(i) == arma::datum::inf) {
          f2(i) = f2(i) + fold - baseVertexValueCurrentBox;
        }
      }
    }
    //updtf.m sets fold = f here. since the inputvalue fold never gets changed, this doesn't actually belong here.
  }

  template<class DistanceFunction>
  arma::Col<double> MultilevelCoordinateSearch<DistanceFunction>::subint(double x, double y) {
    int x2 = y;
    int f = 1000;
    if (f * std::abs(x) < 1) {
      if (std::abs(y) > f) {
        x2 = std::copysign(1.0, y);
      }
    } else {
      if (std::abs(y) > f * std::abs(x)) {
        //TODO: c++ standardlibraries have no signum. wat. using copysign instead...
        //original matlab: x2 = 10.*sign(y)*abs(x);
        x2 = 10 * std::copysign(x, y);
      }
    }
    return arma::Col<double>(x + (x2 - x) / 10.0, x2);
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::updateRecord(unsigned int label, int level, arma::Col<double> f) {
    std::cout << "inside updaterecord, level = " << level << std::endl;
    if (record_.n_elem < level) {
      std::cout << "first if" << std::endl;
      record_(level) = label;
    } else if (record_(level) == 0) {
      std::cout << "second if" << std::endl;
      record_(level) = label;
    } else if (f(label) < f(record_(level))) {
      std::cout << "third if" << std::endl;
      record_(level) = label;
    }
    std::cout << "updaterecord done" << std::endl;
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::checkLocationNotUsed(arma::Col<double> location) {
    for (int i = 0; i < nloc_; i++) {
      //TODO: This might not work correctly
      if (arma::all(location == xloc_.col(i))) {
        return false;
      }
    }
    return true;
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::addLocation(arma::Col<double> loc) {
    nloc_++;
    if (xloc_.n_cols < nloc_) {
      xloc_.resize(xloc_.n_cols + step);
      xloc_.col(nloc_) = loc;
    } else {
      xloc_.col(nloc_) = loc;
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::setLocalSearch(const std::shared_ptr<OptimisationAlgorithm<double, DistanceFunction>> localSearch) noexcept {
    localSearch_ = localSearch;
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::pointInsideDomainOfAttraction(arma::Col<double> loc, std::shared_ptr<double> valueAtLoc, double nbasket) {
    if (nbasket == 0) {
      return true;
    }
    arma::Col<double> distancesToLoc(nbasket);
    for (int k = 0; k < nbasket; k++) {
      distancesToLoc(k) = arma::norm(loc - pointsInBasket_.col(k));
    }
    arma::Col<arma::uword> sortedDistances = arma::sort_index(distancesToLoc);
    for (int k = 0; k < nbasket; k++) {
      int i = sortedDistances(k);
      if (pointsInBasketValue_(i) <= *valueAtLoc) {
        arma::Col<double> p = pointsInBasket_(i) - loc;
        arma::Col<double> y1 = loc + (1 / 3.0) * p;
        double f1 = this->optimisationProblem_->getObjectiveValue(y1);
        this->numberOfIterations_++;
        if (f1 <= *valueAtLoc) {
          arma::Col<double> y2 = loc + (2 / 3.0) * p;
          double f2 = this->optimisationProblem_->getObjectiveValue(y2);
          if (f2 > std::max(f1, pointsInBasketValue_(i))) {
            if (f1 < *valueAtLoc) {
              loc = y1;
              *valueAtLoc = f1;
              if (*valueAtLoc < this->bestObjectiveValue_) {
                this->bestObjectiveValue_ = *valueAtLoc;
                this->bestParameter_ = loc;
                if (this->isFinished() || this->isTerminated()) {
                  return false;
                }
              }
            }
          } else {
            if (f1 < std::min(f2, pointsInBasketValue_(i))) {
              *valueAtLoc = f1;
              loc = y1;
              if (*valueAtLoc < this->bestObjectiveValue_) {
                this->bestObjectiveValue_ = *valueAtLoc;
                this->bestParameter_ = loc;
                if (this->isFinished() || this->isTerminated()) {
                  return false;
                }
              }
            } else if (f2 < std::min(f1, pointsInBasketValue_(i))) {
              *valueAtLoc = f2;
              loc = y2;
              if (*valueAtLoc < this->bestObjectiveValue_) {
                this->bestObjectiveValue_ = *valueAtLoc;
                this->bestParameter_ = loc;
                if (this->isFinished() || this->isTerminated()) {
                  return false;
                }
              }
            } else {
              return false;
            }
          }
        }
      }
    }
    return true;
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::candidateInsideDomainOfAttraction(arma::Col<double> candidate, double valueAtCandidate, double nbasket) {
    if (nbasket == 0) {
      return true;
    }
    arma::Col<double> distancesToCandidate(nbasket);
    for (int k = 0; k < nbasket; k++) {
      distancesToCandidate(k) = arma::norm(candidate - pointsInBasket_.col(k));
    }
    arma::Col<arma::uword> sortedDistances = arma::sort_index(distancesToCandidate);
    for (int k = 0; k < nbasket; k++) {
      int i = sortedDistances(k);
      arma::Col<double> p = pointsInBasket_.col(i) - candidate;
      arma::Col<double> y1 = candidate + (1 / 3.0) * p;
      double f1 = this->optimisationProblem_->getObjectiveValue(y1);
      this->numberOfIterations_++;
      if (f1 <= std::max(pointsInBasketValue_(i), valueAtCandidate)) {
        arma::Col<double> y2 = candidate + (2 / 3.0) * p;
        double f2 = this->optimisationProblem_->getObjectiveValue(y2);
        this->numberOfIterations_++;
        if (f2 <= std::max(f1, pointsInBasketValue_(i))) {
          if (valueAtCandidate < std::min(std::min(f1, f2), pointsInBasketValue_(i))) {
            pointsInBasketValue_(i) = valueAtCandidate;
            pointsInBasket_.col(i) = candidate;
            if (valueAtCandidate < this->bestObjectiveValue_) {
              this->bestObjectiveValue_ = valueAtCandidate;
              this->bestParameter_ = candidate;
              if (this->isFinished() || this->isTerminated()) {
                return false;
              }
            }
            return false;
          } else if (f1 < std::min(std::min(valueAtCandidate, f2), pointsInBasketValue_(i))) {
            pointsInBasketValue_(i) = f1;
            pointsInBasket_.col(i) = y1;
            if (valueAtCandidate < this->bestObjectiveValue_) {
              this->bestObjectiveValue_ = valueAtCandidate;
              this->bestParameter_ = candidate;
              if (this->isFinished() || this->isTerminated()) {
                return false;
              }
            }
            return false;
          } else if (f2 < std::min(std::min(valueAtCandidate, f1), pointsInBasketValue_(i))) {
            pointsInBasketValue_(i) = f2;
            pointsInBasket_.col(i) = y2;
            if (valueAtCandidate < this->bestObjectiveValue_) {
              this->bestObjectiveValue_ = valueAtCandidate;
              this->bestParameter_ = candidate;
              if (this->isFinished() || this->isTerminated()) {
                return false;
              }
            }
            return false;
          } else {
            return false;
          }
        }
      }
    }
    return true;
  }
  // </editor-fold>
}

