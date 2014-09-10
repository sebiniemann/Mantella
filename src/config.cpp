#include <hop_bits/config.hpp>

namespace hop {
  decltype(Config::experimentPath_) Config::experimentPath_ = "";
  decltype(Config::gnuplotBinPath_) Config::gnuplotBinPath_ = "";
  decltype(Config::gnuplotScriptPath_) Config::gnuplotScriptPath_ = "../ext/gnuplot/";

  std::string Config::getExperimentPath() {
    return experimentPath_;
  }

  void Config::setExperimentPath(const std::string& experimentPath) {
    experimentPath_ = experimentPath;
  }

  std::string Config::getGnuplotBinPath() {
    return gnuplotBinPath_;
  }

  void Config::setGnuplotBinPath(const std::string& gnuplotBinPath) {
    gnuplotBinPath_ = gnuplotBinPath;
  }

  std::string Config::getGnuplotScriptPath() {
    return gnuplotScriptPath_;
  }

  void Config::setGnuplotScriptPath(const std::string& gnuplotScriptPath) {
    gnuplotScriptPath_ = gnuplotScriptPath;
  }
}
