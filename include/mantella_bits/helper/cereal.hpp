#pragma once

// C++ Standard Library
#include <cstdlib>
#include <vector>

// Armadillo
#include <armadillo>

// Cereal
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

// TODO Add tests for these functions and add some additional tests within the optimisation problems

namespace cereal {
  // Helper template to serialise matrices.
  template <typename Archive, class Type>
  void save(
      // Archive to be filled.
      Archive& archive,
      // Matrix to be serialised.
      const arma::Mat<Type>& matrix) noexcept {
    std::size_t n_rows = matrix.n_rows;
    std::size_t n_cols = matrix.n_cols;
    std::size_t n_elem = matrix.n_elem;

    // Save the shape ...
    archive(cereal::make_nvp("numberOfRows", n_rows));
    archive(cereal::make_nvp("numberOfCols", n_cols));

    // ... and the values of the matrix.
    std::vector<Type> data(n_elem);
    for (std::size_t n = 0; n < n_elem; ++n) {
      data.at(n) = matrix.at(n);
    }
    archive(cereal::make_nvp("values", data));
  }

  // Helper template to deserialise matrices
  template <typename Archive, class Type>
  void load(
      // Archive containing the serialised matrix.
      Archive& archive,
      // Output location
      arma::Mat<Type>& matrix) noexcept {
    std::size_t n_rows;
    std::size_t n_cols;

    // Extract the shape ...
    archive(cereal::make_nvp("numberOfRows", n_rows));
    archive(cereal::make_nvp("numberOfCols", n_cols));

    // ... and the values ...
    std::vector<Type> data(n_rows * n_cols);
    archive(cereal::make_nvp("values", data));

    // ... in order to fill a temporary matrix ...
    arma::Mat<Type> temp(data);
    // ... and reshae it.
    temp.reshape(n_rows, n_cols);

    // Replace the temporary matrix with the provided storage location.
    matrix.swap(temp);
  }

  // Helper template to serialise column vectors.
  template <typename Archive, class Type>
  void save(
      // Archive to be filled.
      Archive& archive,
      // Column vector to be serialised.
      const arma::Col<Type>& column) noexcept {
    std::size_t n_elem = column.n_elem;

    // Save the leght ...
    archive(cereal::make_nvp("numberOfElements", n_elem));

    // ... and the values of the column vector.
    std::vector<Type> data(n_elem);
    for (std::size_t n = 0; n < n_elem; ++n) {
      data.at(n) = column.at(n);
    }
    archive(cereal::make_nvp("values", data));
  }

  // Helper template to deserialise column vectors.
  template <typename Archive, class Type>
  void load(
      // Archive containing the serialised column vector.
      Archive& archive,
      // Output location
      arma::Col<Type>& column) noexcept {
    std::size_t n_elem;

    // Extract the length ...
    archive(cereal::make_nvp("numberOfElements", n_elem));

    // ... and the values ...
    std::vector<Type> data(n_elem);
    archive(cereal::make_nvp("values", data));

    // ... in order to fill a temporary column vector.
    arma::Col<Type> temp(data);

    // Replace the temporary column vector with the provided storage location.
    column.swap(temp);
  }

  // Helper template to serialise row vectors.
  template <typename Archive, class Type>
  void save(
      // Archive to be filled.
      Archive& archive,
      // Row vector to be serialised.
      const arma::Row<Type>& row) noexcept {
    std::size_t n_elem = row.n_elem;

    // Save the length ...
    archive(cereal::make_nvp("numberOfElements", n_elem));

    // ... and the values of the row vector.
    std::vector<Type> data(n_elem);
    for (std::size_t n = 0; n < n_elem; ++n) {
      data.at(n) = row.at(n);
    }
   archive(cereal::make_nvp("values", data));
  }

  // Helper template to deserialise row vectors.
  template <typename Archive, class Type>
  void load(
      // Archive containing the serialised row vector.
      Archive& archive,
      // Output location
      arma::Row<Type>& row) noexcept {
    std::size_t n_elem;

    // Extract the length ...
    archive(cereal::make_nvp("numberOfElements", n_elem));

    // ... and the values ...
    std::vector<Type> data(n_elem);
    archive(cereal::make_nvp("values", data));

    // ... in order to fill a temporary row vector.
    arma::Row<Type> temp(data);

    // Replace the temporary row vector with the provided storage location.
    row.swap(temp);
  }
}
