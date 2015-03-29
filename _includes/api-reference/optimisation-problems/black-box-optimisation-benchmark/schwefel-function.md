$$\begin{align}
F(X) &:= 0.01 (418.9828872724339 - \frac{\sum_{i=1}^{N-1} \tilde{Z}_i \cdot \sin\left( \sqrt{\left| \tilde{Z}_i \right|} \right)}{N - 1})+ 100 F_\text{penality}\left( \frac{\tilde{Z}}{100} \right)\\
O &:= \text{Randomly and uniformly choosen from } \{(1,1,\ldots,1), (-1,-1,\ldots,-1)\}\\
X^\text{opt}_i &:= 4.2096874633 \left|O\right|\\
\tilde{X} &:= 2 O \circ X\\
Z_i &:= \begin{cases}
\tilde{X}_i + 0.25 \left( \tilde{X}_{i-1} - X^\text{opt}_{i-1} \right) & \text{for } i = 2, \ldots, N \\
\tilde{X}_i & \text{for } i = 1
\end{cases}\\
\tilde{Z} &:= 100 \left( T_\text{scaled}^\sqrt{10} \cdot \left( Z - X^\text{opt} \right) + X^\text{opt} \right)\\
\end{align}$$

**SchwefelFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 2 dimensions.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>void</small> .setOne( <small>bool</small> P )** {% include noexcept %}

- Parameterises the variable \\(O\\).
- All elements in \\(O\\) are either equal to 1 (P = `true`) or -1 (P = `false`).

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *schwefel-function*.