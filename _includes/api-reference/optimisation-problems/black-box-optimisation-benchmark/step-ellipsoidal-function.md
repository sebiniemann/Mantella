$$\begin{align}
F(X) &:=  0.1 \max\left( \frac{\left| Z_1 \right|}{10000}, T_\text{scaled}^{100} \cdot Q \cdot \tilde{Z} \right) + F_\text{penality}(X)\\
Z &:= T_\text{scaled}^\sqrt{10} \cdot R \cdot \left( X - X^\text{opt} \right) \\
\tilde{Z} &:= \begin{cases}
\left\lfloor 0.5 + Z_i\right\rfloor & \text{if } Z_i > 0.5\\
\frac{\left\lfloor 0.5 + 10Z_i\right\rfloor}{10} & \text{otherwise}\\
\end{cases}
\end{align}$$

**StepEllipsoidalFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

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

- Returns a filesystem friendly name of the problem, i.e. *step-ellipsoidal-function*.


