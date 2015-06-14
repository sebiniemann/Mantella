![Mantella](http://sebastianniemann.github.io/Mantella/assets/images/logo_with_name.png)

Version 0.9.0
=============

[![Travis CI Build Status](https://travis-ci.org/SebastianNiemann/Mantella.png?branch=master)](https://travis-ci.org/SebastianNiemann/Mantella) [![Coverity Scan Build Status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) [![Coverage Status](https://coveralls.io/repos/SebastianNiemann/Mantella/badge.svg?branch=master)](https://coveralls.io/r/SebastianNiemann/Mantella?branch=master)

Mantella is a modern, header-only C++ optimisation library, supporting Linux, Windows and Mac OS X. It provides a wide range of tools to describe and analysis real-world online optimisation problems as well as competitive, parallalised solvers, focusing on recent multi-cores architectures and high performance clusters. 

Mantella is written in C++11 and uses [Armadillo](http://arma.sourceforge.net/) (developed by Conrad Sanderson et al., NICTA, Australia) for highly efficient linear algebra calculations.

**Content:**
- [Getting started](#getting-started)
- [Got a Problem or Question?](#got-a-problem-or-question)
- [Found an Issue or Bug?](#found-an-issue-or-bug)
- [Requesting a Feature?](#requesting-a-feature)
- [License](#license)

<a name="getting-started"></a> Getting started
----------------------------------------------
### Prerequirements (newer version should also work)
- GCC 4.8.1, Clang 3.3 or any other C++11 feature complete compiler
- Armadillo C++ 4.650.0 (http://arma.sourceforge.net/download.html)

Installing Armadillo C++ on Ubuntu (visit http://arma.sourceforge.net/download.html to find installation instructions for other operating systems):
```bash
sudo apt-get install libblas-dev liblapack-dev
wget http://downloads.sourceforge.net/project/arma/armadillo-4.650.3.tar.gz
tar -xzf armadillo-4.650.3.tar.gz
cd armadillo-4.650.3
cmake .
make
sudo make install
```

### Installation
Mantella is a header-only library, you just need to download the latest stable version and place the header files into a directory standardly searched by your compiler.
```bash
git clone --depth 1 --branch master https://github.com/SebastianNiemann/Mantella.git
sudo cp -R Mantella/include/* /usr/local/include/
```
**Installation completed.** You can now start creating applications with Mantella.

### Usage
Create a new source file called **hello.cpp** on your computer.
```cpp
#include <iostream>
#include <mantella>

int main() {
  // 1. Setup the optimisation problem.
  unsigned int numberOfDimensions = 10;
  std::shared_ptr<mant::OptimisationProblem<>> optimisationProblem(new mant::bbob::SphereFunction<>(numberOfDimensions));

  // 2. Run the solver.
  mant::HookeJeevesAlgorithm<> optimisationAlgorithm(optimisationProblem);
  optimisationAlgorithm.optimise();

  // 3. Get your results!
  std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << '\n';
  std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << '\n';
  std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << '\n';
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << '\n';
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << '\n';
  std::cout << "bestSoftConstraintsValue: " << optimisationAlgorithm.getBestSoftConstraintsValue() << '\n';
  std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << '\n';
  std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;

  return 0;
}
```

Compile and build an executable from your source.
```bash
c++ -std=c++11 hello.cpp -larmadillo -o hello
# In case Armadillo and Mantella were not installed/placed into 
# standardly included/searched system directories, use the following
c++ -std=c++11 hello.cpp -L/path/to/armaillo/lib -larmadillo -I/path/to/armadillo/include -I/path/to/mantella/include -o hello
```

Run your application.
```bash
./hello
```

Output (may vary due to random initialised problems and algorithms):
```
isFinished: 1
isTerminated: 0
numberOfIterations: 697
numberOfEvaluations: 697
numberOfDistinctEvaluations: 586
bestSoftConstraintsValue: 0
bestObjectiveValueDelta: -2.42667e-09
bestParameter:    2.7208
  -0.8456
   2.2640
   2.3872
   3.2928
  -2.4200
  -1.3184
   2.1456
  -1.7784
   0.4313
```

<a name="got-a-problem-or-question"></a> Got a Problem or Question?
-------------------------------------------------------------------
If you have a question about how to use the Mantella C++ optimisation library, please direct these to [StackOverflow](http://stackoverflow.com/questions/tagged/mantella).

<a name="found-an-issue-or-bug"></a> Found an Issue or Bug?
-----------------------------------------------------------
If you found a bug in the source code or a mistake in any kind of documentation, please **let us know** by adding an issue to the [Github Issue Tracker](https://github.com/SebastianNiemann/Mantella/issues).

You are welcomed to submit a pull request with your fix afterwards, if at hand.

<a name="requesting-a-feature"></a> Requesting a Feature?
---------------------------------------------------------
If you are missing some features within Mantella, feel free to ask us about it by adding a new request to the [Github Issue Tracker](https://github.com/SebastianNiemann/Mantella/issues) labelled `feature request`.

Note that submitting a pull request, providing the needed changes to introduced your requested feature, usually speeds up the process.

<a name="license"></a> License
------------------------------
Distributed under [MIT license](http://opensource.org/licenses/MIT).

Copyright (c) 2013-2015 [Sebastian Niemann](mailto:niemann@sra.uni-hannover.de) and contributors.
