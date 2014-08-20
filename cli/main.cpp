#include <cstdlib>
// EXIT_SUCCESS

#include <iostream>
using std::cout;
using std::endl;

#include <functional>
using std::function;

#include <tuple>
using std::tuple;
using std::make_tuple;

#include <string>
using std::string;

#include <memory>
using std::unique_ptr;

#include <armadillo>
using arma::randu;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>
using hop::BenchmarkProblem;

#include <optimisationProblem/benchmark/sphereFunction.hpp>
using hop::SphereFunction;

#include <console.hpp>

void helloWorld(vector<string> arguments);

int main(const int argc, char* argv[]) {
  Console console;
  console.addCommand(make_tuple("helloWorld", function<void(vector<string>)>(helloWorld), "Simple hello world program"));

  while(console.isRunning()) {
    console.parseInput();
  }

  return EXIT_SUCCESS;
}

void helloWorld(vector<string> arguments) {
  cout << "helloWorld: " << endl;
  for(const auto& argument : arguments) {
    cout << argument << endl;
  }

  unsigned int numberOfDimensions = 2;
  unique_ptr<BenchmarkProblem> benchmarkProblem(new SphereFunction(numberOfDimensions));

  for (unsigned int k = 0; k < 10; k++) {
    cout << k << ": " << benchmarkProblem->getObjectiveValue(randu<Col<double>>(numberOfDimensions) * 10 - 5) << endl;
  }
}
