#include <plotter/plotter.hpp>

#include <boost/filesystem.hpp>

#include <config.hpp>

void Plotter::plot(std::string plotData, std::string plotScript) const {
  boost::filesystem::path workingDirectory = boost::filesystem::current_path();
  boost::filesystem::current_path(Config::getGnuplotScriptPath());
  system((Config::getGnuplotBinPath() + "gnuplot -e \" filename= '...'\" " + plotScript + "Plotter.txt").c_str());
  boost::filesystem::current_path(workingDirectory);
}
