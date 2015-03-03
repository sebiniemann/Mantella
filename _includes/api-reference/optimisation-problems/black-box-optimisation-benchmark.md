Abstract base class for the black-box optimisation benchmark.

$$\min_{X \in [L, U]} F(X)$$

$$\begin{align}
F &:= \text{Any objective function, e.g. the sphere function.}\\
L &:= \text{The lower bound of the search space.}\\
U &:= \text{The upper bound of the search space.}
\end{align}$$

- Separable functions<br>
  <a href="#sphere-function">Sphere function</a>, <a href="#ellipsoidal-function">Ellipsoidal function function</a>, <a href="#rastrigin-function">Rastrigin function</a>, <a href="#b-che-rastrigin-function">Büche-Rastrigin function</a>, <a href="#linear-slope">Linear Slope</a>
- Functions with low or moderate conditioning<br>
  <a href="#attractive-sector-function">Attractive sector function</a>, <a href="#step-ellipsoidal-function">Step ellipsoidal function</a>, <a href="#rosenbrock-function">Rosenbrock function</a>, <a href="#rosenbrock-function-rotated">Rosenbrock function (rotated)</a>
- Unimodal functions with high conditioning<br>
  <a href="#ellipsoidal-function-rotated">Ellipsoidal function (rotated)</a>, <a href="#discus-function">Discus function</a>, <a href="#bent-cigar-function">Bent cigar function</a>, <a href="#sharp-ridge-function">Sharp ridge function</a>, <a href="#different-powers-function">Different powers function</a>
- Multi-modal functions with adequate global structure<br>
  <a href="#rastrigin-function-rotated">Rastrigin function (rotated)</a>, <a href="#weierstrass-function">Weierstrass function</a>, <a href="#schaffers-f7-function">Schaffers F7 function</a>, <a href="#schaffers-f7-function">Schaffers F7 function (ill-conditioned)</a>, <a href="#composite-griewank-rosenbrock-function-f8f2">Composite Griewank-Rosenbrock function F8F2</a>
- Multi-modal functions with weak global structure<br>
  <a href="#schwefel-function">Schwefel function</a>, <a href="#gallagher-s-gaussian-101-me-peaks-function">Gallagher's Gaussian 101-me peaks function</a>, <a href="#gallagher-s-gaussian-21-hi-peaks-function">Gallagher's Gaussian 21-hi peaks function</a>, <a href="#katsuura-function">Katsuura function</a>, <a href="#lunacek-bi-rastrigin-function">Lunacek bi-Rastrigin function</a>