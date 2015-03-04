$$\begin{align}
F(X) &:= \left( \frac{\sum_{i=1}^{N-1} S_i \cdot \left(1 + \sin\left(50 S_i^{0.4}\right)^2\right)}{N - 1} \right)^2 + 10 F_\text{penality}(X)\\

Z &:= T_\text{scaled}^\sqrt{1000} \cdot Q \cdot T_\text{asymmetric}^{0.5}\left( R \cdot \left( X - X^\text{opt} \right) \right)\\
S_i &:= \left(Z_i^2 + Z_{i+1}^2 \right)^{0.25}
\end{align}$$

**SchaffersF7FunctionIllConditioned( <small>unsigned int</small> N )**

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
**<small>void</small> .setQ( <small>arma::Mat&lt;T&gt;</small> Q )**

- Parameterises the rotation by variable \\(Q\\).
- The rotation matrix must be orthonormal \\(\left( Q^{t} = Q^{-1} \right)\\) with determinant 1 or -1.

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *schaffers-f7-function-ill-conditioned*.