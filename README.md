OnlineOptimisation 0.9.0
==================
[![Travis CI Build Status](https://travis-ci.org/SebastianNiemann/OnlineOptimisation.png?branch=master)](https://travis-ci.org/SebastianNiemann/OnlineOptimisation) [![Coverity Scan Build Status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) [![TODO](https://badge.waffle.io/sebastianniemann/onlineoptimisation.png?label=waffle:todo&title=ToDo)](https://waffle.io/sebastianniemann/onlineoptimisation)

This library aims to provide the necessary tools to describe and analysis real-world online optimisation problems as well as (competitive) solvers to reduce the overall computation time on recent multi-cores and high performance clusters.

Future plans
------------
- **Planned for 1.0.0**
  - Adding support for robots with a 3-dimensional workspace (6(P)RUS, 6(P)UPS and multi-level Stewart platforms)
  - Adding Support for objective function analysis (Multi-linear regression, quadratic regression, Lipschitz continuity)
  - Adding the Role-based imitation algorithm, developed by Emre Çakar
  - Adding the Covariance Matrix Adaptation Evolution Strategy (in its most basic implementation), developed by Nikolaus Hansen
- **Backlog**
  - Adding support for the black-box  optimisation benchmark 2014 (waiting for more information until 01.11.2014)

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

Support of real-world robotic optimisation problems
- **Position error minimisation**
  - Parallel kinematic machine 3(P)RRR
  - Parallel kinematic machine 3(P)RPR

License
-------
Distributed under [MIT license](http://opensource.org/licenses/MIT).

Copyright (c) 2013-2014 [Sebastian Niemann](mailto:niemann@sra.uni-hannover.de) and contributors.
