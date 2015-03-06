$$\begin{align}
F(X) &:=  10 \left( \frac{\sum_{i=1}^N \sum_{j=0}^{11} 0.5^j \cos\left( 2 \pi 3^j \left(Z_i + 0.5 \right) \right)}{N}  - F_0 \right)^3 + \frac{10 F_\text{penality}(X)}{N}\\
Z &:= R \cdot T_\text{scaled}^\frac{1}{10} \cdot Q \cdot T_\text{oscillated} \left( R \cdot \left( X - X^\text{opt} \right) \right) \\
F_0 &:= \sum_{i=0}^{11} 0.5^i \cos\left( \pi 3^j \right)
\end{align}$$

**WeierstrassFunction( <small>unsigned int</small> N )**

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

- Returns a filesystem friendly name of the problem, i.e. *weierstrass-function*.


