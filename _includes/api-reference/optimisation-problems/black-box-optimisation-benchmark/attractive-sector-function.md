$$\begin{align}
F(X) &:= T_\text{oscillated}\left(||S||_{2}^{2}\right)^{0.9} \\
Z &:= Q \cdot T_\text{scaled}^{10} \cdot R\left(X - X^\text{opt}\right)\\
S_i &:= \begin{cases}
100 Z_{i} & \text{if } Z_{i} \times X_{i}^\text{opt} > 0 \\
Z_{i} & \text{otherwise}
\end{cases}
\end{align}$$

**AttractiveSectorFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>void</small> .setR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by variable \\(R\\).
- The rotation matrix must be orthonormal (\\(R^{t} = R^{-1}\\)) with determinant 1 or -1.

---
**<small>void</small> .setQ( <small>arma::Mat&lt;T&gt;</small> Q )**

- Parameterises the rotation by variable \\(Q\\).
- The rotation matrix must be orthonormal (\\(Q^{t} = Q^{-1}\\)) with determinant 1 or -1.

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns the name of the problem, i.e. *Attractive-sector-function*.