// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("continuity", "[samplesAnalysis][continuity]") {
  GIVEN("A set of samples and a continuity function") {
    const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);
    mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

    mant::RandomSearch optimisationAlgorithm;
    optimisationAlgorithm.setMaximalNumberOfIterations(100);

    ::mant::isCachingSamples = true;
    optimisationAlgorithm.optimise(optimisationProblem);
    ::mant::isCachingSamples = false;

    WHEN("The set is dimensionally consistent and the continuity function is callable") {
      const auto& samples = optimisationProblem.getCachedSamples();

      double continuity = 0.0;
      for (auto firstSample = samples.cbegin(); firstSample != samples.cend();) {
        for (auto secondSample = ++firstSample; secondSample != samples.cend(); ++secondSample) {
          continuity = std::max(continuity, mant::lipschitzContinuity({firstSample->first, firstSample->second}, {secondSample->first, secondSample->second}));
        }
      }

      THEN("Return the continuity") {
        CHECK(mant::continuity(samples, mant::lipschitzContinuity) == Approx(continuity));
      }
    }

    WHEN("The set is dimensionally inconsistent") {
      auto samples = optimisationProblem.getCachedSamples();
      samples.insert({getContinuousRandomNumbers(numberOfDimensions + 1), getContinuousRandomNumber()});

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::continuity(samples, mant::lipschitzContinuity), std::logic_error);
      }
    }

    WHEN("The continuity function is not callable") {
      const auto& samples = optimisationProblem.getCachedSamples();

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::continuity(samples, nullptr), std::logic_error);
      }
    }
  }
}

SCENARIO("lipschitzContinuity", "[samplesAnalysis][lipschitzContinuity]") {
  GIVEN("Two of samples") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const std::pair<arma::Col<double>, double>& firstSample = {getContinuousRandomNumbers(numberOfDimensions), getContinuousRandomNumber()};

    WHEN("Both samples are dimensionally consistent") {
      const std::pair<arma::Col<double>, double>& secondSample = {getContinuousRandomNumbers(numberOfDimensions), getContinuousRandomNumber()};

      THEN("Return the Lipschitz continuity") {
        CHECK(mant::lipschitzContinuity(firstSample, secondSample) == Approx(std::abs(firstSample.second - secondSample.second) / arma::norm(firstSample.first - secondSample.first)));
      }
    }

    WHEN("Both samples are dimensionally inconsistent") {
      const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
      CAPTURE(differentNumberOfDimensions);

      const std::pair<arma::Col<double>, double>& secondSample = {getContinuousRandomNumbers(differentNumberOfDimensions), getContinuousRandomNumber()};

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::lipschitzContinuity(firstSample, secondSample), std::logic_error);
      }
    }
  }
}

SCENARIO("alphaHoelderContinuity", "[samplesAnalysis][alphaHoelderContinuity]") {
  GIVEN("Two of samples and continuous number [alpha]") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    const std::pair<arma::Col<double>, double>& firstSample = {getContinuousRandomNumbers(numberOfDimensions), getContinuousRandomNumber()};

    WHEN("Both samples are dimensionally consistent") {
      const std::pair<arma::Col<double>, double>& secondSample = {getContinuousRandomNumbers(numberOfDimensions), getContinuousRandomNumber()};

      AND_WHEN("[alpha] < 1") {
        const double alpha = std::min(0.9, getContinuousRandomNumber());
        CAPTURE(alpha);

        THEN("Return the alpha-Hoelder continuity") {
          CHECK(mant::alphaHoelderContinuity(firstSample, secondSample, alpha) == Approx(std::abs(firstSample.second - secondSample.second) / std::pow(arma::norm(firstSample.first - secondSample.first), alpha)));
        }
      }

      AND_WHEN("[alpha] == 1") {
        const double alpha = 1.0;
        CAPTURE(alpha);

        THEN("Return the same as the Lipschitz continuity") {
          CHECK(mant::alphaHoelderContinuity(firstSample, secondSample, alpha) == Approx(mant::lipschitzContinuity(firstSample, secondSample)));
        }
      }

      AND_WHEN("[alpha] > 1") {
        const double alpha = std::max(2.0, getContinuousRandomNumber());
        CAPTURE(alpha);

        THEN("Return 1.0") {
          CHECK(mant::alphaHoelderContinuity(firstSample, secondSample, alpha) == Approx(1.0));
        }
      }
    }

    WHEN("Both samples are dimensionally inconsistent") {
      const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
      CAPTURE(differentNumberOfDimensions);

      const std::pair<arma::Col<double>, double>& secondSample = {getContinuousRandomNumbers(differentNumberOfDimensions), getContinuousRandomNumber()};

      const double alpha = getContinuousRandomNumber();
      CAPTURE(alpha);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::alphaHoelderContinuity(firstSample, secondSample, alpha), std::logic_error);
      }
    }
  }
}
