![Mantella](http://sebastianniemann.github.io/Mantella/assets/image/mantella.png)

Version 0.9.0
=============

[![Travis CI Build Status](https://travis-ci.org/SebastianNiemann/Mantella.png?branch=master)](https://travis-ci.org/SebastianNiemann/Mantella) [![Coverity Scan Build Status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) [![Coverage Status](https://coveralls.io/repos/SebastianNiemann/Mantella/badge.svg?branch=master)](https://coveralls.io/r/SebastianNiemann/Mantella?branch=master)


Mantella is a modern, header-only C++ optimisation library. It provides a wide range of tools to describe and analysis real-world online optimisation problems as well as competitive, parallalised solvers, focusing on recent multi-cores architectures and high performance clusters. 

Mantella is written in C++11 and uses [Armadillo](http://arma.sourceforge.net/) (developed by Conrad Sanderson et al., NICTA, Australia) for highly efficient linear algebra calculations.

Getting started
---------------
### Prerequirements (newer version should also work)
- GCC 4.8.1, Clang 3.3 or any other C++11 feature complete compiler
- Armadillo C++ 4.600.0 (http://arma.sourceforge.net/download.html)

```bash
sudo apt-get install libopenblas-dev liblapack-dev
wget -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-4.600.3.tar.gz
tar -xzf armadillo-4.600.3.tar.gz
cd armadillo-4.600.3
cmake .
make
sudo make install
``` 

### Installation
```bash
git clone --depth 1 --branch master https://github.com/SebastianNiemann/Mantella.git
sudo cp -R Mantella/include/* /usr/local/include/
```

### Usage
Create a new source file called **hello.cpp** on your computer.
```cpp
#include <iostream>
#include <mantella>

int main() {
  // 1. Setup the optimisation problem.
  unsigned int numberOfDimensions = 4;
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new mant::bbob2015::SphereFunction(numberOfDimensions));

  // 2. Run the solver.
  mant::HookeJeevesAlgorithm<mant::EuclideanDistance> optimisationAlgorithm(optimisationProblem);
  optimisationAlgorithm.optimise();

  // 3. Get your results!
  std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
  std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
  std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
  std::cout << "bestSoftConstraintsValue: " << optimisationAlgorithm.getBestSoftConstraintsValue() << std::endl;
  std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
  std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;
    
  return 1;
}
```

Compile and build an executable from your source.
```bash
c++ -std=c++11 hellp.cpp -larmadillo -o hello
```

Run your application.
```bash
./hello
```
Output (may vary due to random initialied problems and algorithms):
```
isFinished: 1
isTerminated: 0
numberOfIterations: 263
numberOfEvaluations: 263
numberOfDistinctEvaluations: 195
bestSoftConstraintsValue: 0
bestObjectiveValueDelta: -4.81944e-09
bestParameter:    2.7208
  -0.8456
   2.2640
   2.3872
```

Implemented features
--------------------
### Optimisation Algorithms
- **Trajectory-based algorithms**
  - Hill Climbing
  - Pure random search
  - Hooke Jeeves algorithm
- **Populations-based algorithms**
  - Standard particle swarm optimisation 2011
  - Parallel standard particle swarm optimisation 2011 (**MPI** support)

### Optimisation Problems
Support of real-world robotic optimisation problems
- **Optimisation problems (supported by each mechanism)**
  - Position error minimisation
- **Mechanisms**
  -  2-dimensional 
    - Parallel kinematic machine 3(P)RRR
    - Parallel kinematic machine 3(P)RPR
  - 3-dimensional
    - Parallel kinematic machine 6(P)RPR
    - Parallel kinematic machine 6(P)RPR
    - Multi-level Stewart platforms

Full support of the [black-box optimisation benchmark (from 2009–2015)](http://coco.gforge.inria.fr), developed by Nikolaus Hansen et al., INRIA, France
- **Separable functions**
  - Sphere function
  - Ellipsoidal function
  - Rastrigin function
  - Büche-Rastrigin function
  - Linear Slope
- **Functions with low or moderate conditioning**
  - Attractive sector function
  - Step ellipsoidal function
  - Rosenbrock function
  - Rosenbrock function, rotated
- **Functions with high conditioning and unimodal**
  - Ellipsoidal function, rotated
  - Discus function
  - Bent Cigar function
  - Sharp ridge function
  - Different powers function
- **Multi-modal functions with adequate global structure**
  - Rastrigin function
  - Weierstraß function
  - Schaffers F7 function
  - Schaffers F7 function, moderately ill-conditioned
  - Composite Griewank-Rosenbrock function F8F2
- **Multi-modal functions with weak global structure**
  - Schwefel function
  - Gallagher's Gaussian 101-me peaks function
  - Gallagher's Gaussian 21-hi peaks function
  - Katsuura function
  - Lunacek bi-Rastrigin function

Roadmap
-------
- **Planned for 1.0.0**
  - ~~Adding robots with a 3-dimensional workspace (6(P)RUS, 6(P)UPS and multi-level Stewart platforms)~~ **(done)**
  - ~~Adding objective function analysis (Multi-linear regression, quadratic regression, Lipschitz continuity)~~ **(done)**
  - Adding the Role-based imitation algorithm, developed by Emre Çakar
  - ~~Set installation of parallised implementations to be optional~~ **(done)**
  - ~~Adding a grid search algorithm~~ **(done)**
  - Adding a multi resolution grid search algorithm
- **Planned for 1.1.0**
  - ~~Adding the Covariance Matrix Adaptation Evolution Strategy (in its most basic implementation at the beginning), developed by Nikolaus Hansen~~ **(done)**
  - Adding the Multilevel Coordinate Search Algorithm, developed by Waltraud Huyer
  - Adding the Nelder–Mead method, developed by John Nelder and Roger Mead
  - Adding parallel (MPI-based) sample-based algorithms
- **Planned for 1.2.0**
  - Adding a seperate website (besides this README on Github), including a user documentation and a *getting started*
- **Backlog**
  - ~~Adding the black-boxoptimisation benchmark 2015 as well as 2009, 2010 and 2012~~ **(done)**

License
-------
Distributed under [MIT license](http://opensource.org/licenses/MIT).

Copyright (c) 2013-2015 [Sebastian Niemann](mailto:niemann@sra.uni-hannover.de) and contributors.
