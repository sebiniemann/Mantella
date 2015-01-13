// Catch
#include <catch.hpp>

// C++ Standard Library
#include <memory>
#include <iostream>
#include <cmath>

// Boost
#include <boost/filesystem.hpp>

// Armadillo
#include <armadillo>

// HOP
#include <hop>

class trajectoryBasedAlgorithmHelper{
 public:
 static void compareResults(std::vector<arma::Col<double>> actualHistory,arma::Mat<double> expected){
    for(std::size_t n = 0; n < expected.n_cols; ++n) {
      arma::Col<double> expectedParameter = expected.col(n);
      arma::Col<double> actualParameter = actualHistory.at(n);

      for (std::size_t k = 0; k < expectedParameter.n_elem; ++k) {
        if(std::isfinite(expectedParameter.at(k))) {
          CHECK(actualParameter.at(k) == Approx(expectedParameter.at(k)));
        } else {
          CHECK(actualParameter.at(k) == expectedParameter.at(k));
        }
      }
    }
 }
};
