---
layout: api-reference
category: api-reference

title: API Reference
subtitle: Reference to Mantella's API
---

<h2 id="table-of-contents">Table of Contents</h2>

<div class="toc">
 - **<a href="#optimisation-algorithms">Optimisation Algorithms</a>**
   - <a href="#population-based-algorithms">Population-based Algorithms</a> <br />
     <a href="#stadard-particle-swarm-optimisation-2011">Standard Particle Swarm Optimisation 2011</a>,
     <a href="#role-based-imitation-algorithm">Role-based Imitation Algorithm</a>,
     <a href="#covariance-matrix-adaption-evolutionary-strategy">Covariance Matrix Adaption - Evolutionary Strategy</a>
     - <a href="#parallel-algorithms">Parallel Algorithms</a> <br />
       <a href="#parallel-standard-particle-swarm-optimisation-2011">Parallel Standard Particle Swarm Optimisation 2011</a>
   - <a href="#trajectory-based-algorithms">Trajectory-based Algorithms</a> <br />
     <a href="#hooke-jeeves-algorithm">Hooke-Jeeves Algorithm</a>,
     <a href="#simulated-annealing">Simulated Annealing</a>,
     <a href="#hill-climbing">Hill Climbing</a>
   - <a href="#sampling-based-algorithms">Sampling-based Algorithms</a> <br />
     <a href="#grid-search">Grid Search</a>,
     <a href="#multi-resolution-grid-search">Multi-resolution Grid Search</a>,
     <a href="#random-search">Random Search</a>
 - **<a href="#optimisation-problems">Optimisation Problems</a>**
   - <a href="#black-box-optimisation-benchmark">Black-box Optimisation Benchmark</a> (2009–2015) <br />
     <a href="#black-box-optimisation-benchmark-attractive-sector-function">Attractive Sector Function</a>,
     <a href="#black-box-optimisation-benchmark-bent-cigar-function">Bent Cigar Function,
     <a href="#black-box-optimisation-benchmark-bueche-rastrigin-function">Büche Rastrigin Function</a>,
     <a href="#black-box-optimisation-benchmark-composite-griewank-rosenbrock-function-f8f2">Composite Griewank Rosenbrock Function F8F2</a>,
     <a href="#black-box-optimisation-benchmark-different-powers-function">Different Powers Function</a>,
     <a href="#black-box-optimisation-benchmark-discus-function">Discus Function</a>,
     <a href="#black-box-optimisation-benchmark-ellipsoidal-function">Ellipsoidal Function</a>,
     <a href="#black-box-optimisation-benchmark-ellipsoidal-function-rotated">Ellipsoidal Function (rotated)</a>,
     <a href="#black-box-optimisation-benchmark-gallaghers-gassuian-101-me-peaks-function">Gallaghers Gaussian 101-me Peaks Function</a>,
     <a href="#black-box-optimisation-benchmark-gallaghers-gassuian-21-hi-peaks-function">Gallaghers Gaussian 21-hi Peaks Function</a>,
     <a href="#black-box-optimisation-benchmark-katsuura-function">Katsuura Function</a>,
     <a href="#black-box-optimisation-benchmark-linear-slope">Linear Slope</a>,
     <a href="#black-box-optimisation-benchmark-lunacek-bi-rastrigin-function">Lunacek Bi Rastrigin Function</a>,
     <a href="#black-box-optimisation-benchmark-rastrigin-function">Rastrigin Function</a>,
     <a href="#black-box-optimisation-benchmark-rastrigin-function-rotated">Rastrigin Function (rotated)</a>,
     <a href="#black-box-optimisation-benchmark-rosenbrock-function">Rosenbrock Function</a>,
     <a href="#black-box-optimisation-benchmark-rosenbrock-function-rotated">Rosenbrock Function (rotated)</a>,
     <a href="#black-box-optimisation-benchmark-schaffers-f7-function">Schaffers F7 Function</a>,
     <a href="#black-box-optimisation-benchmark-schaffers-f7-function-ill-conditioned">Schaffers F7 Function (ill-conditioned)</a>,
     <a href="#black-box-optimisation-benchmark-schwefel-function">Schwefel Function</a>,
     <a href="#black-box-optimisation-benchmark-sharp-ridge-function">Sharp Ridge Function</a>,
     <a href="#black-box-optimisation-benchmark-sphere-function">Sphere Function</a>,
     <a href="#black-box-optimisation-benchmark-step-ellipsoidal-function">Step Ellipsoidal Function</a>,
     <a href="#black-box-optimisation-benchmark-weierstrass-function">Weierstrass Function</a>
   - <a href="#robotic">Robotic</a> <br />
     <a href="#robotic-pose-accuracy">Pose Accuracy</a>
     - <a href="#robotic-2d-models">2D Models</a> <br />
       <a href="#robotic-3prpr">3(P)RPR</a>,
       <a href="#robotic-3prrr">3(P)RRR</a>
     - <a href="#robotic-3d-models">3D Models</a> <br />
       <a href="#robotic-3prus">3(P)RUS</a>,
       <a href="#robotic-3pups">3(P)UPS</a>,
       <a href="#robotic-4prus">4(P)RUS</a>,
       <a href="#robotic-6prus">6(P)RUS</a>,
       <a href="#robotic-6pups">6(P)UPS</a>,
       <a href="#multi-level-stewart-platform">Multi-level Stewart Platform</a>
 - **<a href="#distance-functions">Distance Functions</a>** <br />
   <a href="#euclidean-distance">Euclidean Distance</a>,
   <a href="#hamming-distance">Hamming Distance</a>,
   <a href="#infinity-norm">Infinity Norm</a>,
   <a href="#manhattan-distance">Manhattan Distance</a>
 - **<a href="#propertiy-analysis">Property Analysis</a>**
   - <a href="#passive-property-analysis">Passive Property Analysis</a>
     - <a href="#correlation-analysis">Correlation Analysis</a> <br />
       <a href="#fitness-distance-correlation-analysis">Fitness Distance Correlation Analysis</a>
     - <a href="#function-model-analysis">Function Model Analysis</a>
       - <a href="#linear-function-model-analysis">Linear Function Model Analysis</a> <br />
         <a href="#ordinary-least-squares-linear-function-model-analysis">Ordinary Least Squares Linear Function Model Analysis</a>
       - <a href="#quadratic-function-model-analysis">Quadratic Function Model Analysis</a> <br />
         <a href="#ordinary-least-squares-quadratic-function-model-analysis">Ordinary Least Squares Quadratic Function Model Analysis</a>
     - <a href="#continuity-analysis">Continuity Analysis</a>
       - <a href="#lipschitz-continuity-analysis">Lipschitz Continuity Analysis</a> <br />
         <a href="#direct-lipschitz-continuity-analysis">Direct Lipschitz Continuity Analysis</a>
   - <a href="#active-property-analysis">Active Property Analysis</a>
     - <a href="#separability-analysis">Separability Analysis</a>
       - <a href="#additive-separability-analysis">Additive Separability Analysis</a> <br />
         <a href="#direct-additive-separability-analysis">Direct Additive Separability Analysis</a>
       - <a href="#multiplicative-separability-analysis">Multiplicative Separability Analysis</a> <br />
         <a href="#direct-multiplicative-separability-analysis">Direct Multiplicative Separability Analysis</a>
     - <a href="#proportionality-analysis">Proportionality Analysis</a>
       - <a href="#linear-proportionality-analysis">Linear Proportionality Analysis</a> <br />
         <a href="#direct-linear-proportionality-analysis">Direct Linear Proportionality Analysis</a>
 - **<a href="#property-estimation">Property Estimation</a>** <br />
   <a href="#maximum-likelihood-estimation">Maximum Likelihood Estimation</a>
 - **<a href="#helper">Helper</a>** <br />
   <a href="#random-number-generator">Random Number Generator</a>,
   <a href="#filesystem">Filesystem</a>,
   <a href="#geometry">Geometry</a>,
   <a href="#quasi-random-sequences">Quasi Random Sequences</a>,
   <a href="#random-matrix-vector-generator">Random Matrix/Vector Generator</a>,
   <a href="#regression-analysis">Regression Analysis</a>,
   <a href="#set-theory">Set Theory</a>,
   <a href="#trajectories">Trajectories</a>,
   <a href="#printable">Printable</a>,
   <a href="#string">String</a>,
   <a href="#unordered-container">Unordered Container</a>
</div>
 