#pragma once

#include <string>

#include <experiment.hpp>

class Plot : public Experiment {
  public:
    std::string getGnuplotBinPath() const;
    void setGnuplotBinPath(const std::string& path);

    std::string getGnuplotScriptPath() const;
    void setGnuplotScriptPath(const std::string& path);

  protected:
    std::string _gnuplotBinPath = "";
    std::string _gnuplotScriptPath = "../ext/gnuplot/";
};
