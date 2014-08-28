#pragma once

#include <string>
#include <map>

class OptimisationProblem {
  public:
    enum class Type {
      AttractiveSectorFunction,
      BentCigarFunction,
      BuecheRastriginFunction,
      CompositeGriewankRosenbrockFunctionF8F2,
      DifferentPowersFunction,
      DiscusFunction,
      EllipsoidalFunction,
      EllipsoidalFunctionRotated,
      GallaghersGaussian101mePeaksFunction,
      GallaghersGaussian21hiPeaksFunction,
      KatsuuraFunction,
      LinearSlope,
      LunacekBiRastriginFunction,
      RastriginFunction,
      RastriginFunctionRotated,
      RosenbrockFunction,
      RosenbrockFunctionRotated,
      SchaffersF7Function,
      SchaffersF7FunctionIllConditioned,
      SchwefelFunction,
      SharpRidgeFunction,
      SphereFunction,
      StepEllipsoidalFunction,
      WeierstrassFunction
    };

    std::string getName(const Type& type) const;

  protected:
    static const std::map<Type, std::string> _names;
};
