#pragma once

#include <string>
#include <chrono>
//#include <iomanip> TODO Waiting for GCC to support std::put_time(...)

#include <armadillo>

#include <hop_bits/helper/runable.hpp>

namespace hop {
  class Evaluation : public Runable {
    public:
      Evaluation();

      Evaluation(const Evaluation&) = delete;
      Evaluation& operator=(const Evaluation&) = delete;

    protected:
      //TODO Waiting for GCC to support std::put_time(...)
      std::string evaluationPath_ = "./evaluation/evaluation_" +
                                    /* std::put_time(std::chrono::system_clock::now(), "%F") + */
                                    std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

      void save(arma::Mat<double> data, std::string fileSuffix) const;
  };
}
