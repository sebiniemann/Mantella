#include <plot.hpp>

decltype(Plot::_names) Plot::_names = {
  {Plot::Type::FitnessFunction, "Fitness function"}
};

std::string Plot::getName(Type type) {
  return _names.at(type);
}
