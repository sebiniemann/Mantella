// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <unordered_map>
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

SCENARIO("nodeRank", "[catchExtension][nodeRank]") {
  THEN("Contain the node rank, if MPI is supported, or 0 otherwise") {
    int rank = 0;

#if defined(SUPPORT_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    CHECK(nodeRank == rank);
  }
}

SCENARIO("numberOfNodes", "[catchExtension][numberOfNodes]") {
  THEN("Contain the number of nodes, if MPI is supported, or 1 otherwise") {
    int size = 1;

#if defined(SUPPORT_MPI)
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

    CHECK(numberOfNodes == size);
  }
}

SCENARIO("continuousRandomNumbers", "[catchExtension][continuousRandomNumbers]") {
  GIVEN("A number of rows and a number columns") {
    const arma::uword numberOfRows = 40;
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = 250;
    CAPTURE(numberOfColumns);

    THEN("Return a continuous matrix of uniformly distributed random values within [-100, 100]^n.") {
      CHECK(continuousRandomNumbers(numberOfRows, numberOfColumns).n_rows == numberOfRows);
      CHECK(continuousRandomNumbers(numberOfRows, numberOfColumns).n_cols == numberOfColumns);
      IS_UNIFORM(arma::vectorise(continuousRandomNumbers(numberOfRows, numberOfColumns)), -100.0, 100.0);
    }
  }

  GIVEN("A number of elements") {
    const arma::uword numberOfElements = 10000;
    CAPTURE(numberOfElements);

    THEN("Return a continuous vector of uniformly distributed random values within [-100, 100]^n.") {
      CHECK(continuousRandomNumbers(numberOfElements).is_colvec());
      IS_UNIFORM(continuousRandomNumbers(numberOfElements), -100.0, 100.0);
    }
  }
}

SCENARIO("discreteRandomNumbers", "[catchExtension][discreteRandomNumbers]") {
  GIVEN("A number of rows and a number columns") {
    const arma::uword numberOfRows = 500;
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = 20;
    CAPTURE(numberOfColumns);

    THEN("Return a discrete matrix of uniformly distributed random values within [1, 10]^n.") {
      CHECK(discreteRandomNumbers(numberOfRows, numberOfColumns).n_rows == numberOfRows);
      CHECK(discreteRandomNumbers(numberOfRows, numberOfColumns).n_cols == numberOfColumns);
      IS_UNIFORM(arma::vectorise(discreteRandomNumbers(numberOfRows, numberOfColumns)), 1, 10);
    }
  }

  GIVEN("A number of elements") {
    const arma::uword numberOfElements = 10000;
    CAPTURE(numberOfElements);

    THEN("Return a discrete vector of uniformly distributed random values within [1, 10]^n.") {
      CHECK(discreteRandomNumbers(numberOfElements).is_colvec());
      IS_UNIFORM(discreteRandomNumbers(numberOfElements), 1, 10);
    }
  }
}

SCENARIO("continuousRandomNumber", "[catchExtension][continuousRandomNumber]") {
  THEN("Return a continuous number, uniformly and randomly distributed within [-100, 100]^n.") {
    arma::Col<double>::fixed<10000> randomNumbers;
    for (arma::uword n = 0; n < randomNumbers.n_elem; ++n) {
      randomNumbers(n) = continuousRandomNumber();
    }

    IS_UNIFORM(randomNumbers, -100.0, 100.0);
  }
}

SCENARIO("discreteRandomNumber", "[catchExtension][discreteRandomNumber]") {
  THEN("Return a discrete number, uniformly and randomly distributed within [1, 10]^n.") {
    arma::Col<arma::uword>::fixed<10000> randomNumbers;
    for (arma::uword n = 0; n < randomNumbers.n_elem; ++n) {
      randomNumbers(n) = discreteRandomNumber();
    }

    IS_UNIFORM(randomNumbers, 1, 10);
  }
}

SCENARIO("differentDiscreteRandomNumber", "[catchExtension][differentDiscreteRandomNumber]") {
  GIVEN("A number [a]") {
    THEN("Return a discrete number, uniformly and randomly distributed within [1, 10]^n, excluding [a].") {
      arma::Col<arma::uword>::fixed<10000> randomNumbers;
      for (arma::uword n = 0; n < randomNumbers.n_elem; ++n) {
        arma::uword randomNumberToExclude = discreteRandomNumber();
        arma::uword differentRandomNumber = differentDiscreteRandomNumber(randomNumberToExclude);

        CHECK(differentRandomNumber != randomNumberToExclude);
        randomNumbers(n) = differentRandomNumber;
      }

      CAPTURE(randomNumbers);

      // Since we uniformly exclude a number from [1, 10] for each draw, the whole set of numbers should again be uniform.
      IS_UNIFORM(randomNumbers, 1, 10);
    }
  }
}

