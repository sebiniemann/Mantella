---
layout: api-reference

title: API Reference
subtitle: Reference to Mantella's API

table_of_contents:
  Optimisation Algorithms:
    subsection:
      Population-based Algorithms:
        subsection:
          Standard Particle Swarm Optimisation 2011:
          Role-based Imitation Algorithm:
          Covariance Matrix Adaptation - Evolution Strategy:
          Parallel Algorithms:
            subsection:
              Parallel Standard Particle Swarm Optimisation 2011:
      Trajectory-based Algorithms:
        subsection:
          Hooke-Jeeves Algorithm:
          Simulated Annealing:
          Hill Climbing:
      Sampling-based Algorithms:
        subsection:
          Grid Search:
          Multi-resolution Grid Search:
          Random Search:
  Optimisation Problems:
    subsection:
      Black-box Optimisation Benchmark:
        namespace: bbob2009
        subsection:
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
        namespace: robotic
        subsection:
          Pose Accuracy:
          Models:
            subsection:
              Parallel Kinematic Machine 3PRPR:
              Parallel Kinematic Machine 3PRRR:
              Parallel Kinematic Machine 3PRUS:
              Parallel Kinematic Machine 3PUPS:
              Parallel Kinematic Machine 4PRUS:
              Parallel Kinematic Machine 6PRUS:
              Parallel Kinematic Machine 6PUPS:
              Multi-level Stewart Platform:
  Distance Functions:
    subsection:
      Euclidean Distance:
      Hamming Distance:
      Infinity Norm:
      Manhattan Distance:
  Properties:
    subsection:
      Correlation Property:
      Continuity Properties:
        subsection:
          Lipschitz Continuity Property:
      Function Model Properties:
        subsection:
          Linear Function Model Property:
          Quadratic Function Model Property:
      Separability Properties:
        subsection:
          Additive Separability Property:
          Multiplicative Separability Property:
      Proportionality Properties:
        subsection:
          Linear Proportionality Property:
  Property Analyses:
    subsection:
      Passive Property Analyses:
        subsection:
          Correlation Analyses:
            subsection:
              Fitness Distance Correlation Analysis:
          Continuity Analyses:
            subsection:
              Lipschitz Continuity Analyses:
                subsection:
                  Direct Lipschitz Continuity Analysis:
          Function Model Analyses:
            subsection:
              Linear Function Model Analyses:
                subsection:
                  Ordinary Least Squares Linear Function Model Analysis:
              Quadratic Function Model Analyses:
                subsection:
                  Ordinary Least Squares Quadratic Function Model Analysis:
      Active Property Analyses:
        subsection:
          Separability Analyses:
            subsection:
              Additive Separability Analyses:
                subsection:
                  Direct Additive Separability Analysis:
            Multiplicative Separability Analyses:
              subsection:
                Direct Multiplicative Separability Analysis:
          Proportionality Analyses:
            subsection:
              Linear Proportionality Analyses:
                subsection:
                  Direct Linear Proportionality Analysis:
  Properties Estimations:
    subsection:
      Function Shape Estimation:
      Dimensionality Estimation:
  Helpers:
    subsection:
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

While we use many commen interfaces for continuous and discrete optimisation problems, some calculations will only make sense for one of them.

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
  <dd>This interface won't throw any exception, as it accepts all possible inputs.</dd>
</dl>
</div>