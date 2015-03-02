$$\begin{align}
F(X) &:= 1000000Z_{1} + \sum_{i = 2}^{N} \tilde{Z}_{i}\\
Z &:= T_\text{oscillated}\left(R \cdot \left(X - X^\text{opt}\right)\right)\\
\tilde{Z}_{i} &:= Z_{i}^{2}
\end{align}$$

**DiscusFunction( <small>unsigned int</small> N )**

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
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *discus-function*.