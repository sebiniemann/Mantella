$$\begin{align}
F(X) &:= \min\left( \sum_{i=1}^{N} \left( \tilde{X}_i - 2.5 \right)^2, N + S \sum_{i=1}^{N} \left( \tilde{X}_i - \mu \right)^2 \right) + 10 \left( N - \sum_{i=1}^N \cos\left( 2 \pi Z_i \right)\right) + 10000 F_\text{penality}(X)\\
S &:= 1 - \frac{0.5}{\sqrt{N + 20} - 4.1}\\
\mu &:= -\sqrt{5.25 / S}\\
O &:= \text{Randomly and uniformly choosen from } \{(2.5,2.5,\ldots,2.5), (-2.5,-2.5,\ldots,-2.5)\}\\
\tilde{X}_i &:= 2 \operatorname{sign}(O_i) \cdot X_i\\
Z &:= Q \cdot T_\text{scaled}^\sqrt{10} \cdot R \cdot \left( \tilde{X} - 2.5 \right)\\
\end{align}$$

**LunacekBiRastriginFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setOne( <small>bool</small> P )** {% include noexcept %}

- Parameterises the variable \\(O\\).
- All elements in \\(O\\) are either equal to 2.5 (P = `true`) or -2.5 (P = `false`).

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

- Returns a filesystem friendly name of the problem, e.g. *lunacek-bi-rastrigin-function*.