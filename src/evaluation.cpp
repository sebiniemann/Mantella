#include <hop_bits/evaluation.hpp>

#include <boost/filesystem.hpp>

namespace hop {
  Evaluation::Evaluation() {
    boost::filesystem::path path = boost::filesystem::path(evaluationPath_);

    if(boost::filesystem::exists(path)) {
      //TODO Add exception
    }

    boost::filesystem::create_directories(path);
  }

  std::string Evaluation::getEvaluationPath() const {
    return evaluationPath_;
  }

  void Evaluation::save(arma::Mat<double> data, std::string fileSuffix) const {
    boost::filesystem::path filepath = boost::filesystem::path(evaluationPath_ + "/" + fileSuffix + ".mat");

    if(boost::filesystem::exists(filepath)) {
      //TODO Add exception
    }

    data.save(filepath.string(), arma::raw_ascii);
  }
}
