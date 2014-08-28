#include <config.hpp>

decltype(Config::_experimentPath) Config::_experimentPath = "";
decltype(Config::_gnuplotBinPath) Config::_gnuplotBinPath = "";
decltype(Config::_gnuplotScriptPath) Config::_gnuplotScriptPath = "../ext/gnuplot/";

std::string Config::getExperimentPath() {
  return _experimentPath;
}

void Config::setExperimentPath(const std::string& experimentPath) {
  _experimentPath = experimentPath;
}

std::string Config::getGnuplotBinPath() {
  return _gnuplotBinPath;
}

void Config::setGnuplotBinPath(const std::string& gnuplotBinPath) {
  _gnuplotBinPath = gnuplotBinPath;
}

std::string Config::getGnuplotScriptPath() {
  return _gnuplotScriptPath;
}

void Config::setGnuplotScriptPath(const std::string& gnuplotScriptPath) {
  _gnuplotScriptPath = gnuplotScriptPath;
}
