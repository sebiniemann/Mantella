Hint-based Online Programming 0.9.0
====================================
[![Travis CI Build Status](https://travis-ci.org/SebastianNiemann/OnlineOptimisation.png?branch=master)](https://travis-ci.org/SebastianNiemann/OnlineOptimisation) [![Coverity Scan Build Status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) [![TODO](https://badge.waffle.io/sebastianniemann/onlineoptimisation.png?label=waffle:todo&title=ToDo)](https://waffle.io/sebastianniemann/onlineoptimisation)

The Hint-based Online Programming (HOP) library aims to provide the necessary tools to describe and analysis real-world online optimisation problems as well as (competitive) solvers to reduce the overall computation time on recent multi-cores and high performance clusters.

HOP is based on recent C++11 features and Armadillo, a high efficient linear algebra library. We are also implementing highly parallalised, competitive optimisation algorithms to maximise the speedup on multi-cores or high performance clusters. 

Getting started
---------------
### Installation
- **Requirements** (newer version should also work)
  - CMake 2.8.8
  - GCC 4.8.1 (or any other C++11 featur complete compiler)
  - MPI-2
  - Cereal 1.0.0
  - Catch (development version)
  - Boost filesystem 1.54

Future plans
------------
- **Planned for 1.0.0**
  - ~~Adding robots with a 3-dimensional workspace (6(P)RUS, 6(P)UPS and multi-level Stewart platforms)~~ **(done)**
  - Adding objective function analysis (Multi-linear regression, quadratic regression, Lipschitz continuity)
  - Adding the Role-based imitation algorithm, developed by Emre Çakar
  - Set installation of parallised implementations to be optional (but still activated as default)
- **Planned for 1.1.0**
  - Adding the Covariance Matrix Adaptation Evolution Strategy (in its most basic implementation), developed by Nikolaus Hansen
- **Backlog**
  - Adding the black-boxoptimisation benchmark 2015 (waiting for more information until 01.11.2014)

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
