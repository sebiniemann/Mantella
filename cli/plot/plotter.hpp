#pragma once

#include <experiment.hpp>

class Plotter : public Experiment {
  protected:
    void plot(std::string plotScript) const;
};
