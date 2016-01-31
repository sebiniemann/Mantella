// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("mpiBestSample", "[mpi][mpiBestSample]") {
#if defined(SUPPORT_MPI)
  GIVEN("Two set of samples") {
    const arma::uword numberOfElements = discreteRandomNumber();
    CAPTURE(numberOfElements);

    const arma::uword numberOfSamples = discreteRandomNumber();
    CAPTURE(numberOfSamples);

    MPI_Datatype MANT_MPI_PARAMETER;
    // The first value is the number of dimensions.
    // The second value is the objective value.
    // The third to last values are the parameter.
    MPI_Type_contiguous(static_cast<int>(2 + numberOfElements), MPI_DOUBLE, &MANT_MPI_PARAMETER);
    MPI_Type_commit(&MANT_MPI_PARAMETER);

    // mant::mpiBestSample (like any MPI_Op_Func) uses the second parameter as input/output parameter, replacing it by the *more optimal* input.
    // To observe this, we ensure that the first input is *more optimal* (has a lower objective value).
    // The second input
    arma::Mat<double> secondMpiInput(2 + numberOfElements, numberOfSamples);
    secondMpiInput.row(0).fill(static_cast<double>(numberOfElements));
    secondMpiInput.tail_rows(1 + numberOfElements) = continuousRandomNumbers(1 + numberOfElements, numberOfSamples);
    CAPTURE(secondMpiInput);

    // The first input
    arma::Mat<double> firstMpiInput(2 + numberOfElements, numberOfSamples);
    firstMpiInput.row(0).fill(static_cast<double>(numberOfElements));
    // Ensure that the first input has a lower objective value.
    firstMpiInput.row(1) = secondMpiInput.row(1) - 1;
    firstMpiInput.tail_rows(numberOfElements) = continuousRandomNumbers(numberOfElements, numberOfSamples);
    CAPTURE(firstMpiInput);

    // MPI uses singed integers, instead of unsigned ones.
    int signedNumberOfSamples = static_cast<int>(numberOfSamples);

    arma::Mat<double> expectedFirstMpiInput = firstMpiInput;
    arma::Mat<double> expectedSecondMpiInput = firstMpiInput;

    THEN("Set the second input to the samples with the pair-wise lowest objective value") {
      mant::mpiBestSample(firstMpiInput.memptr(), secondMpiInput.memptr(), &signedNumberOfSamples, &MANT_MPI_PARAMETER);
      IS_EQUAL(expectedFirstMpiInput, firstMpiInput); // The first input should remain untouched.
      IS_EQUAL(expectedSecondMpiInput, secondMpiInput); // The second parameter should be replaced by the first one.
    }
  }
#endif
}
