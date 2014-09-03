#include <cstdlib>
#include <memory>
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <hop>

int main (const int argc, const char* argv[]) {
  if (argc < 2) {

  }

//  std::shared_ptr<Experiment> fitnessFunctionGridPlotter = std::shared_ptr<Experiment>(new FitnessFunctionGridPlotter(std::shared_ptr<hop::OptimisationProblem>(new hop::SphereFunction(2))));

//  std::ofstream output("output"); {
//    cereal::JSONOutputArchive archive(output);
//    archive(cereal::make_nvp("experiment", fitnessFunctionGridPlotter));
//  } output.close();

//  fitnessFunctionGridPlotter->run();
//  fitnessFunctionGridPlotter->waitUntilFinished();

//  std::shared_ptr<Experiment> experiment;
//  std::ifstream input(argv[1]); {
//    cereal::JSONInputArchive archive(input);
//    archive(cereal::make_nvp("experiment", experiment));
//  } input.close();

//  experiment->run();
//  experiment->waitUntilFinished();

  return EXIT_SUCCESS;
}
