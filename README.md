![Mantella](http://sebastianniemann.github.io/OnlineOptimisation/images/Mantella.png)

Mantella 0.9.0
====================================
[![Travis CI Build Status](https://travis-ci.org/SebastianNiemann/OnlineOptimisation.png?branch=master)](https://travis-ci.org/SebastianNiemann/OnlineOptimisation) [![Coverity Scan Build Status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) [![TODO](https://badge.waffle.io/sebastianniemann/onlineoptimisation.png?label=waffle:todo&title=ToDo)](https://waffle.io/sebastianniemann/onlineoptimisation)

The Mantella C++ optimisation library provides a wide range of tools to describe and analysis real-world online optimisation problems as well as competitive solvers and highly parallalised variants, efficiently used on recent multi-cores and high performance clusters. 

Mantella is written in C++11 and uses [Armadillo](http://arma.sourceforge.net/) (developed by Conrad Sanderson et al., NICTA, Australia) for high efficient linear algebra calculations as well as [Cereal](http://uscilab.github.io/cereal/) (developed by Shane Grant and Randolph Voorhies, iLab, USA) for serialisation and reproducibility.

Getting started
---------------
### Prerequirements (newer version should also work)
- CMake 2.8.8
- GCC 4.8.1 (or any other C++11 feature complete compiler)
- Armadillo C++ 4.600.0
- Cereal 1.0.0 (or develop, in case this bug affects you: https://github.com/USCiLab/cereal/issues/94)

#### Only needed for parallel algorithms
- MPI 3.0.0

#### Only needed for tests
- Catch (development version)
- Boost filesystem 1.54

### Installation
```bash
cd path/to/project
mkdir build
cd build
cmake ..
make
sudo make install
```

Future plans
------------
- **Planned for 1.0.0**
  - ~~Adding robots with a 3-dimensional workspace (6(P)RUS, 6(P)UPS and multi-level Stewart platforms)~~ **(done)**
  - ~~Adding objective function analysis (Multi-linear regression, quadratic regression, Lipschitz continuity)~~ **(done)**
  - Adding the Role-based imitation algorithm, developed by Emre Çakar
  - ~~Set installation of parallised implementations to be optional~~ **(done)**
  - ~~Adding a grid search algorithm~~ **(done)**
  - Adding a multi resolution grid search algorithm
- **Planned for 1.1.0**
  - Adding the Covariance Matrix Adaptation Evolution Strategy (in its most basic implementation at the beginning), developed by Nikolaus Hansen
  - Adding the Multilevel Coordinate Search Algorithm, developed by Waltraud Huyer
  - Adding the Nelder–Mead method, developed by John Nelder and Roger Mead
  - Adding parallel (MPI-based) sample-based algorithms
- **Planned for 1.2.0**
  - Adding a seperate website (besides this readme on Github), including a user documentation and a *getting started*
- **Backlog**
  - Adding the black-boxoptimisation benchmark 2015 (waiting for more information from Nikolaus Hansen)

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

Full support of the [black-box optimisation benchmark 2013](http://coco.gforge.inria.fr/doku.php?id=bbob-2013-downloads), developed by Nikolaus Hansen et al., INRIA, France
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

License
-------
Distributed under [MIT license](http://opensource.org/licenses/MIT).

Copyright (c) 2013-2014 [Sebastian Niemann](mailto:niemann@sra.uni-hannover.de) and contributors.
