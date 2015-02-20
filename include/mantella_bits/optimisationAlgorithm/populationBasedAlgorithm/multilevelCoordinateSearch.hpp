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

    arma::Col<arma::sword> isplit_; //accessed by matlab-1, values are matlab
    arma::Col<arma::uword> level_; //accessed by matlab-1, values are matlab
    arma::Col<arma::uword> ipar_; //accessed by matlab-1, values are matlab
    arma::Col<arma::sword> ichild_; //accessed by matlab-1, values are matlab
    arma::Mat<double> boxBaseVertexFunctionValues_; //f
    arma::Mat<double> z_;
    arma::Col<arma::uword> nogain_;

    arma::Col<arma::uword> initialPointIndex_; //l
    arma::Mat<double> x0_;
    arma::Mat<double> initListValues_; //f0
    arma::Col<arma::uword> bestPointIndex_; //istar

    arma::Col<double> baseVertex_; //x
    arma::Col<double> originalOppositeVertex_; //v1
    arma::Col<double> oppositeVertex_; //y

    arma::Col<arma::uword> record_; //accessed by matlab-1, values are matlab
    unsigned int nboxes_ = 1;
    unsigned int nbasket_ = 0;
    unsigned int nbasket0_ = 0;
    unsigned int m_;
    unsigned int nloc_ = 0;
    arma::Mat<double> xloc_;

    void initBoxes(); //initbox.m
    arma::Col<arma::uword> variabilityRanking_; //p, accessed by matlab-1, values are matlab-1

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
    bool splitByInitList(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par);

    //split.m 
    //splits box # par at level s in its ith coordinate into two or three 
    //children and inserts its children and their parameters in the list
    //returns isFinished() || isTerminated()
    bool split(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par);

    //exgain.m 
    //determines the splitting index, the splitting value and the expected 
    //gain vector e for (potentially) splitting a box by expected gain
    arma::Col<double> expectedGainOfSplit(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2);

    unsigned int startSweep(); //strtsw.m

    //vertex.m 
    void vertex(unsigned int par, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2, arma::Col<arma::uword>& n0);

    void vert1(int updateIndex, unsigned int j, unsigned int m, arma::Col<double>& x, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2); //vert1.m

    void vert2(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2); //vert2.m

    void vert3(int updateIndex, unsigned int j, const arma::sword& f0columnIndex, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2); //vert3.m

    void updtf(unsigned int numberOfDimensions, unsigned int splittingIndex, double fold, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double>& f1, arma::Col<double>& f2, double baseVertexValueCurrentBox); //updtf.m

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
    void addLocation(arma::Col<double>& loc);

    //basket.m 
    //checks whether a candidate for local search lies in the 'domain of 
    //attraction' of a point in the 'shopping basket'
    bool pointInsideDomainOfAttraction(arma::Col<double>& loc, std::shared_ptr<double> valueAtLoc, double nbasket);

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

    this->bestParameter_ = arma::Col<double>(numberOfDimensions, arma::fill::zeros);

    std::shared_ptr<OptimisationAlgorithm<double, DistanceFunction >> test(new HillClimbing<double, DistanceFunction>(optimisationProblem));

    setLocalSearch(test);

    std::cout << "assigning boundary" << std::endl;

    boundaries_ = arma::Mat<double>(numberOfDimensions, 2);
    boundaries_.col(0) = optimisationProblem->getLowerBounds();
    boundaries_.col(1) = optimisationProblem->getUpperBounds();

    //assigning standard values for variables. Can't do in header-file since dependent on input variable "boundaries"
    if (boxDivisions_ == 0) {
      boxDivisions_ = 5 * numberOfDimensions + 10;
      std::cout << "boxDivisions_ = " << boxDivisions_ << std::endl;
    }

    std::cout << "initting large arrays" << std::endl;
    //init of large arrays
    isplit_ = arma::Col<arma::sword>(step1_, arma::fill::zeros);
    level_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);
    ipar_ = arma::Col<arma::uword>(step1_, arma::fill::zeros);
    ichild_ = arma::Col<arma::sword>(step1_, arma::fill::zeros);
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
    std::cout << initialPoint << std::endl;

    double valueForInitialPoint = this->optimisationProblem_->getObjectiveValue(initialPoint);
    std::cout << valueForInitialPoint << std::endl;
    initListValues_(0, initialPointIndex_(0)) = valueForInitialPoint;
    this->numberOfIterations_++;

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
          if (initListValues_(j, i) < valueForInitialPoint) {
            valueForInitialPoint = initListValues_(j, i);
            bestPointIndex_(i) = j;
          }
        }
      }
      initialPoint(i) = x0_(i, bestPointIndex_(i));
    }

    //base vertex and opposite vertex init
    baseVertex_ = arma::Col<double>(numberOfDimensions);
    originalOppositeVertex_ = arma::Col<double>(numberOfDimensions);
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      baseVertex_(i) = x0_(i, initialPointIndex_(i));

      //if true, use u, else use v
      if (std::abs(baseVertex_(i) - boundaries_.col(0)(i)) > std::abs(baseVertex_(i) - boundaries_.col(1)(i))) {
        originalOppositeVertex_(i) = boundaries_.col(0)(i);
      } else {
        originalOppositeVertex_(i) = boundaries_.col(1)(i);
      }
    }

    //init of record list, nboxes, nbasket,nbasket0, m, nloc, xloc
    //values not listed here are defined in header
    record_ = arma::Col<arma::uword>(boxDivisions_ - 1, arma::fill::zeros);
    m_ = numberOfDimensions;
    record_(0) = 1;
    xloc_ = arma::Mat<double>(numberOfDimensions, step1_);
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::optimiseImplementation() {
    //for convenience
    unsigned int numberOfDimensions = this->optimisationProblem_->getNumberOfDimensions();

    this->bestParameter_ = arma::Col<double>(numberOfDimensions, arma::fill::zeros);
    //generate boxes
    std::cout << "initting boxes" << std::endl;
    std::cout << initListValues_.submat(0, 0, 2, 1) << std::endl;
    initBoxes();


    //TODO: find better value than step1_...
    pointsInBasket_ = arma::Mat<double>(numberOfDimensions, step1_); //xmin
    pointsInBasketValue_ = arma::Col<double>(step1_); //fmi

    // s - value is matlab - the vector record is updated, and the minimal level s containing non-split boxes is computed
    unsigned int minimalLevel = startSweep();

    std::cout << "start of while, running until " << boxDivisions_ << std::endl;
    //int rofl = 0;
    while (!this->isFinished() && !this->isTerminated() && minimalLevel < boxDivisions_) {
      //while (!this->isFinished() && !this->isTerminated() && rofl < 10) {
      //rofl++;
      std::cout << "minimallevel " << minimalLevel << std::endl;
      unsigned int par = record_(minimalLevel - 1) - 1; //the best box at level s is the current box
      std::cout << "current par " << par << std::endl;

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
      std::cout << "x1" << x1 << std::endl;
      std::cout << "x2" << x2 << std::endl;
      std::cout << "f1" << f1 << std::endl;
      std::cout << "f2" << f2 << std::endl;

      bool doSplit = false; //splt
      if (minimalLevel > 2 * numberOfDimensions * (arma::min(n0) + 1)) {
        std::cout << "before splitbyrank" << std::endl;
        splitByRank(par, numberOfDimensions, n0);
        doSplit = true;
        std::cout << "after splitbyrank" << std::endl;
      } else {
        //TODO: this if should be unnecessary in c++. else be !if
        std::cout << "nogain(" << par << ") = " << nogain_(par) << std::endl;
        if (nogain_(par)) {
          doSplit = false;
        } else {
          std::cout << "before expgain" << std::endl;
          arma::Col<double> expectedGain = expectedGainOfSplit(par, numberOfDimensions, n0, x1, x2, f1, f2); //e
          std::cout << "after expgain" << std::endl;

          //index again so use 0, matlab=1
          double fexp = boxBaseVertexFunctionValues_(0, par) + arma::min(expectedGain);
          std::cout << "fexp = " << fexp << std::endl;
          std::cout << "fbest = " << this->bestObjectiveValue_ << std::endl;
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
        int i = isplit_(par) - 1;
        level_(par) = 0;
        if (z_(1, par) == arma::datum::inf) {
          m_++;
          z_(1, par) = m_;
          std::cout << "before splitbyinitlist" << std::endl;
          if (splitByInitList(i, minimalLevel, par)) {
            break; //should break out of major while loop
          }
          std::cout << "after splitbyinitlist" << std::endl;
        } else {
          z_(0, par) = baseVertex_(i);
          std::cout << "before split" << std::endl;
          if (split(i, minimalLevel, par)) {
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
          
          updateRecord(par, minimalLevel + 1, boxBaseVertexFunctionValues_.row(0).t());
        } else {
          level_(par) = 0;
          nbasket_++;
          pointsInBasket_.col(nbasket_ - 1) = baseVertex_;
          //index again so use 0, matlab=1
          pointsInBasketValue_(nbasket_ - 1) = boxBaseVertexFunctionValues_(0, par);
        }
      } //end of prepare for splitting
      std::cout << "splitting logic finished" << std::endl;
      minimalLevel++;
      std::cout << "minimallevel before = " << minimalLevel << std::endl;
      std::cout << record_ << std::endl;
      while (minimalLevel < boxDivisions_) {
        std::cout << "record(" << minimalLevel - 1 << ") = " << record_(minimalLevel - 1) << std::endl;
        if (record_(minimalLevel - 1) == 0) {
          minimalLevel++;
        } else {
          break;
        }
      }
      std::cout << "minimallevel after = " << minimalLevel << std::endl;
      //if smax is reached, a new sweep is started 
      std::cout << "smax reached " << (minimalLevel == boxDivisions_) << std::endl;
      if (minimalLevel == boxDivisions_) {
        if (maxLocalSearchSteps_ > 0) {
          std::cout << "start local search" << std::endl;
          std::cout << pointsInBasketValue_.n_elem << std::endl;
          std::cout << nbasket0_ << std::endl;
          std::cout << nbasket_ << std::endl;
          std::cout << pointsInBasketValue_.rows(nbasket0_, nbasket_ - 1) << std::endl;
          //original matlab sort: [fmi(nbasket0+1:nbasket),j] = sort(fmi(nbasket0+1:nbasket));
          //original matlab sort: xmin(:,nbasket0+1:nbasket) = xmin(:,nbasket0+j);
          arma::Col<arma::uword> sortedPIBV = arma::sort_index(pointsInBasketValue_.rows(nbasket0_, nbasket_ - 1));
          //since we sort over a subview the indexes will start at 0, but we use them for the whole matrix afterwards
          sortedPIBV+=nbasket0_;
          pointsInBasketValue_.rows(nbasket0_, nbasket_ - 1) = arma::sort(pointsInBasketValue_.rows(nbasket0_, nbasket_ - 1));
          pointsInBasket_.cols(nbasket0_,nbasket_-1) = pointsInBasket_.cols(sortedPIBV);

          for (int j = nbasket0_; j < nbasket_; j++) {
            std::cout << "j = " << j << std::endl;
            //programmatically it seems dumb to overwrite a global variable, but they do it in matlab.
            baseVertex_ = pointsInBasket_.col(j);
            //unfortunately multiple return values of primitives are needed later, so we have to use pointers
            //f1 is already a vector earlier. Why is he overwriting it in matlab??
            std::shared_ptr<double> f1(new double(pointsInBasketValue_(j)));

            if (checkLocationNotUsed(baseVertex_)) {
              std::cout << "location not used" << std::endl;
              addLocation(baseVertex_);

              //intentionally nbasket0, not -1
              bool isInside = pointInsideDomainOfAttraction(baseVertex_, f1, nbasket0_);
              if (this->isFinished() || this->isTerminated()) {
                return;
              }
              if (isInside) {
                std::cout << "point inside" << std::endl;
                ///////////////////////////TODO: TEMPORARY WORKAROUND FOR LSEARCH///////////////////////////
                arma::Col<double> xminTemp(numberOfDimensions, arma::fill::zeros);
                double fmiTemp = 1000;
                //TODO: set bestparameter of localsearch here
                //localSearch_->setMaximalNumberOfIterations(localSearch_->getNumberOfIterations() + maxLocalSearchSteps_);
                //TODO: run localsearch, extract values

                if (fmiTemp < this->bestObjectiveValue_) {
                  this->bestParameter_ = xminTemp;
                  this->bestObjectiveValue_ = fmiTemp;
                  if (this->isFinished() || this->isTerminated()) {
                    nbasket0_++;
                    nbasket_ = nbasket0_;
                    pointsInBasket_.col(nbasket_ - 1) = xminTemp;
                    pointsInBasketValue_(nbasket_ - 1) = fmiTemp;
                    break;
                  }
                }
                //intentionally nbasket0, not -1
                bool candidateInside = candidateInsideDomainOfAttraction(xminTemp, fmiTemp, nbasket0_);
                if (candidateInside) {
                  std::cout << "candidate inside" << std::endl;
                  nbasket0_++;
                  pointsInBasket_.col(nbasket0_ - 1) = xminTemp;
                  pointsInBasketValue_(nbasket0_ - 1) = fmiTemp;
                  //TODO: this check seems redundant, should check if it ever gets called
                  if (pointsInBasketValue_(nbasket0_ - 1) < this->bestObjectiveValue_) {
                    std::cout << "this should never get called" << std::endl;
                    this->bestObjectiveValue_ = pointsInBasketValue_(nbasket0_ - 1);
                    this->bestParameter_ = pointsInBasket_.col(nbasket0_ - 1);
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
          std::cout << "nbasket = " << nbasket_ << std::endl;
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
      isplit_(parent) = -(i + 1); //boxes split in the init. procedure get a negative splitting index
      std::cout << "isplit_(" << parent << ") = " << -i << std::endl;
      int nchild = 0;
      if (x0_(i, 0) > boundaries_.col(0)(i)) {
        std::cout << "first if taken" << std::endl;
        nboxes_++;
        nchild++;
        genBox(nboxes_ - 1, parent, level_(parent) + 1, -nchild, initListValues_(0, i));
      }

      double v1 = 0;
      //in matlab there is more logic applied to v1, but since popSize is fixed for all dimensions, it doesn't matter
      if (this->populationSize_ == 3) {
        std::cout << "pop is 3" << std::endl;
        v1 = boundaries_.col(1)(i);
      } else {
        std::cout << "pop isnt 3" << std::endl;
        v1 = x0_(i, 2);
      }
      std::cout << "v1 = " << v1 << std::endl;

      arma::Col<double> d = quadraticPolynomialInterpolation(x0_.row(i).subvec(0, 2).t(), initListValues_.col(i).subvec(0, 2));
      double xl = minimumQuadraticPolynomial(boundaries_.col(0)(i), v1, d, x0_.submat(i, 0, i, 2));
      double fl = quadraticPolynomial(xl, d, x0_.submat(i, 0, i, 2));
      double xu = minimumQuadraticPolynomial(boundaries_.col(0)(i), v1, -d, x0_.submat(i, 0, i, 2));
      double fu = quadraticPolynomial(xu, d, x0_.submat(i, 0, i, 2));
      std::cout << "d = " << d << std::endl;
      std::cout << "xl = " << xl << std::endl;
      std::cout << "fl = " << fl << std::endl;
      std::cout << "xu = " << xu << std::endl;
      std::cout << "fu = " << fu << std::endl;

      int par1 = 0; //label of the current box for the next coordinate
      if (bestPointIndex_(i) == 0) {
        std::cout << "istar(i) is 1" << std::endl;
        if (xl < x0_(i, 0)) {
          std::cout << "xl smaller x0" << std::endl;
          par1 = nboxes_;
        } else {
          std::cout << "xl not smaller x0" << std::endl;
          par1 = nboxes_ + 1;
        }
      }
      for (std::size_t j = 0; j < this->populationSize_ - 1; j++) {
        std::cout << "j = " << j << std::endl;
        nboxes_++;
        nchild++;
        int s = 0;
        if (initListValues_(j, i) <= initListValues_(j + 1, i)) {
          s = 1;
        } else {
          s = 2;
        }
        std::cout << "s = " << s << std::endl;
        genBox(nboxes_ - 1, parent, level_(parent) + s, -nchild, initListValues_(j, i));
        if (j >= 1) {
          std::cout << "j greaterequal 2" << std::endl;
          if (bestPointIndex_(i) == j) {
            std::cout << "istar(i) == j" << std::endl;
            if (xl <= x0_(i, j)) {
              std::cout << "xl smallerequal x0(i,j)" << std::endl;
              par1 = nboxes_ - 1;
            } else {
              std::cout << "xl not smallerequal x0(i,j)" << std::endl;
              par1 = nboxes_;
            }
          }
          if (j <= this->populationSize_ - 3) {
            std::cout << "j <= popsize-2" << std::endl;
            d = quadraticPolynomialInterpolation(x0_.submat(i, j, i, j + 2), initListValues_.submat(j, i, j + 2, i));
            double u1 = 0;
            if (j < this->populationSize_ - 3) {
              std::cout << "j smaller popsize-2" << std::endl;
              u1 = x0_(i, j + 2);
            } else {
              std::cout << "j not smaller popsize-2" << std::endl;
              u1 = boundaries_.col(1)(i);
            }
            xl = minimumQuadraticPolynomial(x0_(i, j), u1, d, x0_.submat(i, j, i, j + 2));
            fl = std::min(quadraticPolynomial(xl, d, x0_.submat(i, j, i, j + 2)), fl);
            xu = minimumQuadraticPolynomial(x0_(i, j), u1, -d, x0_.submat(i, j, i, j + 2));
            fu = std::max(quadraticPolynomial(xu, d, x0_.submat(i, j, i, j + 2)), fu);
            std::cout << "xl = " << xl << std::endl;
            std::cout << "fl = " << fl << std::endl;
            std::cout << "xu = " << xu << std::endl;
            std::cout << "fu = " << fu << std::endl;
          }
        }
        nboxes_++;
        nchild++;
        genBox(nboxes_ - 1, parent, level_(parent) + 3 - s, -nchild, initListValues_(j + 1, i));
      }
      if (x0_(i, this->populationSize_ - 1) < boundaries_.col(0)(i)) {
        std::cout << "x0 smaller v(i)" << std::endl;
        nboxes_++;
        nchild++;
        genBox(nboxes_ - 1, parent, level_(parent) + 1, -nchild, initListValues_(this->populationSize_ - 1, i));
      }
      if (bestPointIndex_(i) == this->populationSize_ - 1) {
        std::cout << "istar == popsize" << std::endl;
        if (x0_(i, this->populationSize_ - 1) < boundaries_.col(0)(i)) {
          std::cout << "x0 smaller v(i)" << std::endl;
          if (xl <= x0_(i, this->populationSize_ - 1)) {
            std::cout << "xl smallerequal x0" << std::endl;
            par1 = nboxes_ - 1;
          } else {
            std::cout << "xl not smallerequal x0" << std::endl;
            par1 = nboxes_;
          }
        } else {
          std::cout << "par1 = nbox" << std::endl;
          par1 = nboxes_;
        }
      }
      var(i) = fu - fl; // the quadratic model is taken as a crude measure of the variability in the ith component
      level_(parent) = 0; //box is marked as split
      std::cout << "nboxes = " << nboxes_ << std::endl;
      std::cout << "par1 = " << par1 << std::endl;
      parent = par1 - 1;
    }
    std::cout << "calculating variability" << std::endl;
    //from matlab: best function value after the init. procedure
    //doesnt make much sense to me since we never changed initListEvaluations
    std::cout << bestPointIndex_ << std::endl;
    std::cout << initListValues_.n_cols << std::endl;
    std::cout << initListValues_.n_rows << std::endl;
    this->bestObjectiveValue_ = initListValues_(bestPointIndex_(numberOfDimensions - 1), numberOfDimensions - 1);
    for (std::size_t i = 0; i < numberOfDimensions; i++) {
      //next two lines should equal [var0,p(i)] = max(var)
      var.max(variabilityRanking_(i));
      std::cout << "var.max = " << var.max() << std::endl;
      var(variabilityRanking_(i)) = -1;
      std::cout << "variabilityRanking(" << i << ") = " << variabilityRanking_(i) << std::endl;
      std::cout << x0_(i, bestPointIndex_(i)) << std::endl;
      this->bestParameter_(i) = x0_(i, bestPointIndex_(i)); //best point after the init. procedure
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
    //since par is matlab-1 in value, and we want matlab value in ipar_
    ipar_(nbox) = par + 1;
    std::cout << "ipar_(" << nbox << ") = " << par + 1 << std::endl;
    level_(nbox) = level;
    std::cout << "level_(" << nbox << ") = " << level << std::endl;
    ichild_(nbox) = nchild;
    std::cout << "ichild_(" << nbox << ") = " << nchild << std::endl;
    boxBaseVertexFunctionValues_(0, nbox) = baseVertexFunctionValue;
    std::cout << "bbvf_(0, " << nbox << ") = " << baseVertexFunctionValue << std::endl;
    std::cout << "genbox done" << std::endl;
  }

  template<class DistanceFunction>
  arma::Col<double> MultilevelCoordinateSearch<DistanceFunction>::quadraticPolynomialInterpolation(arma::Col<double> supportPoints, arma::Col<double> functionValues) {
    arma::Col<double> d(3);
    d(0) = functionValues(0);
    d(1) = (functionValues(1) - functionValues(0)) / (supportPoints(1) - supportPoints(0));
    double f23 = (functionValues(2) - functionValues(1)) / (supportPoints(2) - supportPoints(1));
    d(2) = (f23 - d(1)) / (supportPoints(2) - supportPoints(0));
    return d;
  }

  template<class DistanceFunction>
  double MultilevelCoordinateSearch<DistanceFunction>::minimumQuadraticPolynomial(double a, double b, arma::Col<double> d, arma::Mat<double> x0_fragment) {
    double x = 0;
    if (d(2) == 0) {
      if (d(1) > 0) {
        x = a;
      } else {
        x = b;
      }
      return x;
    } else if (d(2) > 0) {
      double x1 = 0.5 * (x0_fragment(0) + x0_fragment(1)) - 0.5 * (d(1) / d(2));
      if (a <= x1 && x1 <= b) {
        x = x1;
      } else if (quadraticPolynomial(a, d, x0_fragment) < quadraticPolynomial(b, d, x0_fragment)) {
        x = a;
      } else {
        x = b;
      }
    } else {
      if (quadraticPolynomial(a, d, x0_fragment) < quadraticPolynomial(b, d, x0_fragment)) {
        x = a;
      } else {
        x = b;
      }
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
    isplit_(par) = 1;
    int n1 = n0(0);
    int p1 = variabilityRanking_(0);
    //matlab starts at 2
    for (int i = 1; i < numberOfDimensions; i++) {
      if (n0(i) < n1 || (n0(i) == n1 && variabilityRanking_(i) < p1)) {
        isplit_(par) = i + 1;
        n1 = n0(i);
        p1 = variabilityRanking_(i);
      }
    }
    std::cout << "isplit(" << par << ") " << isplit_(par) << std::endl;
    std::cout << "n1 " << n1 << std::endl;
    std::cout << "p1 " << p1 << std::endl;
    if (n1 > 0) {
      z_(1, par) = splitBySubint(baseVertex_(isplit_(par) - 1), oppositeVertex_(isplit_(par) - 1));
    } else {
      z_(1, par) = arma::datum::inf;
    }
  }

  template<class DistanceFunction>
  double MultilevelCoordinateSearch<DistanceFunction>::splitBySubint(double x, double y) {
    std::cout << "splitbysubint start" << std::endl;
    std::cout << "x = " << x << ", y = " << y << std::endl;
    double x2 = y;
    if (x == 0 && std::abs(y) > 1000) {
      x2 = std::copysign(1.0, y);
    } else if (x != 0 && std::abs(y) > 100 * std::abs(x)) {
      //TODO: c++ standardlibraries have no signum. wat. using copysign instead...
      //original matlab: x2 = 10.*sign(y)*abs(x);
      x2 = 10 * std::copysign(x, y);
    }
    std::cout << "x1 = " << x + 2 * (x2 - x) / 3.0 << std::endl;
    std::cout << "splitbysubint finish" << std::endl;
    return x + 2 * (x2 - x) / 3.0;
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::splitByInitList(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par) {
    arma::Col<double> x = baseVertex_;
    initListValues_.col(m_ - 1).zeros();
    for (int j = 0; j < this->populationSize_; j++) {
      std::cout << "j = " << j << std::endl;
      if (j != initialPointIndex_(splittingIndex)) {
        x(splittingIndex) = x0_(splittingIndex, j);
        initListValues_.col(m_ - 1)(j) = this->optimisationProblem_->getObjectiveValue(x);
        this->numberOfIterations_++;
        if (initListValues_.col(m_ - 1)(j) < this->bestObjectiveValue_) {
          this->bestObjectiveValue_ = initListValues_.col(m_ - 1)(j);
          this->bestParameter_ = x;
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
        std::cout << "inner j = " << j << std::endl;
        nchild++;
        int level0 = 0;
        if (initListValues_.col(m_ - 1)(j) <= initListValues_.col(m_ - 1)(j + 1) || minimalLevel + 2 < boxDivisions_) {
          std::cout << "first if" << std::endl;
          nboxes_++;
          if (initListValues_.col(m_ - 1)(j) <= initListValues_.col(m_ - 1)(j+1)) {
            level0 = minimalLevel + 1;
          } else {
            level0 = minimalLevel + 2;
          }
          genBox(nboxes_ - 1, par, level0, -nchild, initListValues_.col(m_ - 1)(j));
          //index again so use 0, matlab=1
          std::cout << level_.n_elem << std::endl;
          std::cout << nboxes_ << std::endl;
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          std::cout << "first else" << std::endl;
          x(splittingIndex) = x0_(splittingIndex, j);
          nbasket_++;
          pointsInBasket_.col(nbasket_ - 1) = x;
          pointsInBasketValue_(nbasket_ - 1) = initListValues_.col(m_ - 1)(j);
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
          genBox(nboxes_ - 1, par, level0, -nchild, initListValues_.col(m_ - 1)(j + 1));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          std::cout << "second else" << std::endl;
          x(splittingIndex) = x0_(splittingIndex, j + 1);
          nbasket_++;
          pointsInBasket_.col(nbasket_ - 1) = x;
          pointsInBasketValue_(nbasket_ - 1) = initListValues_.col(m_ - 1)(j + 1);
        }
      }
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
        x(splittingIndex) = x0_(splittingIndex, j);
        nbasket_++;
        pointsInBasket_.col(nbasket_ - 1) = x;
        pointsInBasketValue_(nbasket_ - 1) = initListValues_.col(m_ - 1)(j);
      }
    }
    std::cout << "splitbyinitlist done" << std::endl;
    return this->isFinished() || this->isTerminated();
  }

  template<class DistanceFunction>
  bool MultilevelCoordinateSearch<DistanceFunction>::split(unsigned int splittingIndex, unsigned int minimalLevel, unsigned int par) {
    std::cout << "input sIndex = " << splittingIndex << ", minlvl = " << minimalLevel << ", par = " << par << std::endl;
    arma::Col<double> x = baseVertex_;
    arma::Col<double> z = z_.col(par);
    x(splittingIndex) = z(1);
    std::cout << "x(" << splittingIndex << ") = " << x(splittingIndex) << std::endl;
    boxBaseVertexFunctionValues_(1, par) = this->optimisationProblem_->getObjectiveValue(x);
    std::cout << "f(1," << par << ") = " << boxBaseVertexFunctionValues_(1, par) << std::endl;
    this->numberOfIterations_++;
    std::cout << "split init done" << std::endl;
    //index again so use 1, matlab=2
    if (boxBaseVertexFunctionValues_(1, par) < this->bestObjectiveValue_) {
      std::cout << "first if" << std::endl;
      //index again so use 1, matlab=2
      this->bestObjectiveValue_ = boxBaseVertexFunctionValues_(1, par);
      this->bestParameter_ = x;
      //Our stopping conditions are a little different
      if (this->isFinished() || this->isTerminated()) {
        return true;
      }
    }

    if (minimalLevel + 1 < boxDivisions_) {
      std::cout << "second if" << std::endl;
      //index again, all decremented by 1
      if (boxBaseVertexFunctionValues_(0, par) < boxBaseVertexFunctionValues_(1, par)) {
        std::cout << "first inner if" << std::endl;
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
          x(splittingIndex) = z(1);
          nbasket_ = nbasket_ + 1;
          pointsInBasket_.col(nbasket_ - 1) = x;
          //index again so use 1, matlab=2
          pointsInBasketValue_(nbasket_ - 1) = boxBaseVertexFunctionValues_(1, par);
        }

      } else {
        std::cout << "first inner else" << std::endl;
        if (minimalLevel + 2 < boxDivisions_) {
          nboxes_++;
          //index again so use 0, matlab=1
          genBox(nboxes_ - 1, par, minimalLevel + 2, 1, boxBaseVertexFunctionValues_(0, par));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          //index again so use 0, matlab=1
          x(splittingIndex) = z(0);
          nbasket_ = nbasket_ + 1;
          pointsInBasket_.col(nbasket_ - 1) = x;
          //index again so use 0, matlab=1
          pointsInBasketValue_(nbasket_ - 1) = boxBaseVertexFunctionValues_(0, par);
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
      if (z(1) != oppositeVertex_(splittingIndex)) {
        std::cout << "second inner if" << std::endl;
        //index again so use 1, matlab=2
        if (std::abs(z(1) - oppositeVertex_(splittingIndex)) > std::abs(z(1) - z(0))*(3 - std::sqrt(5))*0.5) {
          std::cout << "inner inner if" << std::endl;
          nboxes_++;
          //index again so use 1, matlab=2
          genBox(nboxes_ - 1, par, minimalLevel + 1, 3, boxBaseVertexFunctionValues_(1, par));
          //index again so use 0, matlab=1
          updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
        } else {
          std::cout << "inner inner else" << std::endl;
          if (minimalLevel + 2 < boxDivisions_) {
            std::cout << "inner inner inner if" << std::endl;
            nboxes_++;
            //index again so use 1, matlab=2
            genBox(nboxes_ - 1, par, minimalLevel + 2, 3, boxBaseVertexFunctionValues_(1, par));
            //index again so use 0, matlab=1
            updateRecord(nboxes_ - 1, level_(nboxes_ - 1), boxBaseVertexFunctionValues_.row(0).t());
          } else {
            std::cout << "inner inner inner else" << std::endl;
            //index again so use 1, matlab=2
            x(splittingIndex) = z(1);
            nbasket_ = nbasket_ + 1;
            pointsInBasket_.col(nbasket_ - 1) = x;
            //index again so use 1, matlab=2
            pointsInBasketValue_(nbasket_ - 1) = boxBaseVertexFunctionValues_(1, par);
          }
        }
      }
    } else {
      std::cout << "finishing else" << std::endl;
      //TODO: this is what is actually written in matlab. It makes zero sense to overwrite what was done before. May be an error in matlab?
      //index again so use 0, matlab=1
      x(splittingIndex) = z(0);
      nbasket_ = nbasket_ + 1;
      pointsInBasket_.col(nbasket_ - 1) = x;
      //index again so use 0, matlab=1
      pointsInBasketValue_(nbasket_ - 1) = boxBaseVertexFunctionValues_(0, par);

      //index again so use 1, matlab=2
      x(splittingIndex) = z(1);
      nbasket_ = nbasket_ + 1;
      pointsInBasket_.col(nbasket_ - 1) = x;
      //index again so use 1, matlab=2
      pointsInBasketValue_(nbasket_ - 1) = boxBaseVertexFunctionValues_(1, par);
      std::cout << "x(" << splittingIndex << ") = " << x(splittingIndex) << std::endl;
      std::cout << "nbasket = " << nbasket_ << std::endl;
      std::cout << "pointsinbasket.col(" << nbasket_ - 1 << ") = " << x << std::endl;
      std::cout << "pointsInBasketValue(" << nbasket_ - 1 << ") = " << boxBaseVertexFunctionValues_(1, par) << std::endl;
    }
    return this->isFinished() || this->isTerminated();
  }

  template<class DistanceFunction>
  arma::Col<double> MultilevelCoordinateSearch<DistanceFunction>::expectedGainOfSplit(unsigned int par, unsigned int numberOfDimensions, arma::Col<arma::uword> n0, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double> f1, arma::Col<double> f2) {
    double emin = arma::datum::inf;
    arma::Col<double> expectedGain = arma::Col<double>(numberOfDimensions);
    for (int i = 0; i < numberOfDimensions; i++) {
      std::cout << "i = " << i << std::endl;
      if (n0(i) == 0) {
        std::cout << "if taken" << std::endl;
        //expected gain for splitting according to the initialization list
        expectedGain(i) = arma::min(initListValues_.col(i)) - initListValues_(initialPointIndex_(i), i);
        std::cout << "e = " << arma::min(initListValues_.col(i)) - initListValues_(initialPointIndex_(i), i) << std::endl;
        if (expectedGain(i) < emin) {
          emin = expectedGain(i);
          isplit_(par) = i + 1;
          //index again so use 1, matlab=2
          z_(1, par) = arma::datum::inf;
        }
      } else {
        std::cout << "else taken" << std::endl;
        arma::Col<double> z1;
        z1 << baseVertex_(i) << x1(i) << x2(i) << arma::endr;
        std::cout << "z1 = " << z1 << std::endl;
        //index again at "boxBaseVertexFunctionValues_(0,par)" so use 0, matlab=1
        arma::Col<double> z2;
        z2 << 0 << f1(i) - boxBaseVertexFunctionValues_(0, par) << f2(i) - boxBaseVertexFunctionValues_(0, par) << arma::endr;
        std::cout << "z2 = " << z2 << std::endl;
        arma::Col<double> d = quadraticPolynomialInterpolation(z1, z2);
        std::cout << "d = " << d << std::endl;
        arma::Col<double> eta = subint(baseVertex_(i), oppositeVertex_(i));
        std::cout << "eta = " << eta << std::endl;
        //safeguard against splitting too close to x(i)
        double xi1 = arma::min(eta);
        std::cout << "xi1 = " << xi1 << std::endl;
        double xi2 = arma::max(eta);
        std::cout << "xi2 = " << xi2 << std::endl;
        double z = minimumQuadraticPolynomial(xi1, xi2, d, z1);
        std::cout << "z = " << z << std::endl;
        expectedGain(i) = quadraticPolynomial(z, d, z1);
        std::cout << "e = " << quadraticPolynomial(z, d, z1) << std::endl;
        if (expectedGain(i) < emin) {
          emin = expectedGain(i);
          isplit_(par) = i + 1;
          //index again so use 1, matlab=2
          z_(1, par) = z;
        }
      }
    }
    std::cout << "expectedgain = " << expectedGain << std::endl;
    return expectedGain;
  }

  template<class DistanceFunction>
  unsigned int MultilevelCoordinateSearch<DistanceFunction>::startSweep() {
    std::cout << "startsweep inside" << std::endl;
    record_ = arma::Col<arma::uword>(boxDivisions_ - 1, arma::fill::zeros);
    unsigned int s = boxDivisions_;
    std::cout << "s = " << s << std::endl;
    //std::cout << level_.subvec(0, 10) << std::endl;
    for (unsigned int i = 0; i < nboxes_; i++) {
      std::cout << "level(" << i << ") = " << level_(i) << std::endl;
      if (level_(i) > 0) {
        if (level_(i) < s) {
          s = level_(i);
          std::cout << "s = " << s << std::endl;
        }
        if (!record_(level_(i) - 1)) {
          record_(level_(i) - 1) = i + 1;
          std::cout << "if: record(level(" << i << ")-1) = " << record_(level_(i) - 1) << std::endl;
        } else if (boxBaseVertexFunctionValues_(0, i) < boxBaseVertexFunctionValues_(0, record_(level_(i) - 1) - 1)) {
          record_(level_(i) - 1) = i + 1;
          std::cout << "elseif: record(level(" << i << ")-1) = " << record_(level_(i) - 1) << std::endl;
        }
      }
    }
    std::cout << "startsweep done, s = " << s << std::endl;
    return s;
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vertex(unsigned int par, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2, arma::Col<arma::uword>& n0) {
    //for convenience
    unsigned int numberOfDimensions = this->optimisationProblem_->getNumberOfDimensions();
    std::cout << "starting vertex" << std::endl;

    baseVertex_ = arma::Col<double>(numberOfDimensions);
    baseVertex_.fill(arma::datum::inf);
    oppositeVertex_ = arma::Col<double>(numberOfDimensions);
    oppositeVertex_.fill(arma::datum::inf);

    double fold = boxBaseVertexFunctionValues_(0, par);
    unsigned int mVertex = par;
    while (mVertex > 0) {
      std::cout << "mVertex = " << mVertex << std::endl;
      std::cout << ipar_.subvec(0, 40).t() << std::endl;
      std::cout << isplit_.subvec(0, 40).t() << std::endl;
      unsigned int i = std::abs(std::abs(isplit_(ipar_(mVertex) - 1)) - 1);
      std::cout << "i = " << i << std::endl;
      n0(i) = n0(i) + 1;
      std::cout << "n0(" << i << ") = " << n0(i) << std::endl;
      std::cout << "ichild_(" << mVertex << ") = " << ichild_(mVertex) << std::endl;
      if (ichild_(mVertex) == 1) {
        std::cout << "baseVertex_(" << i << ") = " << baseVertex_(i) << std::endl;
        std::cout << "z_(0, " << ipar_(mVertex) - 1 << ") = " << z_(0, ipar_(mVertex) - 1) << std::endl;
        if (baseVertex_(i) == arma::datum::inf || baseVertex_(i) == z_(0, ipar_(mVertex) - 1)) {
          //matlab passes 2, but it's used as an index so we need to use 1
          vert1(i, 1, mVertex, baseVertex_, x1, x2, f1, f2);
        } else {
          updtf(numberOfDimensions, i, fold, x1, x2, f1, f2, boxBaseVertexFunctionValues_(0, ipar_(mVertex) - 1));
          fold = boxBaseVertexFunctionValues_(0, ipar_(mVertex) - 1);
          //matlab passes 1, but it's used as an index so we need to use 0
          vert2(i, 0, mVertex, baseVertex_, x1, x2, f1, f2);
        }
      } else if (ichild_(mVertex) >= 2) {
        updtf(numberOfDimensions, i, fold, x1, x2, f1, f2, boxBaseVertexFunctionValues_(0, ipar_(mVertex) - 1));
        fold = boxBaseVertexFunctionValues_(0, ipar_(mVertex) - 1);
        std::cout << "baseVertex_(" << i << ") = " << baseVertex_(i) << std::endl;
        std::cout << "z_(1, " << ipar_(mVertex) - 1 << ") = " << z_(1, ipar_(mVertex) - 1) << std::endl;
        if (baseVertex_(i) == arma::datum::inf || baseVertex_(i) == z_(1, ipar_(mVertex) - 1)) {
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
        std::cout << "last if taken" << std::endl;
        std::cout << "z0 = " << z_(0, ipar_(mVertex) - 1) << std::endl;
        std::cout << "z1 = " << z_(1, ipar_(mVertex) - 1) << std::endl;
        std::cout << "f0 = " << boxBaseVertexFunctionValues_(0, ipar_(mVertex) - 1) << std::endl;
        std::cout << "f1 = " << boxBaseVertexFunctionValues_(1, ipar_(mVertex) - 1) << std::endl;
        oppositeVertex_(i) = splitByGoldenSectionRule(z_(0, ipar_(mVertex) - 1), z_(1, ipar_(mVertex) - 1), boxBaseVertexFunctionValues_(0, ipar_(mVertex) - 1), boxBaseVertexFunctionValues_(1, ipar_(mVertex) - 1));
        std::cout << "oppositeVertex(" << i << ") = " << oppositeVertex_(i) << std::endl;
      }
      std::cout << "x1" << x1 << std::endl;
      std::cout << "x2" << x2 << std::endl;
      std::cout << "f1" << f1 << std::endl;
      std::cout << "f2" << f2 << std::endl;
      std::cout << "------------" << std::endl;
      //box m was generated by splitting according to the init. list
      if (ichild_(mVertex) < 0) {
        int j1 = 0;//has matlab-1 value
        int j2 = 0;//has matlab value
        int j3 = 0;//has matlab
        int k = 0;//has matlab-1 value
        if (boundaries_.col(0)(i) < x0_(i, 0)) {
          std::cout << "if taken" << std::endl;
          j1 = std::ceil(std::abs(ichild_(mVertex)) / 2.0);
          j2 = std::floor(std::abs(ichild_(mVertex)) / 2.0);
          if ((std::abs(ichild_(mVertex)) / 2.0 < j1 && j1 > 1) || j1 == this->populationSize_) {
            std::cout << "inner if taken" << std::endl;
            j3 = -1;
          } else {
            std::cout << "inner else taken" << std::endl;
            j3 = 1;
          }
        } else {
          std::cout << "else taken" << std::endl;
          j1 = std::floor(std::abs(ichild_(mVertex)) / 2.0) + 1;
          j2 = std::ceil(std::abs(ichild_(mVertex)) / 2.0);
          std::cout << "j1 = " << j1 << std::endl;
          std::cout << "j2 = " << j2 << std::endl;
          std::cout << "compareval = " << std::abs(ichild_(mVertex)) / 2.0 + 1 << std::endl;
          if ((std::abs(ichild_(mVertex)) / 2.0 + 1) > j1 && j1 < this->populationSize_) {
            std::cout << "inner if taken" << std::endl;
            j3 = 1;
          } else {
            std::cout << "inner else taken" << std::endl;
            j3 = -1;
          }
        }
        //box m was generated in the init. procedure
        if (isplit_(ipar_(mVertex) - 1) < 0) {
          std::cout << "k if taken" << std::endl;
          k = i;
          //box m was generated by a later split according to the init.list
          //k points to the corresponding function values  
        } else {
          std::cout << "k else taken" << std::endl;
          //matlab passes 2, but it's used as an index so we need to use 1
          //have to -1 so it stays at matlab-1 value
          k = z_(1, ipar_(mVertex) - 1)-1;
        }
        j1--;
        std::cout << "j1  = " << j1 << std::endl;
        std::cout << "j2  = " << j2 << std::endl;
        std::cout << "j3  = " << j3 << std::endl;
        std::cout << "k  = " << k << std::endl;

        if (j1 != initialPointIndex_(i) || (baseVertex_(i) != arma::datum::inf && baseVertex_(i) != x0_(i, (initialPointIndex_(i))))) {
          updtf(numberOfDimensions, i, fold, x1, x2, f1, f2, initListValues_(initialPointIndex_(i), k));
          fold = initListValues_(initialPointIndex_(i), k);
        }
        if (baseVertex_(i) == arma::datum::inf || baseVertex_(i) == x0_(i, j1)) {
          std::cout << "if is if" << std::endl;
          baseVertex_(i) = x0_(i, j1);
          if (x1(i) == arma::datum::inf) {
            std::cout << "first if" << std::endl;
            std::cout << "x1" << x1 << std::endl;
            vert3(i, j1, k, x1, x2, f1, f2);
            std::cout << "x1" << x1 << std::endl;
          } else if (x2(i) == arma::datum::inf && x1(i) != x0_(i, j1 + j3)) {
            std::cout << "second if" << std::endl;
            x2(i) = x0_(i, j1 + j3);
            f2(i) = f2(i) + initListValues_(j1 + j3, k);
          } else if (x2(i) == arma::datum::inf) {
            std::cout << "third if" << std::endl;
            //matlab checks for 1, since it's index use 0
            if (j1 != 0 && j1 != this->populationSize_ - 1) {
              std::cout << "inner if" << std::endl;
              x2(i) = x0_(i, j1 - j3);
              f2(i) = f2(i) + initListValues_(j1 - j3, k);
            } else {
              std::cout << "inner else" << std::endl;
              x2(i) = x0_(i, j1 + 2 * j3);
              f2(i) = f2(i) + initListValues_(j1 + 2 * j3, k);
            }
          }
        } else {
          std::cout << "else is else" << std::endl;
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
              if (j1 != 0 && j1 != this->populationSize_ - 1) {
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
          if (j2 == 0) {
            oppositeVertex_(i) = boundaries_.col(0)(i);
            std::cout << "1oppositeVertex(" << i << ") = " << oppositeVertex_(i) << std::endl;
          } else if (j2 == this->populationSize_) {
            oppositeVertex_(i) = boundaries_.col(1)(i);
            std::cout << "2oppositeVertex(" << i << ") = " << oppositeVertex_(i) << std::endl;
          } else {
            //added -1 to all j2 and k since they have matlab value.
            oppositeVertex_(i) = splitByGoldenSectionRule(x0_(i, j2 - 1), x0_(i, j2 + 1 - 1), initListValues_(j2 - 1, k), initListValues_(j2 + 1 - 1, k));
            std::cout << "3oppositeVertex(" << i << ") = " << oppositeVertex_(i) << std::endl;
          }
        }
      }
      mVertex = ipar_(mVertex) - 1;
      std::cout << "new mvertex = " << mVertex << std::endl;
    }
    std::cout << "finishing loop" << std::endl;
    std::cout << "x1" << x1 << std::endl;
    std::cout << "x2" << x2 << std::endl;
    std::cout << "f1" << f1 << std::endl;
    std::cout << "f2" << f2 << std::endl;
    std::cout << "-------------" << std::endl;
    for (int i = 0; i < numberOfDimensions; i++) {
      std::cout << "i = " << i << std::endl;
      if (baseVertex_(i) == arma::datum::inf) {
        baseVertex_(i) = x0_(i, initialPointIndex_(i));
        vert3(i, initialPointIndex_(i), i, x1, x2, f1, f2);
      }
      if (oppositeVertex_(i) == arma::datum::inf) {
        oppositeVertex_(i) = originalOppositeVertex_(i);
        std::cout << "oppositeVertex(" << i << ") = " << oppositeVertex_(i) << std::endl;
      }
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vert1(int updateIndex, unsigned int j, unsigned int m, arma::Col<double>& x, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2) {
    int j1 = 0;
    //matlab checks for 1, since it's index use 0
    if (j == 0) {
      //also index again
      j1 = 1;
    } else {
      j1 = 0;
    }
    x(updateIndex) = z_(j1, ipar_(m) - 1);
    if (x1(updateIndex) == arma::datum::inf) {
      x1(updateIndex) = z_(j, ipar_(m) - 1);
      f1(updateIndex) = f1(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m) - 1);
    } else if (x2(updateIndex) == arma::datum::inf && x1(updateIndex) != z_(j, ipar_(m) - 1)) {
      x2(updateIndex) = z_(j, ipar_(m) - 1);
      f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m) - 1);
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vert2(int updateIndex, unsigned int j, unsigned int m, arma::Col<double> x, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2) {
    int j1 = 0;
    //matlab checks for 1, since it's index use 0
    if (j == 0) {
      //also index again
      j1 = 1;
    } else {
      j1 = 0;
    }
    if (x1(updateIndex) == arma::datum::inf) {
      x1(updateIndex) = z_(j, ipar_(m) - 1);
      f1(updateIndex) = f1(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m) - 1);
      if (x(updateIndex) != z_(j1, ipar_(m) - 1)) {
        x2(updateIndex) = z_(j1, ipar_(m) - 1);
        f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j1, ipar_(m) - 1);
      }
    } else if (x2(updateIndex) == arma::datum::inf && x1(updateIndex) != z_(j, ipar_(m) - 1)) {
      x2(updateIndex) = z_(j, ipar_(m) - 1);
      f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j, ipar_(m) - 1);
    } else if (x2(updateIndex) == arma::datum::inf) {
      x2(updateIndex) = z_(j1, ipar_(m) - 1);
      f2(updateIndex) = f2(updateIndex) + boxBaseVertexFunctionValues_(j1, ipar_(m) - 1);
    }
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::vert3(int updateIndex, unsigned int j, const arma::sword& f0columnIndex, arma::Col<double>& x1, arma::Col<double>& x2, arma::Col<double>& f1, arma::Col<double>& f2) {
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
    f1(updateIndex) = f1(updateIndex) + initListValues_(k1, f0columnIndex);
    f2(updateIndex) = f2(updateIndex) + initListValues_(k2, f0columnIndex);
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::updtf(unsigned int numberOfDimensions, unsigned int splittingIndex, double fold, arma::Col<double> x1, arma::Col<double> x2, arma::Col<double>& f1, arma::Col<double>& f2, double baseVertexValueCurrentBox) {
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
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    double x2 = y;
    int f = 1000;
    if (f * std::abs(x) < 1) {
      if (std::abs(y) > f) {
        x2 = std::copysign(1.0, y);
      }
    } else {
      if (std::abs(y) > f * std::abs(x)) {
        //c++ standardlibraries have no signum. using copysign instead...
        //original matlab: x2 = 10.*sign(y)*abs(x);
        x2 = 10 * std::copysign(std::abs(x), y);
      }
    }
    arma::Col<double> retVector(2);
    retVector(0) = x + (x2 - x) / 10.0;
    retVector(1) = x2;
    return retVector;
  }

  template<class DistanceFunction>
  void MultilevelCoordinateSearch<DistanceFunction>::updateRecord(unsigned int label, int level, arma::Col<double> f) {
    std::cout << "inside updaterecord, label = " << label << ", level = " << level << std::endl;
    if (record_.n_elem < level) {
      std::cout << "first if" << std::endl;
      record_(level - 1) = label + 1;
    } else if (record_(level - 1) == 0) {
      std::cout << "second if" << std::endl;
      record_(level - 1) = label + 1;
    } else if (f(label) < f(record_(level - 1) - 1)) {
      std::cout << "third if" << std::endl;
      record_(level - 1) = label + 1;
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
  void MultilevelCoordinateSearch<DistanceFunction>::addLocation(arma::Col<double>& loc) {
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
  bool MultilevelCoordinateSearch<DistanceFunction>::pointInsideDomainOfAttraction(arma::Col<double>& loc, std::shared_ptr<double> valueAtLoc, double nbasket) {
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
        arma::Col<double> p = pointsInBasket_.col(i) - loc;
        arma::Col<double> y1 = loc + (1 / 3.0) * p;
        double f1 = this->optimisationProblem_->getObjectiveValue(y1);
        this->numberOfIterations_++;
        if (f1 <= *valueAtLoc) {
          arma::Col<double> y2 = loc + (2 / 3.0) * p;
          double f2 = this->optimisationProblem_->getObjectiveValue(y2);
          this->numberOfIterations_++;
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