SCENARIO("SYNCHRONISED", "[catchExtension][SYNCHRONISED]") {
  GIVEN("A continuous matrices for each node") {
    const arma::uword numberOfRows = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfColumns);

    arma::Mat<double> matrix = arma::eye<arma::Mat<double>>(numberOfRows, numberOfColumns) * 1.0 / (static_cast<double>(nodeRank) + 1);
    CAPTURE(matrix);

    THEN("Return the matrix from the first node") {
      const arma::Mat<double>& synchronisedMatrix = SYNCHRONISED(matrix);
      CAPTURE(synchronisedMatrix);

#if defined(SUPPORT_MPI)
      MPI_Bcast(matrix.memptr(), static_cast<int>(matrix.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      CAPTURE(matrix)
#endif

      IS_EQUAL(synchronisedMatrix, matrix);
    }
  }

  GIVEN("A discrete matrices for each node") {
    const arma::uword numberOfRows = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfColumns);

    arma::Mat<arma::uword> matrix = arma::eye<arma::Mat<arma::uword>>(numberOfRows, numberOfColumns) * static_cast<arma::uword>(nodeRank);
    CAPTURE(matrix);

    THEN("Return the matrix from the first node") {
      const arma::Mat<arma::uword>& synchronisedMatrix = SYNCHRONISED(matrix);
      CAPTURE(synchronisedMatrix);

#if defined(SUPPORT_MPI)
      arma::Mat<unsigned int> synchronisedData = arma::conv_to<arma::Mat<unsigned int>>::from(matrix);
      MPI_Bcast(synchronisedData.memptr(), static_cast<int>(synchronisedData.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
      matrix = arma::conv_to<arma::Mat<arma::uword>>::from(synchronisedData);
      CAPTURE(matrix)
#endif

      IS_EQUAL(synchronisedMatrix, matrix);
    }
  }

  GIVEN("A continuous column vectors for each node") {
    const arma::uword numberOfElements = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfElements);

    arma::Col<double> vector = arma::ones<arma::Col<double>>(numberOfElements) * 1.0 / (static_cast<double>(nodeRank) + 1);
    CAPTURE(vector);

    THEN("Return the vector from the first node") {
      const arma::Col<double>& synchronisedVector = SYNCHRONISED(vector);
      CAPTURE(synchronisedVector);

#if defined(SUPPORT_MPI)
      MPI_Bcast(vector.memptr(), static_cast<int>(vector.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      CAPTURE(vector)
#endif

      IS_EQUAL(synchronisedVector, vector);
    }
  }

  GIVEN("A discrete column vectors for each node") {
    const arma::uword numberOfElements = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfElements);

    arma::Col<arma::uword> vector = arma::ones<arma::Col<arma::uword>>(numberOfElements) * static_cast<arma::uword>(nodeRank);
    CAPTURE(vector);

    THEN("Return the vector from the first node") {
      const arma::Col<arma::uword>& synchronisedVector = SYNCHRONISED(vector);
      CAPTURE(synchronisedVector);

#if defined(SUPPORT_MPI)
      arma::Col<unsigned int> synchronisedData = arma::conv_to<arma::Col<unsigned int>>::from(vector);
      MPI_Bcast(synchronisedData.memptr(), static_cast<int>(synchronisedData.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
      vector = arma::conv_to<arma::Col<arma::uword>>::from(synchronisedData);
      CAPTURE(vector)
#endif

      IS_EQUAL(synchronisedVector, vector);
    }
  }

  GIVEN("A continuous row vectors for each node") {
    const arma::uword numberOfElements = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfElements);

    arma::Row<double> vector = arma::ones<arma::Row<double>>(numberOfElements) * 1.0 / (static_cast<double>(nodeRank) + 1);
    CAPTURE(vector);

    THEN("Return the vector from the first node") {
      const arma::Row<double>& synchronisedVector = SYNCHRONISED(vector);
      CAPTURE(synchronisedVector);

#if defined(SUPPORT_MPI)
      MPI_Bcast(vector.memptr(), static_cast<int>(vector.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      CAPTURE(vector)
#endif

      IS_EQUAL(synchronisedVector, vector);
    }
  }

  GIVEN("A discrete row vectors for each node") {
    const arma::uword numberOfElements = SYNCHRONISED(discreteRandomNumber());
    CAPTURE(numberOfElements);

    arma::Row<arma::uword> vector = arma::ones<arma::Row<arma::uword>>(numberOfElements) * static_cast<arma::uword>(nodeRank);
    CAPTURE(vector);

    THEN("Return the vector from the first node") {
      const arma::Row<arma::uword>& synchronisedVector = SYNCHRONISED(vector);
      CAPTURE(synchronisedVector);

#if defined(SUPPORT_MPI)
      arma::Row<unsigned int> synchronisedData = arma::conv_to<arma::Row<unsigned int>>::from(vector);
      MPI_Bcast(synchronisedData.memptr(), static_cast<int>(synchronisedData.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
      vector = arma::conv_to<arma::Row<arma::uword>>::from(synchronisedData);
      CAPTURE(vector)
#endif

      IS_EQUAL(synchronisedVector, vector);
    }
  }

  GIVEN("A continuous numbers for each node") {
    double number = 1.0 / (static_cast<double>(nodeRank) + 1);
    CAPTURE(number);

    THEN("Return the number from the first node") {
      const double synchronisedNumber = SYNCHRONISED(number);
      CAPTURE(synchronisedNumber);

#if defined(SUPPORT_MPI)
      MPI_Bcast(&number, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      CAPTURE(number)
#endif

      CHECK(synchronisedNumber == Approx(number));
    }
  }

  GIVEN("A discrete numbers for each node") {
    arma::uword number = static_cast<arma::uword>(nodeRank);
    CAPTURE(number);

    THEN("Return the number from the first node") {
      const arma::uword synchronisedNumber = SYNCHRONISED(number);
      CAPTURE(synchronisedNumber);

#if defined(SUPPORT_MPI)
      unsigned int synchronisedData = static_cast<unsigned int>(number);
      MPI_Bcast(&synchronisedData, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
      number = static_cast<arma::uword>(synchronisedData);
      CAPTURE(number)
#endif

      CHECK(synchronisedNumber == number);
    }
  }

  GIVEN("A time duration for each node") {
    std::chrono::microseconds timeDuration = std::chrono::microseconds(nodeRank);
    CAPTURE(timeDuration.count());

    THEN("Return the time uration from the first node") {
      const std::chrono::microseconds synchronisedTimeDuration = SYNCHRONISED(timeDuration);
      CAPTURE(synchronisedTimeDuration.count());

#if defined(SUPPORT_MPI)
      unsigned int synchronisedData = static_cast<unsigned int>(timeDuration.count());
      MPI_Bcast(&synchronisedData, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
      timeDuration = std::chrono::microseconds(synchronisedData);
      CAPTURE(timeDuration.count())
#endif

      CHECK(synchronisedTimeDuration == timeDuration);
    }
  }
}

SCENARIO("HAS_SAME_SAMPLES", "[catchExtension][HAS_SAME_SAMPLES]") {
  GIVEN("Two sets of samples (which may not be dimensionally consistent)") {
    WHEN("Both sets contain the same samples, in any order") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> firstSamples;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        firstSamples.insert({continuousRandomNumbers(discreteRandomNumber()), continuousRandomNumber()});
      }

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> secondSamples;
      for (const auto& sample : firstSamples) {
        secondSamples.insert(sample);
      }

      THEN("Passes") {
        HAS_SAME_SAMPLES(firstSamples, secondSamples);
      }
    }
  }
}

SCENARIO("HAS_SAME_SAMPLES failure test", "[catchExtension][HAS_SAME_SAMPLES][!shouldfail]") {
  GIVEN("Two sets of samples (which may not be dimensionally consistent)") {
    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> firstSamples;

    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      firstSamples.insert({continuousRandomNumbers(discreteRandomNumber()), continuousRandomNumber()});
    }

    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> secondSamples;
    for (const auto& sample : firstSamples) {
      secondSamples.insert(sample);
    }

    WHEN("Both sets have the same number of samples, but differ by at least one") {
      firstSamples.begin()->second += 1.0;

      THEN("Fails") {
        HAS_SAME_SAMPLES(firstSamples, secondSamples);
      }
    }

    WHEN("One set has less samples than the other one") {
      firstSamples.erase(firstSamples.cbegin());

      THEN("Fails") {
        HAS_SAME_SAMPLES(firstSamples, secondSamples);
        HAS_SAME_SAMPLES(secondSamples, firstSamples);
      }
    }
  }
}

SCENARIO("HAS_SAME_PARAMETERS", "[catchExtension][HAS_SAME_PARAMETERS]") {
  const arma::uword numberOfParameters = discreteRandomNumber();
  CAPTURE(numberOfParameters);

  GIVEN("Two sets of continuous parameters (which may not be dimensionally consistent)") {
    WHEN("Both sets contain the same parameters, in any order") {
      std::vector<arma::Col<double>> parameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        parameters.push_back(continuousRandomNumbers(discreteRandomNumber()));
      }

      std::vector<arma::Col<double>> shuffeldParameters = parameters;
      std::random_shuffle(shuffeldParameters.begin(), shuffeldParameters.end());

      THEN("Passes") {
        HAS_SAME_PARAMETERS(shuffeldParameters, parameters);
      }
    }
  }

  GIVEN("Two sets of discrete parameters (which may not be dimensionally consistent)") {
    WHEN("Both sets contain the same parameters, in any order") {
      std::vector<arma::Col<arma::uword>> parameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        parameters.push_back(discreteRandomNumbers(discreteRandomNumber()));
      }

      std::vector<arma::Col<arma::uword>> shuffeldParameters = parameters;
      std::random_shuffle(shuffeldParameters.begin(), shuffeldParameters.end());

      THEN("Passes") {
        HAS_SAME_PARAMETERS(shuffeldParameters, parameters);
      }
    }
  }

  GIVEN("A set of samples and a set of parameters (which may not be dimensionally consistent)") {
    WHEN("Both sets contain the same parameters, in any order") {
      std::vector<arma::Col<double>> parameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        parameters.push_back(continuousRandomNumbers(discreteRandomNumber()));
      }

      std::vector<std::pair<arma::Col<double>, double>> samples;
      for (const auto& parameter : parameters) {
        samples.push_back({parameter, continuousRandomNumber()});
      }
      std::random_shuffle(samples.begin(), samples.end());

      THEN("Passes") {
        HAS_SAME_PARAMETERS(samples, parameters);
      }
    }
  }
}

SCENARIO("HAS_SAME_PARAMETERS failure test", "[catchExtension][HAS_SAME_PARAMETERS][!shouldfail]") {
  const arma::uword numberOfParameters = discreteRandomNumber();
  CAPTURE(numberOfParameters);

  GIVEN("Two sets of continuous parameters (which may not be dimensionally consistent)") {
    std::vector<arma::Col<double>> parameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      parameters.push_back(continuousRandomNumbers(discreteRandomNumber()));
    }

    std::vector<arma::Col<double>> shuffeldParameters = parameters;
    std::random_shuffle(shuffeldParameters.begin(), shuffeldParameters.end());

    WHEN("Both sets have the same number of samples, but differ by at least one") {
      shuffeldParameters.at(0) += 1.0;

      THEN("Fails") {
        HAS_SAME_PARAMETERS(parameters, shuffeldParameters);
      }
    }

    WHEN("One set has less parameters than the other one") {
      shuffeldParameters.pop_back();

      THEN("Fails") {
        HAS_SAME_PARAMETERS(parameters, shuffeldParameters);
        HAS_SAME_PARAMETERS(shuffeldParameters, parameters);
      }
    }
  }

  GIVEN("Two sets of discrete parameters (which may not be dimensionally consistent)") {
    std::vector<arma::Col<arma::uword>> parameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      parameters.push_back(discreteRandomNumbers(discreteRandomNumber()));
    }

    std::vector<arma::Col<arma::uword>> shuffeldParameters = parameters;
    std::random_shuffle(shuffeldParameters.begin(), shuffeldParameters.end());

    WHEN("Both sets have the same number of samples, but differ by at least one") {
      shuffeldParameters.at(0) += 1;

      THEN("Fails") {
        HAS_SAME_PARAMETERS(shuffeldParameters, parameters);
      }
    }

    WHEN("One set has less parameters than the other one") {
      shuffeldParameters.pop_back();

      THEN("Fails") {
        HAS_SAME_PARAMETERS(parameters, shuffeldParameters);
        HAS_SAME_PARAMETERS(shuffeldParameters, parameters);
      }
    }
  }

  GIVEN("A set of samples and a set of parameters (which may not be dimensionally consistent)") {
    std::vector<arma::Col<double>> parameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      parameters.push_back(continuousRandomNumbers(discreteRandomNumber()));
    }

    std::vector<std::pair<arma::Col<double>, double>> samples;
    for (const auto& parameter : parameters) {
      samples.push_back({parameter, continuousRandomNumber()});
    }
    std::random_shuffle(samples.begin(), samples.end());

    WHEN("Both sets have the same number of samples, but differ by at least one parameter") {
      samples.at(0).first += 1.0;

      THEN("Fails") {
        HAS_SAME_PARAMETERS(samples, parameters);
      }
    }

    WHEN("The set of samples has less parameter than set of parameters") {
      samples.pop_back();

      THEN("Fails") {
        HAS_SAME_PARAMETERS(samples, parameters);
      }
    }

    WHEN("The set of parameters has less parameter than set of samples") {
      parameters.pop_back();

      THEN("Fails") {
        HAS_SAME_PARAMETERS(samples, parameters);
      }
    }
  }
}

SCENARIO("HAS_SAME_ELEMENTS", "[catchExtension][HAS_SAME_ELEMENTS]") {
  GIVEN("Two parameters") {
    WHEN("Both parameters contain the same elements (including duplicates), in any order") {
      const arma::uword numberOfElements = discreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::Col<double>& parameter = continuousRandomNumbers(numberOfElements);
      CAPTURE(parameter);
      const arma::Col<double>& shuffeldParameter = arma::shuffle(parameter);
      CAPTURE(shuffeldParameter);

      THEN("Passes") {
        HAS_SAME_ELEMENTS(shuffeldParameter, parameter);
      }
    }
  }
}

SCENARIO("HAS_SAME_ELEMENTS failure test", "[catchExtension][HAS_SAME_ELEMENTS][!shouldfail]") {
  GIVEN("Two parameters") {
    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    const arma::Col<double> parameter = continuousRandomNumbers(numberOfElements);
    CAPTURE(parameter);

    arma::Col<double> shuffeldParameter = arma::shuffle(parameter);

    WHEN("Both parameters have the same number of element but differ by at least one element") {
      shuffeldParameter(0) += 1;
      CAPTURE(shuffeldParameter);

      THEN("Fails") {
        HAS_SAME_ELEMENTS(parameter, shuffeldParameter);
      }
    }

    WHEN("One set has less elements than the other one") {
      shuffeldParameter.resize(shuffeldParameter.n_elem - 1);
      CAPTURE(shuffeldParameter);

      THEN("Fails") {
        HAS_SAME_ELEMENTS(parameter, shuffeldParameter);
        HAS_SAME_ELEMENTS(shuffeldParameter, parameter);
      }
    }
  }
}

SCENARIO("IS_EQUAL", "[catchExtension][IS_EQUAL]") {
  GIVEN("Two sets of samples") {
    WHEN("Both sets are identical (have the same samples in the same order)") {
      const arma::uword numberOfSamples = discreteRandomNumber();
      CAPTURE(numberOfSamples);

      std::vector<std::pair<arma::Col<double>, double>> firstSamples;
      for (arma::uword n = 0; n < numberOfSamples; ++n) {
        firstSamples.push_back({continuousRandomNumbers(discreteRandomNumber()), continuousRandomNumber()});
      }

      std::vector<std::pair<arma::Col<double>, double>> secondSamples;
      for (arma::uword n = 0; n < firstSamples.size(); ++n) {
        secondSamples.push_back(firstSamples.at(n));
      }

      THEN("Passes") {
        IS_EQUAL(firstSamples, secondSamples);
      }
    }
  }

  GIVEN("Two sets of continuous parameters") {
    WHEN("Both sets are identical (have the same elements in the same order)") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      std::vector<arma::Col<double>> firstParameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        firstParameters.push_back(continuousRandomNumbers(discreteRandomNumber()));
      }

      std::vector<arma::Col<double>> secondParameters;
      for (arma::uword n = 0; n < firstParameters.size(); ++n) {
        secondParameters.push_back(firstParameters.at(n));
      }

      THEN("Passes") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two sets of 3-dimensional continuous parameters ") {
    WHEN("Both sets are identical (have the same elements in the same order)") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      std::vector<arma::Col<double>::fixed<3>> firstParameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        firstParameters.push_back(continuousRandomNumbers(3));
      }

      std::vector<arma::Col<double>> secondParameters;
      for (arma::uword n = 0; n < firstParameters.size(); ++n) {
        secondParameters.push_back(firstParameters.at(n));
      }

      THEN("Passes") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two sets of 2-dimensional continuous parameters") {
    WHEN("Both sets are identical (have the same elements in the same order)") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      std::vector<arma::Col<double>::fixed<2>> firstParameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        firstParameters.push_back(continuousRandomNumbers(2));
      }

      std::vector<arma::Col<double>> secondParameters;
      for (arma::uword n = 0; n < firstParameters.size(); ++n) {
        secondParameters.push_back(firstParameters.at(n));
      }

      THEN("Passes") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two sets of discrete parameters") {
    WHEN("Both sets are identical (have the same elements in the same order)") {
      const arma::uword numberOfParameters = discreteRandomNumber();
      CAPTURE(numberOfParameters);

      std::vector<arma::Col<arma::uword>> firstParameters;
      for (arma::uword n = 0; n < numberOfParameters; ++n) {
        firstParameters.push_back(discreteRandomNumbers(discreteRandomNumber()));
      }

      std::vector<arma::Col<arma::uword>> secondParameters;
      for (arma::uword n = 0; n < firstParameters.size(); ++n) {
        secondParameters.push_back(firstParameters.at(n));
      }

      THEN("Passes") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two continuous cubes") {
    WHEN("Both cubes are identical (have the same elements in the same order)") {
      const arma::uword numberOfRows = discreteRandomNumber();
      CAPTURE(numberOfRows);

      const arma::uword numberOfColumns = discreteRandomNumber();
      CAPTURE(numberOfColumns);

      const arma::uword numberOfSlices = discreteRandomNumber();
      CAPTURE(numberOfSlices);

      const arma::Cube<double>& firstCube = arma::randu<arma::Cube<double>>(numberOfRows, numberOfColumns, numberOfSlices);
      CAPTURE(firstCube);

      const arma::Cube<double>& secondCube = firstCube;
      CAPTURE(secondCube);

      THEN("Passes") {
        IS_EQUAL(firstCube, secondCube);
      }
    }
  }

  GIVEN("Two discrete cubes") {
    WHEN("Both cubes are identical (have the same elements in the same order)") {
      const arma::uword numberOfRows = discreteRandomNumber();
      CAPTURE(numberOfRows);

      const arma::uword numberOfColumns = discreteRandomNumber();
      CAPTURE(numberOfColumns);

      const arma::uword numberOfSlices = discreteRandomNumber();
      CAPTURE(numberOfSlices);

      const arma::Cube<arma::uword>& firstCube = arma::randi<arma::Cube<arma::uword>>(numberOfRows, numberOfColumns, numberOfSlices);
      CAPTURE(firstCube);

      const arma::Cube<arma::uword>& secondCube = firstCube;
      CAPTURE(secondCube);

      THEN("Passes") {
        IS_EQUAL(firstCube, secondCube);
      }
    }
  }

  GIVEN("Two continuous matrices") {
    WHEN("Both sets are identical (have the same elements in the same order)") {
      const arma::uword numberOfRows = discreteRandomNumber();
      CAPTURE(numberOfRows);

      const arma::uword numberOfColumns = discreteRandomNumber();
      CAPTURE(numberOfColumns);

      const arma::Mat<double>& firstMatrix = continuousRandomNumbers(numberOfRows, numberOfColumns);
      CAPTURE(firstMatrix);

      const arma::Mat<double>& secondMatrix = firstMatrix;
      CAPTURE(secondMatrix);

      THEN("Passes") {
        IS_EQUAL(firstMatrix, secondMatrix);
      }
    }
  }

  GIVEN("Two discrete matrices") {
    WHEN("Both sets are identical (have the same elements in the same order)") {
      const arma::uword numberOfRows = discreteRandomNumber();
      CAPTURE(numberOfRows);

      const arma::uword numberOfColumns = discreteRandomNumber();
      CAPTURE(numberOfColumns);

      const arma::Mat<arma::uword>& firstMatrix = discreteRandomNumbers(numberOfRows, numberOfColumns);
      CAPTURE(firstMatrix);

      const arma::Mat<arma::uword>& secondMatrix = firstMatrix;
      CAPTURE(secondMatrix);

      THEN("Passes") {
        IS_EQUAL(firstMatrix, secondMatrix);
      }
    }
  }

  GIVEN("Two continuous column vectors") {
    WHEN("Both vectors are identical (have the same elements in the same order)") {
      const arma::uword numberOfElements = discreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::Col<double>& firstVector = continuousRandomNumbers(numberOfElements);
      CAPTURE(firstVector);

      const arma::Col<double>& secondVector = firstVector;
      CAPTURE(secondVector);

      THEN("Passes") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }

  GIVEN("Two discrete column vectors") {
    WHEN("Both vectors are identical (have the same elements in the same order)") {
      const arma::uword numberOfElements = discreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::Col<arma::uword>& firstVector = discreteRandomNumbers(numberOfElements);
      CAPTURE(firstVector);

      const arma::Col<arma::uword>& secondVector = firstVector;
      CAPTURE(secondVector);

      THEN("Passes") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }

  GIVEN("Two continuous row vectors") {
    WHEN("Both vectors are identical (have the same elements in the same order)") {
      const arma::uword numberOfElements = discreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::Row<double>& firstVector = continuousRandomNumbers(numberOfElements).t();
      CAPTURE(firstVector);

      const arma::Row<double>& secondVector = firstVector;
      CAPTURE(secondVector);

      THEN("Passes") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }

  GIVEN("Two discrete row vectors") {
    WHEN("Both vectors are identical (have the same elements in the same order)") {
      const arma::uword numberOfElements = discreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::Row<arma::uword>& firstVector = discreteRandomNumbers(numberOfElements).t();
      CAPTURE(firstVector);

      const arma::Row<arma::uword>& secondVector = firstVector;
      CAPTURE(secondVector);

      THEN("Passes") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }
}

SCENARIO("IS_EQUAL failure test", "[catchExtension][IS_EQUAL][!shouldfail]") {
  GIVEN("Two sets of samples") {
    const arma::uword numberOfSamples = discreteRandomNumber();
    CAPTURE(numberOfSamples);

    std::vector<std::pair<arma::Col<double>, double>> firstSamples;
    for (arma::uword n = 0; n < numberOfSamples; ++n) {
      firstSamples.push_back({continuousRandomNumbers(discreteRandomNumber()), continuousRandomNumber()});
    }

    std::vector<std::pair<arma::Col<double>, double>> secondSamples;
    for (arma::uword n = 0; n < firstSamples.size(); ++n) {
      secondSamples.push_back(firstSamples.at(n));
    }

    WHEN("Both sets have the same number of samples, but differ by at least one") {
      firstSamples.at(0).first += 1.0;

      THEN("Fails") {
        IS_EQUAL(firstSamples, secondSamples);
      }
    }

    WHEN("One set has less samples than the other one") {
      firstSamples.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstSamples, secondSamples);
        IS_EQUAL(secondSamples, firstSamples);
      }
    }

    WHEN("Both sets have the same number of samples, but in a different order") {
      firstSamples.push_back({firstSamples.at(0).first + 1.0, firstSamples.at(0).second});
      secondSamples.push_back({secondSamples.at(0).first + 1.0, secondSamples.at(0).second});
      std::reverse(secondSamples.begin(), secondSamples.end());

      THEN("Fails") {
        IS_EQUAL(firstSamples, secondSamples);
      }
    }
  }

  GIVEN("Two sets of continuous parameters") {
    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    std::vector<arma::Col<double>> firstParameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      firstParameters.push_back(continuousRandomNumbers(discreteRandomNumber()));
    }

    std::vector<arma::Col<double>> secondParameters;
    for (arma::uword n = 0; n < firstParameters.size(); ++n) {
      secondParameters.push_back(firstParameters.at(n));
    }

    WHEN("Both sets have the same number of parameters, but differ by at least one") {
      firstParameters.at(0) += 1.0;

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("One set has less parameters than the other one") {
      firstParameters.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
        IS_EQUAL(secondParameters, firstParameters);
      }
    }

    WHEN("Both sets have the same number of parameters, but in a different order") {
      firstParameters.push_back(firstParameters.at(0) + 1.0);
      secondParameters.push_back(secondParameters.at(0) + 1.0);
      std::reverse(secondParameters.begin(), secondParameters.end());

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two sets of 3-dimensional continuous parameters ") {
    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    std::vector<arma::Col<double>::fixed<3>> firstParameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      firstParameters.push_back(continuousRandomNumbers(3));
    }

    std::vector<arma::Col<double>> secondParameters;
    for (arma::uword n = 0; n < firstParameters.size(); ++n) {
      secondParameters.push_back(firstParameters.at(n));
    }

    WHEN("Both sets have the same number of parameters, but differ by at least one") {
      firstParameters.at(0) += 1.0;

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("The first set has less parameter than the second one") {
      firstParameters.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("The second set has less parameter than the first one") {
      secondParameters.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("Both sets have the same number of parameters, but in a different order") {
      firstParameters.push_back(firstParameters.at(0) + 1.0);
      secondParameters.push_back(secondParameters.at(0) + 1.0);
      std::reverse(secondParameters.begin(), secondParameters.end());

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two sets of 2-dimensional continuous parameters") {
    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    std::vector<arma::Col<double>::fixed<2>> firstParameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      firstParameters.push_back(continuousRandomNumbers(2));
    }

    std::vector<arma::Col<double>> secondParameters;
    for (arma::uword n = 0; n < firstParameters.size(); ++n) {
      secondParameters.push_back(firstParameters.at(n));
    }

    WHEN("Both sets have the same number of parameters, but differ by at least one") {
      firstParameters.at(0) += 1.0;

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("The first set has less parameter than the second one") {
      firstParameters.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("The second set has less parameter than the first one") {
      secondParameters.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("Both sets have the same number of parameters, but in a different order") {
      firstParameters.push_back(firstParameters.at(0) + 1.0);
      secondParameters.push_back(secondParameters.at(0) + 1.0);
      std::reverse(secondParameters.begin(), secondParameters.end());

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two sets of discrete parameters") {
    const arma::uword numberOfParameters = discreteRandomNumber();
    CAPTURE(numberOfParameters);

    std::vector<arma::Col<arma::uword>> firstParameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      firstParameters.push_back(discreteRandomNumbers(discreteRandomNumber()));
    }

    std::vector<arma::Col<arma::uword>> secondParameters;
    for (arma::uword n = 0; n < firstParameters.size(); ++n) {
      secondParameters.push_back(firstParameters.at(n));
    }

    WHEN("Both sets have the same number of parameters, but differ by at least one") {
      firstParameters.at(0) += 1;

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }

    WHEN("One set has less parameters than the other one") {
      firstParameters.pop_back();

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
        IS_EQUAL(secondParameters, firstParameters);
      }
    }

    WHEN("Both sets have the same number of parameters, but in a different order") {
      firstParameters.push_back(firstParameters.at(0) + 1);
      secondParameters.push_back(secondParameters.at(0) + 1);
      std::reverse(secondParameters.begin(), secondParameters.end());

      THEN("Fails") {
        IS_EQUAL(firstParameters, secondParameters);
      }
    }
  }

  GIVEN("Two continuous cubes") {
    const arma::uword numberOfRows = discreteRandomNumber();
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = discreteRandomNumber();
    CAPTURE(numberOfColumns);

    const arma::uword numberOfSlices = discreteRandomNumber();
    CAPTURE(numberOfSlices);

    arma::Cube<double> firstCube = arma::randu<arma::Cube<double>>(numberOfRows, numberOfColumns, numberOfSlices);
    arma::Cube<double> secondCube = firstCube;

    WHEN("Both cubes have a different number of rows") {
      firstCube.resize(firstCube.n_rows + 1, firstCube.n_cols, firstCube.n_slices);
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
        IS_EQUAL(secondCube, firstCube);
      }
    }

    WHEN("Both cubes have a different number of columns") {
      firstCube.resize(firstCube.n_rows, firstCube.n_cols + 1, firstCube.n_slices);
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
        IS_EQUAL(secondCube, firstCube);
      }
    }

    WHEN("Both cubes have a different number of slices") {
      firstCube.resize(firstCube.n_rows, firstCube.n_cols, firstCube.n_slices + 1);
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
        IS_EQUAL(secondCube, firstCube);
      }
    }

    WHEN("Both cubes differ by at least one element") {
      firstCube(0) += 1.0;
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
      }
    }

    WHEN("Both cubes have the same elements, but in a different order") {
      firstCube.insert_slices(0, firstCube.slices(0, 0) + 1.0);
      CAPTURE(firstCube);

      secondCube.insert_slices(1, secondCube.slices(0, 0) + 1.0);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
      }
    }
  }

  GIVEN("Two discrete cubes") {
    const arma::uword numberOfRows = discreteRandomNumber();
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = discreteRandomNumber();
    CAPTURE(numberOfColumns);

    const arma::uword numberOfSlices = discreteRandomNumber();
    CAPTURE(numberOfSlices);

    arma::Cube<arma::uword> firstCube = arma::randi<arma::Cube<arma::uword>>(numberOfRows, numberOfColumns, numberOfSlices);
    arma::Cube<arma::uword> secondCube = firstCube;

    WHEN("Both cubes have a different number of rows") {
      firstCube.resize(firstCube.n_rows + 1, firstCube.n_cols, firstCube.n_slices);
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
        IS_EQUAL(secondCube, firstCube);
      }
    }

    WHEN("Both cubes have a different number of columns") {
      firstCube.resize(firstCube.n_rows, firstCube.n_cols + 1, firstCube.n_slices);
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
        IS_EQUAL(secondCube, firstCube);
      }
    }

    WHEN("Both cubes have a different number of slices") {
      firstCube.resize(firstCube.n_rows, firstCube.n_cols, firstCube.n_slices + 1);
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
        IS_EQUAL(secondCube, firstCube);
      }
    }

    WHEN("Both cubes differ by at least one element") {
      firstCube(0) += 1;
      CAPTURE(firstCube);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
      }
    }

    WHEN("Both cubes have the same elements, but in a different order") {
      firstCube.insert_slices(0, firstCube.slices(0, 0) + 1);
      CAPTURE(firstCube);

      secondCube.insert_slices(1, secondCube.slices(0, 0) + 1);
      CAPTURE(secondCube);

      THEN("Fails") {
        IS_EQUAL(firstCube, secondCube);
      }
    }
  }

  GIVEN("Two continuous matrices") {
    const arma::uword numberOfRows = discreteRandomNumber();
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = discreteRandomNumber();
    CAPTURE(numberOfColumns);

    arma::Mat<double> firstMatrix = continuousRandomNumbers(numberOfRows, numberOfColumns);
    arma::Mat<double> secondMatrix = firstMatrix;

    WHEN("Both matrices have a different number of rows") {
      firstMatrix.resize(firstMatrix.n_rows + 1, firstMatrix.n_cols);
      CAPTURE(firstMatrix);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
        IS_EQUAL(secondMatrix, firstMatrix);
      }
    }

    WHEN("Both matrices have a different number of columns") {
      firstMatrix.resize(firstMatrix.n_rows, firstMatrix.n_cols + 1);
      CAPTURE(firstMatrix);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
        IS_EQUAL(secondMatrix, firstMatrix);
      }
    }

    WHEN("Both matrices differ by at least one element") {
      firstMatrix(0) += 1.0;
      CAPTURE(firstMatrix);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
      }
    }

    WHEN("Both matrices have the same elements, but in a different order") {
      firstMatrix.insert_cols(0, firstMatrix.cols(0, 0) + 1.0);
      CAPTURE(firstMatrix);

      secondMatrix.insert_cols(1, secondMatrix.cols(0, 0) + 1.0);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
      }
    }
  }

  GIVEN("Two discrete matrices") {
    const arma::uword numberOfRows = discreteRandomNumber();
    CAPTURE(numberOfRows);

    const arma::uword numberOfColumns = discreteRandomNumber();
    CAPTURE(numberOfColumns);

    arma::Mat<arma::uword> firstMatrix = discreteRandomNumbers(numberOfRows, numberOfColumns);
    arma::Mat<arma::uword> secondMatrix = firstMatrix;

    WHEN("Both matrices have a different number of rows") {
      firstMatrix.resize(firstMatrix.n_rows + 1, firstMatrix.n_cols);
      CAPTURE(firstMatrix);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
        IS_EQUAL(secondMatrix, firstMatrix);
      }
    }

    WHEN("Both matrices have a different number of columns") {
      firstMatrix.resize(firstMatrix.n_rows, firstMatrix.n_cols + 1);
      CAPTURE(firstMatrix);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
        IS_EQUAL(secondMatrix, firstMatrix);
      }
    }

    WHEN("Both matrices differ by at least one element") {
      firstMatrix(0) += 1;
      CAPTURE(firstMatrix);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
      }
    }

    WHEN("Both matrices have the same elements, but in a different order") {
      firstMatrix.insert_cols(0, firstMatrix.cols(0, 0) + 1);
      CAPTURE(firstMatrix);

      secondMatrix.insert_cols(1, secondMatrix.cols(0, 0) + 1);
      CAPTURE(secondMatrix);

      THEN("Fails") {
        IS_EQUAL(firstMatrix, secondMatrix);
      }
    }
  }

  GIVEN("Two continuous column vectors") {
    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    arma::Col<double> firstVector = continuousRandomNumbers(numberOfElements);
    arma::Col<double> secondVector = firstVector;

    WHEN("Both vectors have a different number of elements") {
      firstVector.resize(firstVector.n_elem + 1);
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
        IS_EQUAL(secondVector, firstVector);
      }
    }

    WHEN("Both vectors differ by at least one element") {
      firstVector(0) += 1.0;
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }

    WHEN("Both vectors have the same elements, but in a different order") {
      firstVector.insert_rows(0, firstVector.rows(0, 0) + 1.0);
      CAPTURE(firstVector);

      secondVector.insert_rows(1, secondVector.rows(0, 0) + 1.0);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }

  GIVEN("Two discrete column vectors") {
    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    arma::Col<arma::uword> firstVector = discreteRandomNumbers(numberOfElements);
    arma::Col<arma::uword> secondVector = firstVector;

    WHEN("Both vectors have a different number of elements") {
      firstVector.resize(firstVector.n_elem + 1);
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
        IS_EQUAL(secondVector, firstVector);
      }
    }

    WHEN("Both vectors differ by at least one element") {
      firstVector(0) += 1;
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }

    WHEN("Both vectors have the same elements, but in a different order") {
      firstVector.insert_rows(0, firstVector.rows(0, 0) + 1);
      CAPTURE(firstVector);

      secondVector.insert_rows(1, secondVector.rows(0, 0) + 1);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }

  GIVEN("Two continuous row vectors") {
    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    arma::Row<double> firstVector = continuousRandomNumbers(numberOfElements).t();
    arma::Row<double> secondVector = firstVector;

    WHEN("Both vectors have a different number of elements") {
      firstVector.resize(firstVector.n_elem + 1);
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
        IS_EQUAL(secondVector, firstVector);
      }
    }

    WHEN("Both vectors differ by at least one element") {
      firstVector(0) += 1.0;
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }

    WHEN("Both vectors have the same elements, but in a different order") {
      firstVector.insert_cols(0, firstVector.cols(0, 0) + 1.0);
      CAPTURE(firstVector);

      secondVector.insert_cols(1, secondVector.cols(0, 0) + 1.0);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }

  GIVEN("Two discrete row vectors") {
    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    arma::Row<arma::uword> firstVector = discreteRandomNumbers(numberOfElements).t();
    arma::Row<arma::uword> secondVector = firstVector;

    WHEN("Both vectors have a different number of elements") {
      firstVector.resize(firstVector.n_elem + 1);
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
        IS_EQUAL(secondVector, firstVector);
      }
    }

    WHEN("Both vectors differ by at least one element") {
      firstVector(0) += 1;
      CAPTURE(firstVector);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }

    WHEN("Both vectors have the same elements, but in a different order") {
      firstVector.insert_cols(0, firstVector.cols(0, 0) + 1);
      CAPTURE(firstVector);

      secondVector.insert_cols(1, secondVector.cols(0, 0) + 1);
      CAPTURE(secondVector);

      THEN("Fails") {
        IS_EQUAL(firstVector, secondVector);
      }
    }
  }
}

