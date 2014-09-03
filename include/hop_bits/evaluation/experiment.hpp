#pragma once

#include <string>

#include <hop_bits/evaluation/runable.hpp>

namespace hop {
  class Experiment : public Runable {
    public:
      Experiment(std::string folder);

    protected:
      std::string _experiementFolder;
  };
}
