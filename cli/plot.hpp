#pragma once

#include <string>
#include <map>

class Plot {
  public:
    enum class Type {
      FitnessFunction
    };

    std::string getName(Type type);

  protected:
    static const std::map<Type, std::string> _names;
};