// We can't use any of the above generators for this, since we use *IS_UNIFORM* to test these generators and would run into circular reasoning otherwise.
SCENARIO("IS_UNIFORM", "[catchExtension][IS_UNIFORM]") {
  GIVEN("A continuous vector") {
    WHEN("The vector is uniformly distributed") {
      const arma::Col<double>& vector = arma::randu<arma::Col<double>>(10000) * 200.0 - 100.0;
      CAPTURE(vector);

      THEN("Passes") {
        IS_UNIFORM(vector, -100.0, 100.0);
      }
    }
  }

  GIVEN("A discrete vector") {
    WHEN("The vector is uniformly distributed") {
      const arma::Col<arma::uword>& vector = arma::randi<arma::Col<arma::uword>>(10000, arma::distr_param(0, 100));
      CAPTURE(vector);

      THEN("Passes") {
        IS_UNIFORM(vector, 0, 100);
      }
    }
  }
}

SCENARIO("IS_UNIFORM failure test", "[catchExtension][IS_UNIFORM][!shouldfail]") {
  GIVEN("A continuous vector") {
    WHEN("The vector is out of bound") {
      arma::Col<double> vector = arma::randu<arma::Col<double>>(10000) * 200.0 - 100.0;
      vector(0) = -200.0;
      CAPTURE(vector);

      THEN("Fails") {
        IS_UNIFORM(vector, -100.0, 100.0);
      }
    }

    WHEN("The vector is not uniformly distributed") {
      const arma::Col<double>& vector = arma::randu<arma::Col<double>>(100) * 200.0 - 100.0;
      CAPTURE(vector);

      THEN("Fails") {
        IS_UNIFORM(vector, -200.0, 200.0);
      }
    }
  }

  GIVEN("A discrete vector") {
    WHEN("The vector is out of bound") {
      arma::Col<arma::uword> vector = arma::randi<arma::Col<arma::uword>>(10000, arma::distr_param(0, 100));
      vector(0) = 200.0;
      CAPTURE(vector);

      THEN("Fails") {
        IS_UNIFORM(vector, 0, 100);
      }
    }

    WHEN("The vector is not uniformly distributed") {
      const arma::Col<arma::uword>& vector = arma::randi<arma::Col<arma::uword>>(100, arma::distr_param(0, 100));
      CAPTURE(vector);

      THEN("Fails") {
        IS_UNIFORM(vector, 0, 200);
      }
    }
  }
}
