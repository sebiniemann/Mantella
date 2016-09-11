#include "mantella_bits/mpi.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#if defined(SUPPORT_MPI)
#include <limits> // IWYU pragma: keep
#include <stdexcept> // IWYU pragma: keep
#endif

// MPI
#if defined(SUPPORT_MPI)
#include <mpi.h> // IWYU pragma: keep
#endif

namespace mant {
  arma::mat synchronise(
      arma::mat data) {
#if defined(SUPPORT_MPI)
    arma::Col<unsigned long long> dataSize(2);
    if (nodeRank() == 0) {
      if (data.n_rows > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of rows to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_cols > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of columns to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_elem > std::numeric_limits<int>::max()) {
        throw std::range_error("synchronise: The number of elements must be less than or equal to the largest supported (signed) integer.");
      }

      dataSize(0) = static_cast<unsigned long long>(data.n_rows);
      dataSize(1) = static_cast<unsigned long long>(data.n_cols);
    }

    MPI_Bcast(dataSize.memptr(), 2, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if (arma::prod(dataSize) == 0) {
      return {};
    } else if (nodeRank() != 0) {
      data.set_size(dataSize(0), dataSize(1));
    }

    MPI_Bcast(data.memptr(), static_cast<int>(data.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
    return data;
  }

  arma::umat synchronise(
      const arma::umat& data) {
#if defined(SUPPORT_MPI)
    arma::Col<unsigned long long> dataSize(2);
    if (nodeRank() == 0) {
      if (data.n_rows > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of rows to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_cols > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of columns to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_elem > std::numeric_limits<int>::max()) {
        throw std::range_error("synchronise: The number of elements must be less than or equal to the largest supported (signed) integer.");
      } else {
        for (const auto element : data) {
          if (element > std::numeric_limits<unsigned long long>::max()) {
            throw std::range_error("synchronise: The numbers to be synchronised must be less than or equal to the largest supported MPI integer.");
          }
        }
      }

      dataSize(0) = static_cast<unsigned long long>(data.n_rows);
      dataSize(1) = static_cast<unsigned long long>(data.n_cols);
    }

    MPI_Bcast(dataSize.memptr(), 2, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    arma::Mat<unsigned long long> synchronisedData;
    if (arma::prod(dataSize) == 0) {
      return {};
    } else if (nodeRank() != 0) {
      synchronisedData.set_size(dataSize(0), dataSize(1));
    } else {
      synchronisedData = arma::conv_to<arma::Mat<unsigned long long>>::from(data);
    }

    // `arma::uword` has no guaranteed bit length and therefore no explicit MPI_* // counterpart.
    // We are therefore converting it into the largest integer with a guaranteed MPI_* counterpart.
    MPI_Bcast(synchronisedData.memptr(), static_cast<int>(synchronisedData.n_elem), MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    return arma::conv_to<arma::umat>::from(synchronisedData);
#else
    return data;
#endif
  }

  arma::vec synchronise(
      arma::vec data) {
#if defined(SUPPORT_MPI)
    unsigned long long dataSize;
    if (nodeRank() == 0) {
      if (data.n_elem > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of elements to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_elem > std::numeric_limits<int>::max()) {
        throw std::range_error("synchronise: The number of elements must be less than or equal to the largest supported (signed) integer.");
      }

      dataSize = static_cast<unsigned long long>(data.n_elem);
    }

    MPI_Bcast(&dataSize, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if (dataSize == 0) {
      return {};
    } else if (nodeRank() != 0) {
      data.set_size(dataSize);
    }

    MPI_Bcast(data.memptr(), static_cast<int>(data.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
    return data;
  }

  arma::uvec synchronise(
      const arma::uvec& data) {
#if defined(SUPPORT_MPI)
    unsigned long long dataSize;
    if (nodeRank() == 0) {
      if (data.n_elem > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of elements to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_elem > std::numeric_limits<int>::max()) {
        throw std::range_error("synchronise: The number of elements must be less than or equal to the largest supported (signed) integer.");
      } else {
        for (const auto element : data) {
          if (element > std::numeric_limits<unsigned long long>::max()) {
            throw std::range_error("synchronise: The numbers to be synchronised must be less than or equal to the largest supported MPI integer.");
          }
        }
      }

      dataSize = static_cast<unsigned long long>(data.n_elem);
    }

    MPI_Bcast(&dataSize, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    arma::Col<unsigned long long> synchronisedData;
    if (dataSize == 0) {
      return {};
    } else if (nodeRank() != 0) {
      synchronisedData.set_size(dataSize);
    } else {
      synchronisedData = arma::conv_to<arma::Col<unsigned long long>>::from(data);
    }

    // `arma::uword` has no guaranteed bit length and therefore no explicit MPI_* // counterpart.
    // We are therefore converting it into the largest integer with a guaranteed MPI_* counterpart.
    MPI_Bcast(synchronisedData.memptr(), static_cast<int>(synchronisedData.n_elem), MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    return arma::conv_to<arma::uvec>::from(synchronisedData);
#else
    return data;
#endif
  }

  arma::rowvec synchronise(
      arma::rowvec data) {
#if defined(SUPPORT_MPI)
    unsigned long long dataSize;
    if (nodeRank() == 0) {
      if (data.n_elem > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of elements to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_elem > std::numeric_limits<int>::max()) {
        throw std::range_error("synchronise: The number of elements must be less than or equal to the largest supported (signed) integer.");
      }

      dataSize = static_cast<unsigned long long>(data.n_elem);
    }

    MPI_Bcast(&dataSize, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if (dataSize == 0) {
      return {};
    } else if (nodeRank() != 0) {
      data.set_size(dataSize);
    }

    MPI_Bcast(data.memptr(), static_cast<int>(data.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
    return data;
  }

  arma::urowvec synchronise(
      const arma::urowvec& data) {
#if defined(SUPPORT_MPI)
    unsigned long long dataSize;
    if (nodeRank() == 0) {
      if (data.n_elem > std::numeric_limits<unsigned long long>::max()) {
        throw std::range_error("synchronise: The number of elements to be synchronised must be less than or equal to the largest supported MPI integer.");
      } else if (data.n_elem > std::numeric_limits<int>::max()) {
        throw std::range_error("synchronise: The number of elements must be less than or equal to the largest supported (signed) integer.");
      } else {
        for (const auto element : data) {
          if (element > std::numeric_limits<unsigned long long>::max()) {
            throw std::range_error("synchronise: The numbers to be synchronised must be less than or equal to the largest supported MPI integer.");
          }
        }
      }

      dataSize = static_cast<unsigned long long>(data.n_elem);
    }

    MPI_Bcast(&dataSize, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    arma::Col<unsigned long long> synchronisedData;
    if (dataSize == 0) {
      return {};
    } else if (nodeRank() != 0) {
      synchronisedData.set_size(dataSize);
    } else {
      synchronisedData = arma::conv_to<arma::Col<unsigned long long>>::from(data);
    }

    // `arma::uword` has no guaranteed bit length and therefore no explicit MPI_* // counterpart.
    // We are therefore converting it into the largest integer with a guaranteed MPI_* counterpart.
    MPI_Bcast(synchronisedData.memptr(), static_cast<int>(synchronisedData.n_elem), MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    return arma::conv_to<arma::urowvec>::from(synchronisedData);
#else
    return data;
#endif
  }

  double synchronise(
      double data) {
#if defined(SUPPORT_MPI)
    MPI_Bcast(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return data;
#else
    return data;
#endif
  }

  arma::uword synchronise(
      const arma::uword data) {
#if defined(SUPPORT_MPI)
    // `arma::uword` has no guaranteed bit length and therefore no explicit MPI_* // counterpart.
    // We are therefore converting it into the largest integer with a guaranteed MPI_* counterpart.
    if (data > std::numeric_limits<unsigned long long>::max()) {
      throw std::range_error("synchronise: The number to be synchronised must be less than or equal to the largest supported MPI integer.");
    }

    unsigned long long synchronisedData = static_cast<unsigned long long>(data);
    MPI_Bcast(&synchronisedData, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    return static_cast<arma::uword>(synchronisedData);
#else
    return data;
#endif
  }
}
