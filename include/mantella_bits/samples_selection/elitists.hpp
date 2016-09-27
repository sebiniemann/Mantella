#pragma once

/**

*/
  std::pair<arma::mat, arma::vec> elitists(
      const arma::mat& parameters,
      const arma::vec& objectiveValues,
      const arma::uword numberOfSamplesToSelect) {
    assert(parameters.n_cols >= numberOfSamplesToSelect && "elitists: The number of parameters must be greater than or equal to the number of samples to select.");
    assert(parameters.n_cols == objectiveValues.n_elem && "fitnessDistanceCorrelation: The number of parameters must be equal to the number of objective values.");

    if (numberOfSamplesToSelect == 0) {
      return {};
    }

    const arma::uvec& elitists = arma::uvec(arma::sort_index(objectiveValues)).head(numberOfSamplesToSelect);
    return {parameters.cols(elitists), objectiveValues.elem(elitists)};
  }