$$\begin{align}
F(X) &:= 100 \sum_{i=1}^{N-1} \left( Z_i^2 - Z_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( Z_i + 1 \right)^2\\
Z &:= \max\left(1, \frac{\sqrt{N}}{8}\right) \cdot \left( X - X^\text{opt} \right) + 1
\end{align}$$

**RosenbrockFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 2 dimensions.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *rosenbrock-function*.