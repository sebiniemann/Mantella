#pragma once

#include <string>

class Config {
  public:
    static std::string getExperimentPath();
    static void setExperimentPath(const std::string& experimentPath);

    static std::string getGnuplotBinPath();
    static void setGnuplotBinPath(const std::string& gnuplotBinPath);

    static std::string getGnuplotScriptPath();
    static void setGnuplotScriptPath(const std::string& gnuplotScriptPath);

  protected:
    static std::string _experimentPath;

    static std::string _gnuplotBinPath;
    static std::string _gnuplotScriptPath;
};
