$$\begin{align}
F(X) &:=  - S \cdot Z + \sum_{i=1}^{N} 5 \left| S_i \right|\\
O &:= \text{Uniformly choosen from } \{(5,5,\ldots,5), (-5,-5,\ldots,-5)\}\\
S_i &:= T_{\text{scaled}, i}^\sqrt{10} \cdot \operatorname{sign}(O_i) \\
Z_i &:= \begin{cases}
X_i & \text{if } X_{i} \cdot O_i < 25 \\
O_i & \text{otherwise}
\end{cases}
\end{align}$$

**LinearSlope( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>void</small> .setOne( <small>bool</small> P )** {% include noexcept %}

- Parameterises the variable \\(O\\).
- All elements in \\(O\\) are either equal to 5 (P = `true`) or -5 (P = `false`).

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *linear-slope*.