#include <plot/plot.hpp>

std::string Plot::getGnuplotBinPath() const {
  return _gnuplotBinPath;
}

void Plot::setGnuplotBinPath(const std::string& path) {
  _gnuplotBinPath = path;
}

std::string Plot::getGnuplotScriptPath() const {
  return _gnuplotScriptPath;
}

void Plot::setGnuplotScriptPath(const std::string& path) {
  _gnuplotScriptPath = path;
}
