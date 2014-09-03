#pragma once

#include <memory>

#include <hop_bits/evaluation/runable.hpp>
#include <hop_bits/evaluation/experiment.hpp>

namespace hop {
  class Analysis : public Runable {
    public:
      Analysis(const std::shared_ptr<Experiment>& experiment);

    protected:
      const std::shared_ptr<Experiment> _experiment;
  };
}
