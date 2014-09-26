#pragma once

#include <cstdlib>
#include <vector>

#include <armadillo>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

#include <iostream>

namespace cereal {
  template <class Archive, class Type>
  void save(Archive& archive, const arma::Mat<Type>& matrix) {
    std::size_t n_rows = matrix.n_rows;
    std::size_t n_cols = matrix.n_cols;
    std::size_t n_elem = matrix.n_elem;

    archive(cereal::make_nvp("numberOfRows", n_rows));
    archive(cereal::make_nvp("numberOfCols", n_cols));

    std::vector<Type> data(n_elem);
    for(size_t n = 0; n < n_elem; ++n) {
      data.at(n) = matrix.at(n);
    }
    archive(cereal::make_nvp("values", data));
  }

  template <class Archive, class Type>
  void load(Archive& archive, arma::Mat<Type>& matrix) {
    std::size_t n_rows;
    std::size_t n_cols;

    archive(cereal::make_nvp("numberOfRows", n_rows));
    archive(cereal::make_nvp("numberOfCols", n_cols));

    std::vector<Type> data(n_rows * n_cols);
    archive(cereal::make_nvp("values", data));

    arma::Mat<Type> temp(data);
    temp.reshape(n_rows, n_cols);
    matrix.swap(temp);
  }

  template <class Archive, class Type>
  void save(Archive& archive, const arma::Col<Type>& column) {
    std::size_t n_elem = column.n_elem;

    archive(cereal::make_nvp("numberOfElements", n_elem));

    std::vector<Type> data(n_elem);
    for(size_t n = 0; n < n_elem; ++n) {
      data.at(n) = column.at(n);
    }
    archive(cereal::make_nvp("values", data));
  }

  template <class Archive, class Type>
  void load(Archive& archive, arma::Col<Type>& column) {
    std::size_t n_elem;

    archive(cereal::make_nvp("numberOfElements", n_elem));

    std::vector<Type> data(n_elem);
    archive(cereal::make_nvp("values", data));

    arma::Col<Type> temp(data);
    column.swap(temp);
  }

  template <class Archive, class Type>
  void save(Archive& archive, const arma::Row<Type>& row) {
    std::size_t n_elem = row.n_elem;

    archive(cereal::make_nvp("numberOfElements", n_elem));

    std::vector<Type> data(n_elem);
    for(size_t n = 0; n < n_elem; ++n) {
      data.at(n) = row.at(n);
    }
    archive(cereal::make_nvp("values", data));
  }

  template <class Archive, class Type>
  void load(Archive& archive, arma::Row<Type>& row) {
    std::size_t n_elem;

    archive(cereal::make_nvp("numberOfElements", n_elem));

    std::vector<Type> data(n_elem);
    archive(cereal::make_nvp("values", data));

    arma::Row<Type> temp(data);
    row.swap(temp);
  }
}
