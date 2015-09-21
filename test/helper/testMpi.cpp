// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <random>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("mpiGetBestSample") {
  SECTION("SUPPORT_MPI is always defined if used together with MPI and otherwise undefined.") {
#if defined(MPI_SUCCESS) xor defined(SUPPORT_MPI)
    FAIL("SUPPORT_MPI was not defined together with MPI_SUCCESS.");
#endif
  }

#if defined(SUPPORT_MPI)
  SECTION("Sets the second parameter to the pair with the best (lowest) objective value.") {
    const arma::uword numberOfDimensions = getRandomNumberOfValues();
    CAPTURE(numberOfDimensions);

    const arma::uword numberOfSamples = getRandomNumberOfValues();
    CAPTURE(numberOfSamples);

    MPI_Datatype MANT_MPI_PARAMETER;
    // The first value is the number of dimensions.
    // The second value is the objective value.
    // The third to last values are the parameter.
    MPI_Type_contiguous(2 + numberOfDimensions, MPI_DOUBLE, &MANT_MPI_PARAMETER);
    MPI_Type_commit(&MANT_MPI_PARAMETER);

    // mant::mpiGetBestSample (like any MPI_Op_Func) uses the second parameter as input/output parameter, replacing it by the *more optimal* input.
    // To observe this, we ensure that the first input is *more optimal* (has a lower objective value).
    // The second input
    arma::Mat<double> secondMpiInput(2 + numberOfDimensions, numberOfSamples);
    secondMpiInput.row(0).fill(static_cast<double>(numberOfDimensions));
    secondMpiInput.tail_rows(1 + numberOfDimensions) = getRandomValues(1 + numberOfDimensions, numberOfSamples);
    CAPTURE(secondMpiInput);

    // The first input
    arma::Mat<double> firstMpiInput(2 + numberOfDimensions, numberOfSamples);
    firstMpiInput.row(0).fill(static_cast<double>(numberOfDimensions));
    // Ensure that the first input has a lower objective value.
    firstMpiInput.row(1) = secondMpiInput.row(1) - arma::randu<arma::Row<double>>(numberOfSamples) * 10 - 20;
    firstMpiInput.tail_rows(numberOfDimensions) = getRandomValues(numberOfDimensions, numberOfSamples);
    CAPTURE(firstMpiInput);

    // MPI uses singed integers, instead of unsigned ones.
    int signedNumberOfSamples = static_cast<int>(numberOfSamples);
    
    mant::mpiGetBestSample(firstMpiInput.memptr(), secondMpiInput.memptr(), &signedNumberOfSamples, &MANT_MPI_PARAMETER);
    IS_EQUAL(firstMpiInput, firstMpiInput); // The first input should remain untouched.
    IS_EQUAL(secondMpiInput, firstMpiInput); // The second parameter should be replaced by the first one.
  }
#endif
}
