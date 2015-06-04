// Catch
#include <catch.hpp>
#include "helper.hpp"

// C++ Standard Library
#include <limits>
#include <map>
#include <algorithm>

// Mantella
#include <mantella>

TEST_CASE("OptimisationProblem<T> (general case)") {
  class DummyOptimisationProblem : public mant::OptimisationProblem<double> {
    public:
      using mant::OptimisationProblem<double>::OptimisationProblem;

      std::string toString() const noexcept override {
        return "test_optimisation_problem";
      }

      double getDefaultSoftConstraintsValue(
          const arma::Col<double>& parameter) const noexcept {
        return mant::OptimisationProblem<double>::getSoftConstraintsValueImplementation(parameter);
      }

      double getSoftConstraintsValueImplementation(
          const arma::Col<double>& parameter) const noexcept override {
        // All parameters with more negative than positive values fulfil the soft-constraint.
        if(arma::accu(parameter < 0.0) <= std::floor(numberOfDimensions_ / 2.0)) {
          return 1.0;
        } else {
          return 0.0;
        }
      }
    
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const noexcept override {
        // The objective value is a weighted sum of the parameter values.
        return arma::accu(parameter % arma::linspace<arma::Col<double>>(1, numberOfDimensions_, numberOfDimensions_));
      }
  };

  std::size_t numberOfDimensions = 5;
  DummyOptimisationProblem optimisationProblem(numberOfDimensions);
  
  SECTION("OptimisationProblem.numberOfDimensions_.") {
    SECTION("Checking the parametrisation by the constructor.") {
      CHECK(optimisationProblem.numberOfDimensions_ == numberOfDimensions);
    }
  }
  
  SECTION("OptimisationProblem.getLowerBounds()") {
    SECTION("Checking the default value.") {
      // Each dimension of the lower bound is set to the lowest, representable value.
      arma::Col<double> expected = arma::zeros<arma::Col<double>>(numberOfDimensions) + std::numeric_limits<double>::lowest();
      compare(optimisationProblem.getLowerBounds(), expected);
    }
  }

  SECTION("OptimisationProblem.getUpperBounds()") {
    SECTION("Checking the default value.") {
      // Each dimension of the lower bound is set to the largest, representable value.
      arma::Col<double> expected = arma::zeros<arma::Col<double>>(numberOfDimensions) + std::numeric_limits<double>::max();
      compare(optimisationProblem.getUpperBounds(), expected);
    }
  }

  SECTION("OptimisationProblem.setLowerBounds(...)") {
    SECTION("Checking the parametrisation by .setLowerBounds(...)") {
      arma::Col<double> lowerBounds = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;

      optimisationProblem.setLowerBounds(lowerBounds);
      compare(optimisationProblem.getLowerBounds(), lowerBounds);
    }
    
    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements of the lower bound is greater than the problem dimension.") {
        arma::Col<double> lowerBounds(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements of the lower bound is lower than the problem dimension.") {
        arma::Col<double> lowerBounds(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.setLowerBounds(lowerBounds), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.setUpperBounds(...)") {
    SECTION("Checking the parametrisation by .setUpperBounds(...)") {      
      arma::Col<double> upperBounds = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;

      optimisationProblem.setUpperBounds(upperBounds);
      compare(optimisationProblem.getUpperBounds(), upperBounds);
    }

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements of the upper bound is greater than the problem dimension.") {
        arma::Col<double> upperBounds(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements of the upper bound is lower than the problem dimension.") {
        arma::Col<double> upperBounds(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.setUpperBounds(upperBounds), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.getAcceptableObjectiveValue()") {
    SECTION("Checking the default value.") {
      CHECK(optimisationProblem.getAcceptableObjectiveValue() == std::numeric_limits<double>::lowest());
    }
  }

  SECTION("OptimisationProblem.setAcceptableObjectiveValue(...)") {
    SECTION("Checking the parametrisation of the acceptable objective value.") {
      std::array<double, 3> expecteds = {{
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator())
      }};
      
      for(const auto expected : expecteds) {
        optimisationProblem.setAcceptableObjectiveValue(expected);
        CHECK(optimisationProblem.getAcceptableObjectiveValue() == expected);
      }
    }
  }

  SECTION("OptimisationProblem.getCachedObjectiveValues()") {
    SECTION("Checking the cached mapping of parameters to objective values..") {
      arma::Mat<double> parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, 4) * 200.0 - 100.0;
    
      // Generate all expected results and populate the cache
      std::unordered_map<arma::Col<double>, double, mant::Hash<double>, mant::IsEqual<double>> expected;
      for (std::size_t n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        expected.insert({parameter, optimisationProblem.getObjectiveValue(parameter)});
      }
      
      // Call *.getObjectiveValue(...)* multiple times to ensure correct duplication handling.
      for (std::size_t n = 0; n < 2 * parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(std::uniform_int_distribution<std::size_t>(0, parameters.n_cols - 1)(mant::Rng::getGenerator())));
      }
      
      // Compare both unordered maps
      std::unordered_map<arma::Col<double>, double, mant::Hash<double>, mant::IsEqual<double>> actual = optimisationProblem.getCachedObjectiveValues();
      
      CHECK(actual.size() == expected.size());
      for (const auto& expectedPair : expected) {
        const auto n = actual.find(expectedPair.first);
        
        CHECK(n != actual.cend()); // Found *expectedPair* within *actual*.
        CHECK(n->second == expectedPair.second);
      }
    }
  }

  SECTION("OptimisationProblem.getNumberOfDistinctEvaluations(...)") {
    SECTION("Checking the number of distinct, i.e. unique objective function evaluations.") {
      arma::Mat<double> parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, 4) * 200.0 - 100.0;
    
      // Populate the cache
      for (std::size_t n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        optimisationProblem.getObjectiveValue(parameter);
      }
      
      // Call *.getObjectiveValue(...)* multiple times to ensure correct duplication handling.
      for (std::size_t n = 0; n < 2 * parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(std::uniform_int_distribution<std::size_t>(0, parameters.n_cols - 1)(mant::Rng::getGenerator())));
      }
      
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == parameters.n_cols);
    }
  }

  SECTION("OptimisationProblem.getNumberOfEvaluations(...)") {
    SECTION("Checking the number of ALL objective function evaluations.") {
      // Generate some 
      arma::Mat<double> parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, 4) * 200.0 - 100.0;
    
      // Call *.getObjectiveValue(...)* multiple times.
      for (std::size_t n = 0; n < 3 * parameters.n_cols; ++n) {
        optimisationProblem.getObjectiveValue(parameters.col(std::uniform_int_distribution<std::size_t>(0, parameters.n_cols - 1)(mant::Rng::getGenerator())));
      }
      
      CHECK(optimisationProblem.getNumberOfEvaluations() == 3 * parameters.n_cols);
    }
  }

  SECTION("OptimisationProblem.getObjectiveValue(...)") {
    SECTION("Checking the objective value.") {
      arma::Mat<double> parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, 10) * 200.0 - 100.0;
      
      for (std::size_t n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == optimisationProblem.getObjectiveValueImplementation(parameter));
      }
    }

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.getObjectiveValue(parameter), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.getSoftConstraintsValue(...)") {
    SECTION("Checking the default value.") {
      arma::Mat<double> parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, 10) * 200.0 - 100.0;
      
      for (std::size_t n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getDefaultSoftConstraintsValue(parameter) == 0.0);
      }
    }

    SECTION("Checking the soft-constraints value.") {
      arma::Mat<double> parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, 10) * 200.0 - 100.0;
      
      for (std::size_t n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        CHECK(optimisationProblem.getObjectiveValue(parameter) == optimisationProblem.getObjectiveValueImplementation(parameter));
      }
    }

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.getSoftConstraintsValue(parameter), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.getSoftConstraintsValue(parameter), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.isSatisfyingConstraints(...)") {
    SECTION("Checking if it returns true if all bound- and soft-constraints are fulfilled and false otherwise.") {
      // All elements of the lower bound are set to -0.5.
      optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 0.5);
      // All elements of the upper bound are set to 2.
      optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 2.0);
      
      // All parameters with more negative than positive values fulfil the soft-constraint.
      CHECK(optimisationProblem.isSatisfyingConstraints({-0.5, 2.0, -0.3, 0.0, -0.1}) == true);
      // Using 0.0001 instead of arma::datum::eps to avoid unexpected loss of precision. 
      CHECK(optimisationProblem.isSatisfyingConstraints({-0.50001, 2.0, -1.0, 0.0, -0.5}) == false); // Out of lower bound
      CHECK(optimisationProblem.isSatisfyingConstraints({-0.5, 2.1, -0.3, 0.0, 2.0}) == false); // Out of upper bound
      CHECK(optimisationProblem.isSatisfyingConstraints({-0.5, 2.0, 1.0, 0.0, -0.1}) == false); // Does not satisfy the soft-constraints.
    }
    
    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.isSatisfyingConstraints(parameter), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.isSatisfyingConstraints(parameter), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.isSatisfyingSoftConstraints(...)") {
    SECTION("Checking if it returns true if all bound- and soft-constraints are fulfilled and false otherwise.") {
      // All elements of the lower bound are set to -0.5.
      optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 0.5);
      // All elements of the upper bound are set to 2.
      optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 2.0);
      
      // All parameters with more negative than positive values fulfil the soft-constraint.
      CHECK(optimisationProblem.isSatisfyingSoftConstraints({-0.5, 2.0, -0.3, 0.0, -0.1}) == true);
      // Using 0.0001 instead of arma::datum::eps to avoid unexpected loss of precision. 
      CHECK(optimisationProblem.isSatisfyingSoftConstraints({-0.50001, 2.0, -1.0, 0.0, -0.5}) == true); // Out of lower bound
      CHECK(optimisationProblem.isSatisfyingSoftConstraints({-0.5, 2.1, -0.3, 0.0, 2.0}) == false); // Out of upper bound
      CHECK(optimisationProblem.isSatisfyingSoftConstraints({-0.5, 2.0, 1.0, 0.0, -0.1}) == false); // Does not satisfy the soft-constraints.
    }
    
    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.isSatisfyingSoftConstraints(parameter), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.isSatisfyingSoftConstraints(parameter), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.isWithinLowerBounds(...)") {
    SECTION("Checking if it returns true if all bound- and soft-constraints are fulfilled and false otherwise.") {
      // All elements of the lower bound are set to -0.5.
      optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 0.5);
      // All elements of the upper bound are set to 2.
      optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 2.0);
      
      compare(optimisationProblem.isWithinLowerBounds({-0.6, 10.0, std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -0.5}), {0, 1, 1, 0, 1});
    }
    
    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.isWithinLowerBounds(parameter), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.isWithinLowerBounds(parameter), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.isWithinUpperBounds(...)") {
    SECTION("Checking if it returns true if all bound- and soft-constraints are fulfilled and false otherwise.") {
      // All elements of the lower bound are set to -0.5.
      optimisationProblem.setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - 0.5);
      // All elements of the upper bound are set to 2.
      optimisationProblem.setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + 2.0);
      
      compare(optimisationProblem.isWithinUpperBounds({-0.6, 10.0, std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 2.0}), {1, 0, 0, 1, 1});
    }
    
    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.isWithinUpperBounds(parameter), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameter(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.isWithinUpperBounds(parameter), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.setObjectiveValueScaling(...)") {
    SECTION("Checking the parametrisation of the scaling of the soft-constraints value.") {
      const double objectiveValueScaling = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
        
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = objectiveValueScaling * optimisationProblem.getSoftConstraintsValueImplementation(parameter);
      
      CHECK(optimisationProblem.getSoftConstraintsValue(parameter) == expected);
    } 
    
    SECTION("Checking the parametrisation of the scaling of the objective value.") {
      const double objectiveValueScaling = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
        
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = objectiveValueScaling * optimisationProblem.getObjectiveValueImplementation(parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      // Do it again, to ensure it also works with caching.
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    } 
      
    SECTION("Checking the ordering between the scaling and translation of the objective value.") {
      const double objectiveValueTranslation = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      const double objectiveValueScaling = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
    
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
      
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = objectiveValueScaling * optimisationProblem.getObjectiveValueImplementation(parameter) + objectiveValueTranslation;
        
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      // Do it again, to ensure it also works with caching.
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    }

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the objective value scaling is infinite.") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveValueScaling(std::numeric_limits<double>::infinity()), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.setObjectiveValueTranslation(...)") {
    SECTION("Checking the parametrisation of the scaling of the objective value.") {
      const double objectiveValueTranslation = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = optimisationProblem.getObjectiveValueImplementation(parameter) + objectiveValueTranslation;
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      // Do it again, to ensure it also works with caching.
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    } 
    
    // The ordering between between the scaling and translation of the objective value is already
    // checked within OptimisationProblem.setObjectiveValueScaling(...).

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the objective value translation is infinite.") {
        CHECK_THROWS_AS(optimisationProblem.setObjectiveValueTranslation(std::numeric_limits<double>::infinity()), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.setParameterPermutation(...)") {
    SECTION("Checking the parametrisation of the permutation of the dimensions of the soft-constraints function.") {
      std::array<arma::Col<unsigned int>, 2> parameterPermutations = {{
        // Neutral permutation (0, 1, ..., numberOfDimensions - 1)
        arma::linspace<arma::Mat<unsigned int>>(0, numberOfDimensions - 1, numberOfDimensions),
        mant::getRandomPermutation(numberOfDimensions)
      }};

      for (auto parameterPermutation : parameterPermutations) {
        optimisationProblem.setParameterPermutation(parameterPermutation);
        
        const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
        const double expected = optimisationProblem.getSoftConstraintsValueImplementation(parameter.elem(parameterPermutation));
        
        CHECK(optimisationProblem.getSoftConstraintsValue(parameter) == expected);
      }
    }
    
    SECTION("Checking the parametrisation of the permutation of the dimensions of the objective function.") {
      std::array<arma::Col<unsigned int>, 2> parameterPermutations = {{
        // Neutral permutation (0, 1, ..., numberOfDimensions - 1)
        arma::linspace<arma::Mat<unsigned int>>(0, numberOfDimensions - 1, numberOfDimensions),
        mant::getRandomPermutation(numberOfDimensions)
      }};

      for (auto parameterPermutation : parameterPermutations) {
        optimisationProblem.setParameterPermutation(parameterPermutation);
        
        const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
        const double expected = optimisationProblem.getObjectiveValueImplementation(parameter.elem(parameterPermutation));
        
        CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
        // Do it again, to ensure it also works with caching.
        CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      }
    }

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<unsigned int> parameterPermutations(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutations), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<unsigned int> parameterPermutations(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutations), std::logic_error);
      }
 
      SECTION("Throw an exception, if any elements is duplicated.") {
        arma::Col<unsigned int> parameterPermutation = mant::getRandomPermutation(numberOfDimensions);
        parameterPermutation(0) = parameterPermutation(numberOfDimensions - 1);

        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
      }
      
      SECTION("Throw an exception, if any elements lies not within [0, numberOfDimensions - 1].") {
        arma::Col<unsigned int> parameterPermutation = mant::getRandomPermutation(numberOfDimensions);
        
        // Sets the largest index to be greater than the upper bound.
        parameterPermutation.elem(arma::find(parameterPermutation == numberOfDimensions - 1)) = arma::Col<unsigned int>({static_cast<unsigned int>(numberOfDimensions)});
        CHECK_THROWS_AS(optimisationProblem.setParameterPermutation(parameterPermutation), std::logic_error);
        
        // Checking the lower bound is unnecessary, as 0 is already the lowest representable
        // unsigned integer.
      }
    }
  }

  SECTION("OptimisationProblem.reset(...)") {
    SECTION("Checking if all parametrisations remain as given, after resetting the problem.") {
      // Parametrises the optimisation problem.
      const arma::Col<double>& lowerBounds = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      optimisationProblem.setLowerBounds(lowerBounds);
      const arma::Col<double>& upperBounds = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      optimisationProblem.setUpperBounds(upperBounds);
    
      const double acceptableObjectiveValue = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      optimisationProblem.setAcceptableObjectiveValue(acceptableObjectiveValue);
      
      const double objectiveValueScaling = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      optimisationProblem.setObjectiveValueScaling(objectiveValueScaling);
      const double objectiveValueTranslation = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      
      const arma::Col<unsigned int>& parameterPermutation = mant::getRandomPermutation(numberOfDimensions);
      optimisationProblem.setParameterPermutation(parameterPermutation);
      
      // Reset the problem.
      optimisationProblem.reset();
      
      compare(optimisationProblem.getLowerBounds(), lowerBounds);
      compare(optimisationProblem.getUpperBounds(), upperBounds);
      
      CHECK(optimisationProblem.getAcceptableObjectiveValue() == acceptableObjectiveValue);
      
      const arma::Col<double> parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      
      double expected = objectiveValueScaling * optimisationProblem.getSoftConstraintsValueImplementation(parameter);
      CHECK(optimisationProblem.getSoftConstraintsValue(parameter) == expected);
      expected = objectiveValueScaling * optimisationProblem.getObjectiveValueImplementation(parameter.elem(parameterPermutation)) + objectiveValueTranslation;
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    }
    
    SECTION("Checking if all counters are reset and the cache is emptied.") {
      // Increase the evaluation counter ad populate the cache.
      for (std::size_t n = 0; n < 10; ++n) {
        optimisationProblem.getSoftConstraintsValue(arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0);
        optimisationProblem.getObjectiveValue(arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0);
      }
      
      // Reset the problem.
      optimisationProblem.reset();
      
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() == 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() == 0);
      CHECK(optimisationProblem.getCachedObjectiveValues().size() == 0);
    }
  }
}

