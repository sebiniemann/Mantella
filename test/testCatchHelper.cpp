// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("operator==", "[catchHelper][operator==]") {
  GIVEN("Two maps") {
    WHEN("One map is empty") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({})) == false);
      }
    }

    WHEN("Both maps are empty") {
      THEN("Return true") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({})) == true);
      }
    }

    WHEN("Both maps contain NaNs") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::quiet_NaN()}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("Both maps contain infinity") {
      THEN("Return true") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{std::numeric_limits<double>::infinity()}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{std::numeric_limits<double>::infinity()}, 2.0}})) == true);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, -std::numeric_limits<double>::infinity()}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, -std::numeric_limits<double>::infinity()}})) == true);
      }
    }

    WHEN("One map is a subset of the other one") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0}, -3.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0}, -3.0}})) == false);
      }
    }

    WHEN("Both maps contain different samples") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{2.0}, -3.0}})) == false);
      }
    }

    WHEN("Both maps are identical (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0, -3.0}, 4.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0, -3.0}, 4.0}})) == true);
      }
    }
  }

  GIVEN("Two vectors of floating points") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({}) == std::vector<arma::vec>({arma::vec({1.0})})) == false);
        CHECK((std::vector<arma::vec>({arma::vec({1.0})}) == std::vector<arma::vec>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({}) == std::vector<arma::vec>({})) == true);
      }
    }

    WHEN("Both vectors contain NaNs") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::quiet_NaN()})}) == std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::quiet_NaN()})})) == false);
      }
    }

    WHEN("Both vectors contain infinity") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::infinity()})}) == std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::infinity()})})) == true);
      }
    }

    WHEN("One vector is a subset of the other one") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({{0.0}, {2.0}}) == std::vector<arma::vec>({arma::vec({0.0})})) == false);
        CHECK((std::vector<arma::vec>({arma::vec({0.0})}) == std::vector<arma::vec>({{0.0}, {2.0}})) == false);
      }
    }

    WHEN("Both vectors contain different parameters") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({arma::vec({0.0})}) == std::vector<arma::vec>({arma::vec({2.0})})) == false);
      }
    }

    WHEN("Both vectors differ by less than the defined machine precision") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({{0.0}}) == std::vector<arma::vec>({{std::nexttoward(::mant::machinePrecision, 0.0)}})) == true);
      }
    }

    WHEN("Both vectors are identical (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({{0.0}, {2.0, -3.0}}) == std::vector<arma::vec>({{0.0}, {2.0, -3.0}})) == true);
      }
    }
  }

  GIVEN("Two vectors of samples") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({}) == std::vector<arma::vec>({})) == true);
      }
    }

    WHEN("Both vectors contain NaNs") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::quiet_NaN()}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::quiet_NaN()}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::quiet_NaN()}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("Both vectors contain infinity") {
      THEN("Return true") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::infinity()}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::infinity()}), -2.0}})) == true);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::infinity()}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::infinity()}})) == true);
      }
    }

    WHEN("One vector is a subset of the other one") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0}), -4.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0}), -4.0}})) == false);
      }
    }

    WHEN("Both vectors contain different samples") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({3.0}), -4.0}})) == false);
      }
    }

    WHEN("Both vectors differ by less than the defined machine precision") {
      THEN("Return true") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({0.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::nexttoward(::mant::machinePrecision, 0.0)}), -2.0}})) == true);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({3.0}), 0.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({3.0}), std::nexttoward(::mant::machinePrecision, 0.0)}})) == true);
      }
    }

    WHEN("Both vectors are identical (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0, 1.0}), -4.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0, 1.0}), -4.0}})) == true);
      }
    }
  }
}

SCENARIO("operator!", "[catchHelper][operator!]") {
  GIVEN("A discrete vector") {
    WHEN("Any element is 0") {
      THEN("Return true") {
        CHECK(!arma::uvec({0, 1}) == true);
      }
    }

    WHEN("No element is 0") {
      THEN("Return false") {
        CHECK(!arma::uvec({1, 1}) == false);
      }
    }
  }
}

