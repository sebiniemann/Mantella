#pragma once

#include <string>
#include <chrono>
#include <ostream>
//#include <iomanip> TODO Waiting for GCC to support std::put_time(...)

#include <armadillo>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <hop_bits/helper/runable.hpp>
#include <hop_bits/helper/random.hpp>

namespace hop {
  class Evaluation : public Runable {
    public:
      Evaluation();

      Evaluation(const Evaluation&) = delete;
      Evaluation& operator=(const Evaluation&) = delete;

      std::string getEvaluationPath() const;

    protected:
      //TODO Waiting for GCC to support std::put_time(...)
      std::string evaluationPath_ = "./evaluation/evaluation_" +
                                    /* std::put_time(std::chrono::system_clock::now(), "%F") + */
                                    std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
      void save(arma::Mat<double> data, std::string fileSuffix) const;

      friend class cereal::access;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("majorVersion", 0));
        archive(cereal::make_nvp("minorVersion", 9));
        archive(cereal::make_nvp("hotfixVersion", 0));

        std::stringstream RngSeed;
        RngSeed << Random::Rng;
        archive(cereal::make_nvp("randomNumberGeneratorState", RngSeed.str()));
      }

    private:
      unsigned int majorVersion = 0;
      unsigned int minorVersion = 9;
      unsigned int hotfixVersion = 0;
  };
}
