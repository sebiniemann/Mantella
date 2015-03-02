$$\begin{align}
F(X) &:= 10 \left(\frac{\sum_{i = 1}^{N} \frac{S_i}{4000} - \cos{S_i}}{N} + 1\right)\\
M &:= \max{1, \frac{\sqrt{n}}{8}}\\
Z &:= M \cdot R \cdot X + 0.5\\
S &:= 100 \left(\sum_{i = 1}^{N - 1} (Z_{i}^{2} - Z_{i + 1})\right)^{2}+\left(\sum_{i = 1}^{N-1} (1 - Z_{i})\right)^{2}
\end{align}$$

**CompositeGriewankRosenbrockFunctionF8F2( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 2 dimensions.

---
**<small>void</small> .setR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by variable \\(R\\).
- The rotation matrix must be orthonormal (\\(R^{t} = R^{-1}\\)) with determinant 1 or -1.

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *composite-griewank-rosenbrock-function-f8f2*.