$$\begin{align}
F(X) &:= Z_{1} + 1000000 \sum_{i = 2}^{N} Z_{i}\\
Z &:= \left( R \cdot T_\text{asymmetric}^{0.5} \cdot \left( R \cdot \left( X - X^\text{opt} \right) \right) \right)^{2}\\
\end{align}$$

**BentCigarFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 2 dimensions.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>void</small> .setR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by variable \\(R\\).
- The rotation matrix must be orthonormal \\(\left( R^{t} = R^{-1} \right)\\) with determinant 1 or -1.

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *bent-cigar-function*.