TEST_CASE("OptimisationProblem<T> (floating point type)") {
  class DummyOptimisationProblem : public mant::OptimisationProblem<double> {
    public:
      using mant::OptimisationProblem<double>::OptimisationProblem;

      std::string toString() const noexcept override {
        return "test_optimisation_problem";
      }
    
      double getObjectiveValueImplementation(
          const arma::Col<double>& parameter) const noexcept override {
        // The objective value is a weighted sum of the parameter values.
        return arma::accu(parameter % arma::linspace<arma::Col<double>>(1, numberOfDimensions_, numberOfDimensions_));
      }
  };

  std::size_t numberOfDimensions = 5;
  DummyOptimisationProblem optimisationProblem(numberOfDimensions);

  SECTION("OptimisationProblem.setParameterRotation(...)") {
    SECTION("Checking the parametrisation of the rotation of the parameter space.") {
      std::array<arma::Mat<double>, 2> parameterRotations = {{
        arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions),
        mant::getRandomRotationMatrix(numberOfDimensions)
      }};

      for (auto parameterRotation : parameterRotations) {
        optimisationProblem.setParameterRotation(parameterRotation);
        
        const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
        const double expected = optimisationProblem.getObjectiveValueImplementation(parameterRotation * parameter);
        
        CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
        // Do it again, to ensure it also works with caching.
        CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      }
    }
    
    SECTION("Checking the ordering between the permutation, scaling, translation and rotation of the parameter space.") {
      const arma::Col<unsigned int>& parameterPermutation = mant::getRandomPermutation(numberOfDimensions);
      const arma::Col<double>& parameterScaling = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const arma::Col<double>& parameterTranslation = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const arma::Mat<double>& parameterRotation = mant::getRandomRotationMatrix(numberOfDimensions);
    
      optimisationProblem.setParameterPermutation(parameterPermutation);
      optimisationProblem.setParameterScaling(parameterScaling);
      optimisationProblem.setParameterTranslation(parameterTranslation);
      optimisationProblem.setParameterRotation(parameterRotation);
      
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = optimisationProblem.getObjectiveValueImplementation(parameterRotation * (parameterScaling % parameter.elem(parameterPermutation) - parameterTranslation));
        
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      // Do it again, to ensure it also works with caching.
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    }

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameterRotation(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameterRotation(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(parameterRotation), std::logic_error);
      }
 
      SECTION("Throw an exception, if the matrix is not orthonormal.") {
        CHECK_THROWS_AS(optimisationProblem.setParameterRotation(arma::zeros<arma::Mat<double>>(numberOfDimensions, numberOfDimensions)), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.setParameterScaling(...)") {
    SECTION("Checking the parametrisation of the scaling of the parameter space.") {
      const arma::Col<double>& parameterScaling = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      optimisationProblem.setParameterScaling(parameterScaling);
        
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = optimisationProblem.getObjectiveValueImplementation(parameterScaling % parameter);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      // Do it again, to ensure it also works with caching.
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    }
    
    // The ordering between between the permutation, scaling, translation and rotation of the
    // parameter space is already checked within OptimisationProblem.setParameterRotation(...).

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameterScaling(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameterScaling(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }

      SECTION("Throw an exception, if any parameter value is infinite.") {
        arma::Col<double> parameterScaling = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
        parameterScaling(0) = std::numeric_limits<double>::infinity();
        
        CHECK_THROWS_AS(optimisationProblem.setParameterScaling(parameterScaling), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.setParameterTranslation(...)") {
    SECTION("Checking the parametrisation of the translation of the parameter space.") {
      const arma::Col<double>& parameterTranslation = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      optimisationProblem.setParameterTranslation(parameterTranslation);
        
      const arma::Col<double>& parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected = optimisationProblem.getObjectiveValueImplementation(parameter - parameterTranslation);
      
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
      // Do it again, to ensure it also works with caching.
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    }
    
    // The ordering between between the permutation, scaling, translation and rotation of the
    // parameter space is already checked within OptimisationProblem.setParameterRotation(...).

    SECTION("Exception tests") {
      SECTION("Throw an exception, if the number of elements is higher than the problem dimension.") {
        arma::Col<double> parameterTranslation(numberOfDimensions + 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }

      SECTION("Throw an exception, if the number of elements is lower than the problem dimension.") {
        arma::Col<double> parameterTranslation(numberOfDimensions - 1);
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }

      SECTION("Throw an exception, if any parameter value is infinite.") {
        arma::Col<double> parameterTranslation = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
        parameterTranslation(0) = std::numeric_limits<double>::infinity();
        
        CHECK_THROWS_AS(optimisationProblem.setParameterTranslation(parameterTranslation), std::logic_error);
      }
    }
  }

  SECTION("OptimisationProblem.reset(...)") {
    SECTION("Checking if all parametrisations remain as given, after resetting the problem.") {
      // Parametrises the optimisation problem.
      const arma::Mat<double>& parameterRotation = mant::getRandomRotationMatrix(numberOfDimensions);
      optimisationProblem.setParameterRotation(parameterRotation);
      const arma::Col<double>& parameterScaling = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      optimisationProblem.setParameterScaling(parameterScaling);
      const arma::Col<double>& parameterTranslation = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      optimisationProblem.setParameterTranslation(parameterTranslation);
      
      // Reset the problem.
      optimisationProblem.reset();
      
      const arma::Col<double> parameter = arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
      const double expected =  optimisationProblem.getObjectiveValueImplementation(parameterRotation * (parameterScaling % parameter - parameterTranslation));
      CHECK(optimisationProblem.getObjectiveValue(parameter) == expected);
    }
  }
}
