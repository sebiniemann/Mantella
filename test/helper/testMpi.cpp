// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "mpiGetBestSample") {
  SECTION(
      "SUPPORT_MPI is always defined if used together with MPI and otherwise undefined.") {
#if defined(MPI_SUCCESS) xor defined(SUPPORT_MPI)
    FAIL(
        "SUPPORT_MPI was not defined together with MPI_SUCCESS.");
#endif
  }

#if defined(SUPPORT_MPI)
  SECTION(
      "Sets the second parameter to the pair with the best (lowest) objective value.") {
    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(2, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfSamples = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfSamples);

    arma::Mat<double> firstMpiPackage(2 + numberOfDimensions, numberOfSamples);
    firstMpiPackage.row(0).fill(static_cast<double>(numberOfDimensions));
    firstMpiPackage.tail_rows(1 + numberOfDimensions) = arma::randu<arma::Mat<double>>(1 + numberOfDimensions, numberOfSamples);

    arma::Mat<double> secondMpiPackage(2 + numberOfDimensions, numberOfSamples);
    secondMpiPackage.row(0).fill(static_cast<double>(numberOfDimensions));
    secondMpiPackage.tail_rows(1 + numberOfDimensions) = arma::randu<arma::Mat<double>>(1 + numberOfDimensions, numberOfSamples);

    firstMpiPackage.row(1) = secondMpiPackage.row(1) - arma::randu<arma::Row<double>>(numberOfSamples) * 10 - 20;
    const arma::Mat<double>& bestMpiPackage = secondMpiPackage;
    CAPTURE(firstMpiPackage);
    CAPTURE(secondMpiPackage);

    int signedNumberOfSamples = static_cast<int>(numberOfSamples);
    MPI_Datatype MANT_MPI_PARAMETER;
    MPI_Type_contiguous(3 + numberOfDimensions, MPI_DOUBLE, &MANT_MPI_PARAMETER);
    MPI_Type_commit(&MANT_MPI_PARAMETER);

    // Make sure that the first parameter is better than the second one.
    mant::mpiGetBestSample(firstMpiPackage.memptr(), secondMpiPackage.memptr(), &signedNumberOfSamples, &MANT_MPI_PARAMETER);
    COMPARE(secondMpiPackage, bestMpiPackage);
  }
#endif
}
