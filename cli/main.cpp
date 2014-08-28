#include <cstdlib>
#include <memory>
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <hop>

#include <experiment.hpp>
#include <plot/plot2DFitnessFunction.hpp>

int main (const int argc, const char* argv[]) {
  if (argc < 2) {

  }

  std::shared_ptr<Experiment> plot2DFitnessFunction = std::shared_ptr<Experiment>(new Plot2DFitnessFunction(std::shared_ptr<hop::OptimisationProblem>(new hop::SphereFunction(2))));

  std::ofstream output("output"); {
    cereal::JSONOutputArchive archive(output);
    archive(cereal::make_nvp("experiement", plot2DFitnessFunction));
  } output.close();

  plot2DFitnessFunction->run();
  plot2DFitnessFunction->waitUntilFinished();

  std::shared_ptr<Experiment> experiment;
  std::ifstream input(argv[1]); {
    cereal::JSONInputArchive archive(input);
    archive(cereal::make_nvp("experiement", experiment));
  } input.close();

  experiment->run();
  experiment->waitUntilFinished();

  return EXIT_SUCCESS;
}
