#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction.hpp>

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>
#include <limits>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  namespace bbob {
    GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const arma::uword numberOfDimensions)
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        weight_(arma::join_cols(arma::Col<double>({10.0}), arma::linspace<arma::Col<double>>(1.1, 9.1, 100))) {
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterConditionings(getRandomLocalParameterConditionings());
      setLocalParameterTranslations(getRandomLocalParameterTranslations());
    }

    void GallaghersGaussian101mePeaksFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    void GallaghersGaussian101mePeaksFunction::setLocalParameterConditionings(
        const arma::Mat<double>& localParameterConditionings) {
      verify(localParameterConditionings.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterConditionings.n_cols == 101, "The number of columns must be equal to the number of peaks (101).");

      localParameterConditionings_ = localParameterConditionings;
    }

    void GallaghersGaussian101mePeaksFunction::setLocalParameterTranslations(
        const arma::Mat<double>& localParameterTranslations) {
      verify(localParameterTranslations.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterTranslations.n_cols == 101, "The number of columns must be equal to the number of peaks (101).");

      localParameterTranslations_ = localParameterTranslations;
    }

    arma::Mat<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterConditionings() const {
      arma::Col<double> conditions(101);
      conditions(0) = 49.5;
      conditions.tail(conditions.n_elem - 1) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(conditions.n_elem - 1));

      arma::Mat<double> localParameterConditionings(numberOfDimensions_, conditions.n_elem);
      for (arma::uword n = 0; n < conditions.n_elem; ++n) {
        const double condition = std::pow(10.0, conditions(n) / 33.0);
        localParameterConditionings.col(n) = getParameterConditioning(condition) / std::sqrt(condition);
      }

      return localParameterConditionings;
    }

    arma::Mat<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterTranslations() const {
      arma::Mat<double> localParameterTranslations = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 10.0 - 5.0;
      localParameterTranslations.col(0) = 0.8 * localParameterTranslations.col(0);

      return localParameterTranslations;
    }

    double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const {
      assert(parameter.n_elem == numberOfDimensions_);
        
      double maximalValue = std::numeric_limits<double>::lowest();
      for (arma::uword n = 0; n < 101; ++n) {
        const arma::Col<double>& locallyTranslatedParameter = parameter - localParameterTranslations_.col(n);
        maximalValue = std::max(maximalValue, weight_(n) * std::exp(-0.5 / static_cast<double>(numberOfDimensions_) * arma::dot(locallyTranslatedParameter, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(n)) * rotationQ_ * locallyTranslatedParameter)));
      }

      return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
    }

    std::string GallaghersGaussian101mePeaksFunction::toString() const {
      return "bbob_gallaghers_gaussian_101me_peaks_function";
    }

#if defined(SUPPORT_MPI)
    std::vector<double> GallaghersGaussian101mePeaksFunction::serialise() const {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark::serialise();
      
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      for(arma::uword n = 0; n < localParameterConditionings_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(localParameterConditionings_(n));
      }
      
      for(arma::uword n = 0; n < localParameterTranslations_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(localParameterTranslations_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    void GallaghersGaussian101mePeaksFunction::deserialise(
        std::vector<double> serialisedOptimisationProblem) {
      localParameterTranslations_.set_size(numberOfDimensions_, 21);
      for(arma::uword n = 0; n < localParameterTranslations_.n_elem; ++n) {
        localParameterTranslations_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
      
      localParameterConditionings_.set_size(numberOfDimensions_, 21);
      for(arma::uword n = 0; n < localParameterConditionings_.n_elem; ++n) {
        localParameterConditionings_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
      
      rotationQ_.set_size(numberOfDimensions_, numberOfDimensions_);
      for(arma::uword n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.back();
        serialisedOptimisationProblem.pop_back();
      }
        
      BlackBoxOptimisationBenchmark::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