SCENARIO("hasSameElements", "[catchHelper][hasSameElements]") {
  GIVEN("Two vectors of integers") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<arma::uvec>({}), std::vector<arma::uvec>({arma::uvec({0})})) == false);
        CHECK(hasSameElements(std::vector<arma::uvec>({arma::uvec({0})}), std::vector<arma::uvec>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<arma::uvec>({}), std::vector<arma::uvec>({})) == true);
      }
    }

    WHEN("One vector is a proper subset of the other one") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<arma::uvec>({{0}, {1}}), std::vector<arma::uvec>({arma::uvec({0})})) == false);
        CHECK(hasSameElements(std::vector<arma::uvec>({arma::uvec({0})}), std::vector<arma::uvec>({{0}, {1}})) == false);
      }
    }

    WHEN("Both vectors contain different samples") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<arma::uvec>({arma::uvec({0})}), std::vector<arma::uvec>({arma::uvec({1})})) == false);
      }
    }

    WHEN("Both vectors have the same elements (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<arma::uvec>({{0}, {1, 2}}), std::vector<arma::uvec>({{1, 2}, {0}})) == true);
      }
    }
  }

  GIVEN("Two vectors of pairs") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}})) == false);
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({}), std::vector<std::pair<arma::uvec, arma::uvec>>({})) == true);
      }
    }

    WHEN("One vector is a proper subset of the other one") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}, {{2}, {3}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}})) == false);
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}, {{2}, {3}}})) == false);
      }
    }

    WHEN("Both vectors contain different samples") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{2}, {3}}})) == false);
      }
    }

    WHEN("Both vectors have the same elements (and may be dimensionally inconsistent or in different order)") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1, 2}}, {{3, 4}, {5, 6, 7}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{3, 4}, {5, 6, 7}}, {{0}, {1, 2}}})) == true);
      }
    }
  }
}

SCENARIO("hasSameDistribution", "[catchHelper][hasSameDistribution]") {
  GIVEN("A vector and a cumulative distribution") {
    WHEN("Either the vector or the distribution is empty") {
      THEN("Return false") {
        CHECK(hasSameDistribution(arma::vec({1.0}), arma::vec()) == false);
        CHECK(hasSameDistribution(arma::vec(), arma::vec({1.0})) == false);
      }
    }

    WHEN("Both are empty") {
      THEN("Return true") {
        CHECK(hasSameDistribution(arma::vec(), arma::vec()) == true);
      }
    }

    WHEN("The vector has a different distribution") {
      THEN("Return false") {
        CHECK(hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(mant::uniformRandomNumbers(10000), 10)) / 10000.0), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::ones<arma::vec>(10000), 10)) / 10000.0)) == false);
      }
    }

    WHEN("The vector has the same distribution") {
      THEN("Return true") {
        CHECK(hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(mant::normalRandomNumbers(10000), -100.0, 100.0), 10)) / 10000.0), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(mant::normalRandomNumbers(10000), -100.0, 100.0), 10)) / 10000.0)) == true);
      }
    }
  }
}

SCENARIO("isUniformDistributed", "[catchHelper][isUniformDistributed]") {
  GIVEN("A vector") {
    WHEN("The vector is empty") {
      THEN("Return false") {
        CHECK(isUniformDistributed(arma::vec(), 0.0, 1.0) == false);
      }
    }

    WHEN("The vector is not uniformly distributed") {
      THEN("Return false") {
        CHECK(isUniformDistributed(arma::ones<arma::vec>(10000), 0.0, 1.0) == false);
      }
    }

    WHEN("The vector is uniformly distributed") {
      THEN("Return true") {
        CHECK(isUniformDistributed(mant::uniformRandomNumbers(10000) * 2.0 + 1.0, 1.0, 2.0) == true);
      }
    }
  }
}

SCENARIO("isNormalDistributed", "[catchHelper][isNormalDistributed]") {
  GIVEN("A vector") {
    WHEN("The vector is empty") {
      THEN("Return false") {
        CHECK(isNormalDistributed(arma::vec(), 1.0) == false);
      }
    }

    WHEN("The vector is not normal distributed") {
      THEN("Return false") {
        CHECK(isNormalDistributed(arma::ones<arma::vec>(10000), 1.0) == false);
      }
    }

    WHEN("The vector is normal distributed") {
      THEN("Return true") {
        arma::vec::fixed<10000> normalDistribution;
        for (arma::uword n = 0; n < normalDistribution.n_elem; ++n) {
          normalDistribution(n) = std::normal_distribution<double>(0.0, 3.0)(mant::Rng::getGenerator());
        }
        CHECK(isNormalDistributed(normalDistribution, 3.0) == true);
      }
    }
  }
}

SCENARIO("isCauchyDistributed", "[catchHelper][isCauchyDistributed]") {
  GIVEN("A vector") {
    WHEN("The vector is empty") {
      THEN("Return false") {
        CHECK(isCauchyDistributed(arma::vec(), 1.0) == false);
      }
    }

    WHEN("The vector is not Cauchy distributed") {
      THEN("Return false") {
        CHECK(isCauchyDistributed(arma::ones<arma::vec>(10000), 1.0) == false);
      }
    }

    WHEN("The vector is Cauchy distributed") {
      THEN("Return true") {
        arma::vec::fixed<10000> cauchyDistribtion;
        for (arma::uword n = 0; n < cauchyDistribtion.n_elem; ++n) {
          cauchyDistribtion(n) = std::cauchy_distribution<double>(0.0, 10.0)(mant::Rng::getGenerator());
        }
        CHECK(isCauchyDistributed(cauchyDistribtion, 10.0) == true);
      }
    }
  }
}
