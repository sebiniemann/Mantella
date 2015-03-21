---
layout: api-reference

title: API Reference
subtitle: Reference to Mantella's API

table_of_contents:
  Optimisation Algorithms:
    Population-based Algorithms:
      Standard Particle Swarm Optimisation 2011:
      Role-based Imitation Algorithm:
      Covariance Matrix Adaptation - Evolution Strategy:
      Parallel Algorithms:
        Parallel Standard Particle Swarm Optimisation 2011:
    Trajectory-based Algorithms:
      Hooke-Jeeves Algorithm:
      Simulated Annealing:
      Hill Climbing:
    Sampling-based Algorithms:
      Grid Search:
      Multi-resolution Grid Search:
      Random Search:
  Optimisation Problems:
    Black-box Optimisation Benchmark:
      Attractive Sector Function:
      Bent Cigar Function:
      Büche-Rastrigin Function:
      Composite Griewank-Rosenbrock Function F8F2:
      Different Powers Function:
      Discus Function:
      Ellipsoidal Function:
      Ellipsoidal Function (rotated):
      Gallagher's Gaussian 101-me Peaks Function:
      Gallagher's Gaussian 21-hi Peaks Function:
      Katsuura Function:
      Linear Slope:
      Lunacek bi-Rastrigin Function:
      Rastrigin Function:
      Rastrigin Function (rotated):
      Rosenbrock Function:
      Rosenbrock Function (rotated):
      Schaffers F7 Function:
      Schaffers F7 Function (ill-conditioned):
      Schwefel Function:
      Sharp Ridge Function:
      Sphere Function:
      Step Ellipsoidal Function:
      Weierstrass Function:
    Robotic:
      Pose Accuracy:
      Models:
        Parallel Kinematic Machine 3(P)RPR:
        Parallel Kinematic Machine 3(P)RRR:
        Parallel Kinematic Machine 3(P)RUS:
        Parallel Kinematic Machine 3(P)UPS:
        Parallel Kinematic Machine 4(P)RUS:
        Parallel Kinematic Machine 6(P)RUS:
        Parallel Kinematic Machine 6(P)UPS:
        Multi-level Stewart Platform:
  Distance Functions:
    Euclidean Distance:
    Hamming Distance:
    Infinity Norm:
    Manhattan Distance:
  Properties:
    Correlation Property:
    Continuity Properties:
      Lipschitz Continuity Property:
    Function Model Properties:
      Linear Function Model Property:
      Quadratic Function Model Property:
    Separability Properties:
      Additive Separability Property:
      Multiplicative Separability Property:
    Proportionality Properties:
      Linear Proportionality Property:
  Property Analyses:
    Passive Property Analyses:
      Correlation Analyses:
        Fitness Distance Correlation Analysis:
      Continuity Analyses:
        Lipschitz Continuity Analyses:
          Direct Lipschitz Continuity Analysis:
      Function Model Analyses:
        Linear Function Model Analyses:
          Ordinary Least Squares Linear Function Model Analysis:
        Quadratic Function Model Analyses:
          Ordinary Least Squares Quadratic Function Model Analysis:
    Active Property Analyses:
      Separability Analyses:
        Additive Separability Analyses:
          Direct Additive Separability Analysis:
        Multiplicative Separability Analyses:
          Direct Multiplicative Separability Analysis:
      Proportionality Analyses:
        Linear Proportionality Analyses:
          Direct Linear Proportionality Analysis:
  Properties Estimations:
    Function Shape Estimation:
    Dimensionality Estimation:
  Helpers:
    Random Number Generator:
    Cereal:
    Filesystem:
    Geometry:
    Quasi Random Sequences:
    Random Matrix/Vector Generators:
    Regression Analyses:
    Set Theory:
    Trajectories:
    Printable:
    String:
    Unordered Containers:
---

<div class="custom-callout custom-callout-info">
#### Can this interface be used for my problem?

While we use many commen interfaces for continuous and discrete optimisation problems, some calculations will only sense for one of them.

<dl class="dl-horizontal" markdown="0">
  <dt>{% include continuous-only %}</dt>
  <dd>These interfaces can only be used for continuous optimisation problems.</dd>
  
  <dt>{% include discrete-only %}</dt>
  <dd>These interfaces can only be used for discrete optimisation problems.</dd>
</dl>

Interfaces marked without any specifiers can be used for all problem types.
</div>

<div class="custom-callout custom-callout-info">
#### Notable interface guarantees

<dl class="dl-horizontal" markdown="0">
  <dt>{% include noexcept %}</dt>
  <dd>This interface wont throw any exception, as it accepts all possible inputs.</dd>
</dl>
</div>