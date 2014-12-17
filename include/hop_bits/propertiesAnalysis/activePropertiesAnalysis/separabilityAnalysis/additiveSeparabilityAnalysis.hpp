#pragma once

// HOP
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis.hpp>

namespace hop {
  template <typename ParameterType>
  class AdditiveSeparabilityAnalysis : public SeparabilityAnalysis<ParameterType> {
    public:
      explicit AdditiveSeparabilityAnalysis() noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      AdditiveSeparabilityAnalysis(const AdditiveSeparabilityAnalysis&) = delete;
      AdditiveSeparabilityAnalysis& operator=(const AdditiveSeparabilityAnalysis&) = delete;

      void setAdditiveSeparabilityErrorThreshold(
          const double& additiveSeparabilityErrorThreshold);

    protected:
      double additiveSeparabilityErrorThreshold_;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  AdditiveSeparabilityAnalysis<ParameterType>::AdditiveSeparabilityAnalysis() noexcept {
    setAdditiveSeparabilityErrorThreshold(0.75);
  }

  template <typename ParameterType>
  void AdditiveSeparabilityAnalysis<ParameterType>::setAdditiveSeparabilityErrorThreshold(
      const double& additiveSeparabilityErrorThreshold) {
    if(additiveSeparabilityErrorThreshold < 0 || additiveSeparabilityErrorThreshold > 1) {
      throw std::runtime_error("The additive separability error threshold (" + std::to_string(additiveSeparabilityErrorThreshold) + ") must be within 0 and 1.");
    }

    additiveSeparabilityErrorThreshold_ = additiveSeparabilityErrorThreshold;
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
