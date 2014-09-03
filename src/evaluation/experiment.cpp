#include <hop_bits/evaluation/experiment.hpp>

#include <boost/filesystem.hpp>

namespace hop {
  Experiment::Experiment(std::string experiementFolder) : _experiementFolder(experiementFolder) {
    boost::filesystem::path path = boost::filesystem::path(_experiementFolder);

    if(boost::filesystem::exists(path)) {
      //TODO Add exception
    }

    boost::filesystem::create_directories(path);
  }
}
