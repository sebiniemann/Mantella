// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <limits>
#include <map>
#include <algorithm>
#include <array>

// Mantella
#include <mantella>

TEST_CASE("OptimisationProblem") {
  // Some tests require at least a second dimensions, to differentiate between both. 
  arma::uword numberOfDimensions = SYNCRONISED(1 + getDiscreteRandomNumber());
  CAPTURE(numberOfDimensions);
  
  mant::OptimisationProblem optimisationProblem(numberOfDimensions);

  SECTION("Initialises the lower/upper bounds with the expected default values.") {
    IS_EQUAL(optimisationProblem.getLowerBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) - 10);
    IS_EQUAL(optimisationProblem.getUpperBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) + 10);
  }

  SECTION(".numberOfDimensions_") {
    SECTION("Is parametrisable by the constructor.") {
      CHECK(optimisationProblem.numberOfDimensions_ == numberOfDimensions);
    }
  }

  SECTION(".setObjectiveFunction") {
    auto objectiveFunction = [] (
        const arma::Col<double>& parameter) {
      return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
    };
    
    const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
    CAPTURE(parameter);
      
    SECTION("Updates the objective function, without specifying a name.") {
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter)));
    }

    SECTION("Updates the objective function, and names it.") {
      optimisationProblem.setObjectiveFunction(objectiveFunction, "My Optimisation Problem");
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter)));
      CHECK(optimisationProblem.getObjectiveFunctionName() == "My Optimisation Problem");
    }
    
    SECTION("Resets the cache.") {
      mant::isCachingSamples = true;
      
      // Populates the cache
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      optimisationProblem.getObjectiveValue(parameter);
      
      // Uses *parameter* to *anotherParameter* to avoid shadowing.
      auto differentObjectiveFunction = [] (
          const arma::Col<double>& anotherParameter) {
        // Return an objective value different from *objectiveFunction*.
        return arma::accu(anotherParameter % mant::range<double>(1, anotherParameter.n_elem)) + 1;
      };
      optimisationProblem.setObjectiveFunction(differentObjectiveFunction);
      // The evaluation counters should also be reset.
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(differentObjectiveFunction(parameter)));
    }
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveFunction(nullptr), std::logic_error);
      }
    }
  }

  SECTION(".getObjectiveValue") {
    auto objectiveFunction = [] (
        const arma::Col<double>& parameter) {
      return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
    };
    optimisationProblem.setObjectiveFunction(objectiveFunction);
      
    const arma::uword numberOfParameters = getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
    // Duplicated all parameters and shuffle them afterwards, to observe if it works as expected with/without caching.
    parameters = arma::shuffle(arma::join_rows(parameters, parameters));
    CAPTURE(parameters);
    
    SECTION("Works with caching enabled.") {
      mant::isCachingSamples = true;

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter)));
      }
    }
    
    SECTION("Works with caching disabled.") {
      mant::isCachingSamples = false; 

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter)));
      }
    }
    
    // We test the effectiveness and order of all parameter space and objective value space modifications at this point, to avoid repetitive checks for each space modification setter.
    SECTION("Is adjusted by the parameter space and objective value space modifiers (including the correct order).") {
      // Parameter space modifiers
      arma::Col<arma::uword> parameterPermutation = SYNCRONISED(mant::randomPermutationVector(numberOfDimensions));
      optimisationProblem.setParameterPermutation(parameterPermutation);
      CAPTURE(parameterPermutation);
      
      arma::Col<double> parameterScaling = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      optimisationProblem.setParameterScaling(parameterScaling);
      CAPTURE(parameterScaling);
      
      arma::Col<double> parameterTranslation = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      optimisationProblem.setParameterTranslation(parameterTranslation);
      CAPTURE(parameterTranslation);
      
      arma::Mat<double> parameterRotation = SYNCRONISED(mant::randomRotationMatrix(numberOfDimensions));
      optimisationProblem.setParameterRotation(parameterRotation);
      CAPTURE(parameterRotation);
      
      // Objective value space modifiers
      double objectiveValueScaling = SYNCRONISED(getContinuousRandomNumber());
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
      CAPTURE(objectiveValueScaling);
      
      double objectiveValueTranslation = SYNCRONISED(getContinuousRandomNumber());
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      CAPTURE(objectiveValueTranslation);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
      }
    }

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the number of elements is unequal to than the problem dimension.") {
        const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
        CAPTURE(numberOfDimensions);
        const arma::Col<double> parameter = getContinuousRandomNumbers(differentNumberOfDimensions);
        CAPTURE(parameter);
        
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
      }
    }
  }
  
  SECTION(".getObjectiveFunctionName") {
    // This is already covered by *SECTION(".setObjectiveFunction")*.
  }

  SECTION(".setLowerBounds") {
    SECTION("Updates the lower bounds.") {
      const arma::Col<double>& lowerBounds = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      CAPTURE(lowerBounds);
      
      optimisationProblem.setLowerBounds(lowerBounds);
      
      IS_EQUAL(optimisationProblem.getLowerBounds(), lowerBounds);
    }
    
    SECTION("Does not reset the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const arma::Col<double>& lowerBounds = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(lowerBounds);
      optimisationProblem.setLowerBounds(lowerBounds);
    
      CHECK(optimisationProblem.getCachedSamples().size() != 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() != 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() != 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      arma::Col<double> lowerBounds = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(lowerBounds);
      
      optimisationProblem.setLowerBounds(lowerBounds);
      
      IS_EQUAL(optimisationProblem.getLowerBounds(), SYNCRONISED(lowerBounds));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the number of bounds is unequal to the problem dimension.") {
        const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
        CAPTURE(numberOfDimensions);
        const arma::Col<double>& lowerBounds = getContinuousRandomNumbers(differentNumberOfDimensions);
        CAPTURE(lowerBounds);
        
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }
      
      SECTION("Throws an exception, if any bound is infinite.") {
        arma::Col<double> lowerBounds = getContinuousRandomNumbers(numberOfDimensions);
        lowerBounds(0) = arma::datum::inf;
        CAPTURE(lowerBounds);
        
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }
    }
  }
  
  SECTION(".getLowerBounds") {
    // This is already covered by *SECTION(".setLowerBounds")*.
  }

  SECTION(".setUpperBounds") {
    SECTION("Updates the upper bounds.") {
      const arma::Col<double>& upperBounds = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      CAPTURE(upperBounds);
      
      optimisationProblem.setUpperBounds(upperBounds);
      
      IS_EQUAL(optimisationProblem.getUpperBounds(), upperBounds);
    }
    
    SECTION("Does not reset the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const arma::Col<double>& upperBounds = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(upperBounds);
      optimisationProblem.setUpperBounds(upperBounds);
    
      CHECK(optimisationProblem.getCachedSamples().size() != 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() != 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() != 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      arma::Col<double> upperBounds = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(upperBounds);
      
      optimisationProblem.setUpperBounds(upperBounds);
      
      IS_EQUAL(optimisationProblem.getUpperBounds(), SYNCRONISED(upperBounds));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the number of bounds is unequal to the problem dimension.") {
        const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
        CAPTURE(numberOfDimensions);
        const arma::Col<double>& upperBounds = getContinuousRandomNumbers(differentNumberOfDimensions);
        CAPTURE(upperBounds);
        
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }
      
      SECTION("Throws an exception, if any bound is infinite.") {
        arma::Col<double> upperBounds = getContinuousRandomNumbers(numberOfDimensions);
        upperBounds(0) = arma::datum::inf;
        CAPTURE(upperBounds);
        
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }
    }
  }
  
  SECTION(".getUpperBounds") {
    // This is already covered by *SECTION(".setUpperBounds")*.
  }
  
  SECTION(".setParameterPermutation") {
    // The influence on *.getObjectiveValue* is already tested in *SECTION(".getObjectiveValue")*.
    
    SECTION("Resets the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const arma::Col<arma::uword>& parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
      CAPTURE(parameterPermutation);
      optimisationProblem.setParameterPermutation(parameterPermutation);
    
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      arma::Col<arma::uword> parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
      CAPTURE(parameterPermutation);
      
      optimisationProblem.setParameterPermutation(parameterPermutation);
    
      // Instead of checking the internal class field, we want to observe the expected modification on the parameter space.
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      
      const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter.elem(SYNCRONISED(parameterPermutation)))));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the provided vector is not a compatible permutation matrix.") {
        arma::Col<arma::uword> parameterPermutation = mant::randomPermutationVector(numberOfDimensions);
        CAPTURE(parameterPermutation);
        
        SECTION("Throws an exception, if the permutation matrix's size is unequal to the number of dimensions.") {
          parameterPermutation.resize(numberOfDimensions - 1);
          CAPTURE(parameterPermutation);

          CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
        }

        SECTION("Throws an exception, if any element of the permutation matrix is not within [0, numberOfDimensions - 1].") {
          parameterPermutation(0) = numberOfDimensions;
          CAPTURE(parameterPermutation);

          CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
        }

        SECTION("Throws an exception, if the elements in the permutation matrix are not unique.") {
          parameterPermutation(0) = parameterPermutation(1);
          CAPTURE(parameterPermutation);

          CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
        }
      }
    }
  }
  
  SECTION(".setParameterScaling") {
    // The influence on *.getObjectiveValue* is already tested in *SECTION(".getObjectiveValue")*.
    
    SECTION("Resets the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const arma::Col<double>& parameterScaling = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameterScaling);
      optimisationProblem.setParameterScaling(parameterScaling);
    
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      arma::Col<double> parameterScaling = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameterScaling);
      
      optimisationProblem.setParameterScaling(parameterScaling);
      
      // Instead of checking the internal class field, we want to observe the expected modification on the parameter space.
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      
      const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(SYNCRONISED(parameterScaling) % parameter)));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the number of elements is unequal to the problem dimension.") {
        const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
        CAPTURE(numberOfDimensions);
        const arma::Col<double>& parameterScaling = getContinuousRandomNumbers(differentNumberOfDimensions);
        CAPTURE(parameterScaling);

        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }
      
      SECTION("Throws an exception, if any parameter is infinite.") {
        arma::Col<double> parameterScaling = getContinuousRandomNumbers(numberOfDimensions);
        parameterScaling(0) = arma::datum::inf;
        CAPTURE(parameterScaling);

        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }
    }
  }
  
  SECTION(".setParameterTranslation") {
    // The influence on *.getObjectiveValue* is already tested in *SECTION(".getObjectiveValue")*.
        
    SECTION("Resets the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const arma::Col<double>& parameterTranslation = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameterTranslation);
      optimisationProblem.setParameterTranslation(parameterTranslation);
    
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      arma::Col<double> parameterTranslation = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameterTranslation);
      
      optimisationProblem.setParameterTranslation(parameterTranslation);
      
      // Instead of checking the internal class field, we want to observe the expected modification on the parameter space.
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      
      const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter - SYNCRONISED(parameterTranslation))));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the number of elements is unequal to the problem dimension.") {
        const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
        CAPTURE(numberOfDimensions);
        const arma::Col<double>& parameterTranslation = getContinuousRandomNumbers(differentNumberOfDimensions);
        CAPTURE(parameterTranslation);

        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }
      
      SECTION("Throws an exception, if any parameter is infinite.") {
        arma::Col<double> parameterTranslation = getContinuousRandomNumbers(numberOfDimensions);
        parameterTranslation(0) = arma::datum::inf;
        CAPTURE(parameterTranslation);

        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }
    }
  }
  
  SECTION(".setParameterRotation") {
    // The influence on *.getObjectiveValue* is already tested in *SECTION(".getObjectiveValue")*.
    
    SECTION("Resets the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const arma::Mat<double>& parameterRotation = mant::randomRotationMatrix(numberOfDimensions);
      CAPTURE(parameterRotation);
      optimisationProblem.setParameterRotation(parameterRotation);
    
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      arma::Mat<double> parameterRotation = mant::randomRotationMatrix(numberOfDimensions);
      CAPTURE(parameterRotation);
      
      optimisationProblem.setParameterRotation(parameterRotation);
      
      // Instead of checking the internal class field, we want to observe the expected modification on the parameter space.
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      
      const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(SYNCRONISED(parameterRotation) * parameter)));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the provided matrix is not a compatible rotation matrix.") {
      arma::Mat<double> parameterRotation = mant::randomRotationMatrix(numberOfDimensions);
        CAPTURE(parameterRotation);
        
        SECTION("Throws an exception, if the provided matrix's number of rows is unequal to the problem dimensions.") {
          parameterRotation.resize(parameterRotation.n_rows - 1, parameterRotation.n_cols - 1);
          CAPTURE(parameterRotation);

          CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
        }
      
        SECTION("Throws an exception, if the provided matrix is not square.") {
          parameterRotation.shed_col(0);
          CAPTURE(parameterRotation);

          CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
        }

        SECTION("Throws an exception, if the provided matrix has not an determinant of (nearly) -1 or 1.") {
          // Increasing any element of a orthonormal rotation matrix by 1 should result in a determinant unequal to 1 or -1.
          parameterRotation(0, 0) += 1;
          CAPTURE(parameterRotation);

          CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
        }

        SECTION("Throws an exception, if its transpose is not (nearly) equal to its inverse.") {
          // Increasing any element of a orthonormal rotation matrix by 1 should result in an inequality transpose and inverse.
          parameterRotation(0, parameterRotation.n_cols - 1) += 1;
          CAPTURE(parameterRotation);

          CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
        }
      }
    }
  }
  
  SECTION(".setObjectiveValueScaling") {
    // The influence on *.getObjectiveValue* is already tested in *SECTION(".getObjectiveValue")*.
        
    SECTION("Resets the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const double objectiveValueScaling = getContinuousRandomNumber();
      CAPTURE(objectiveValueScaling);
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
    
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      double objectiveValueScaling = getContinuousRandomNumber();
      CAPTURE(objectiveValueScaling);
      
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
      
      // Instead of checking the internal class field, we want to observe the expected modification on the parameter space.
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      
      const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(SYNCRONISED(objectiveValueScaling) * objectiveFunction(parameter)));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the parameter is infinite.") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveValueScaling(arma::datum::inf), std::logic_error);
      }
    }
  }
  
  SECTION(".setObjectiveValueTranslation") {
    // The influence on *.getObjectiveValue* is already tested in *SECTION(".getObjectiveValue")*.
    
    SECTION("Resets the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::isCachingSamples = true;
      
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const double objectiveValueTranslation = getContinuousRandomNumber();
      CAPTURE(objectiveValueTranslation);
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
    
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      double objectiveValueTranslation = getContinuousRandomNumber();
      CAPTURE(objectiveValueTranslation);
      
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      
      // Instead of checking the internal class field, we want to observe the expected modification on the parameter space.
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      
      const arma::Col<double> parameter = getContinuousRandomNumbers(numberOfDimensions);
      CAPTURE(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveFunction(parameter) + SYNCRONISED(objectiveValueTranslation)));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the parameter is infinite.") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveValueTranslation(arma::datum::inf), std::logic_error);
      }
    }
  }
  
  SECTION(".getCachedSamples") {
    SECTION("Returns all (unique) samples.") {
      mant::isCachingSamples = true;
      
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> expectedSamples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double> parameter = parameters.col(n);
        expectedSamples.insert({parameter, objectiveFunction(parameter)});
      }
      
      HAS_SAME_SAMPLES(optimisationProblem.getCachedSamples(), expectedSamples);
    }
    
    SECTION("Is empty, if the caching is disabled.") {
      mant::isCachingSamples = false;
      
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
    }
  }
  
  SECTION(".getNumberOfEvaluations") {
    SECTION("Returns the number of (all) function evaluations, including the duplicated ones.") {
      mant::isCachingSamples = true;
      
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Initialises the cache.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }

      // Performs duplicated function evaluations.
      for (const auto n : arma::randi<arma::Col<arma::uword>>(2 * parameters.n_cols, arma::distr_param(0, static_cast<int>(parameters.n_cols) - 1))) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }

      CHECK(optimisationProblem.getNumberOfEvaluations() == 3 * parameters.n_cols);
    }
  }
  
  SECTION(".getNumberOfDistinctEvaluations") {
    SECTION("Returns only the distinct (unique) number of function evaluations.") {
      mant::isCachingSamples = true;
      
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Initialises the cache.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }

      // Performs duplicated function evaluations.
      for (const auto n : arma::randi<arma::Col<arma::uword>>(2 * parameters.n_cols, arma::distr_param(0, static_cast<int>(parameters.n_cols) - 1))) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }

      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == parameters.n_cols);
    }
    
    SECTION("Returns the number of (all) function evaluations, including the duplicated ones, if caching is deactivated.") {
      mant::isCachingSamples = false;
      
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Initialises the cache.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }

      // Performs duplicated function evaluations.
      for (const auto n : arma::randi<arma::Col<arma::uword>>(2 * parameters.n_cols, arma::distr_param(0, static_cast<int>(parameters.n_cols) - 1))) {
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }

      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 3 * parameters.n_cols);
    }
  }
  
  SECTION(".reset") {
    SECTION("Resets only the cache and counters.") {
      // Objective
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
      // Constraints
      const arma::Col<double>& lowerBounds = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      CAPTURE(lowerBounds);
      optimisationProblem.setLowerBounds(lowerBounds);
      const arma::Col<double>& upperBounds = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      CAPTURE(upperBounds);
      optimisationProblem.setUpperBounds(upperBounds);
      // Parameter space modifiers
      const arma::Col<arma::uword>& parameterPermutation = SYNCRONISED(mant::randomPermutationVector(numberOfDimensions));
      CAPTURE(parameterPermutation);
      optimisationProblem.setParameterPermutation(parameterPermutation);
      const arma::Col<double>& parameterScaling = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      CAPTURE(parameterScaling);
      optimisationProblem.setParameterScaling(parameterScaling);
      const arma::Col<double>& parameterTranslation = SYNCRONISED(getContinuousRandomNumbers(numberOfDimensions));
      CAPTURE(parameterTranslation);
      optimisationProblem.setParameterTranslation(parameterTranslation);
      const arma::Mat<double>& parameterRotation = SYNCRONISED(mant::randomRotationMatrix(numberOfDimensions));
      CAPTURE(parameterRotation);
      optimisationProblem.setParameterRotation(parameterRotation);
      // Objective value space modifiers
      const double& objectiveValueScaling = SYNCRONISED(getContinuousRandomNumber());
      CAPTURE(objectiveValueScaling);
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
      const double& objectiveValueTranslation = SYNCRONISED(getContinuousRandomNumber());
      CAPTURE(objectiveValueTranslation);
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      
      optimisationProblem.reset();
      
      // We check the cache and counters first, as the code below populates the cache and increases the counters again.
      CHECK(optimisationProblem.getCachedSamples().size() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
      
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == Approx(objectiveValueScaling * objectiveFunction(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation)) + objectiveValueTranslation));
      }
    }
  }
}
