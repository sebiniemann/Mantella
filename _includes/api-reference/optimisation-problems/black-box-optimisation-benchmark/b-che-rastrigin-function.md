$$\begin{align}
F(X) &:= 10 n - \sum_{i = 1}^{n} \left( \cos{2 \pi S_{i}} \right) + \left\Vert S \right\Vert_{2}^{2} + 100 \cdot F_\text{penality}(X)\\
Z &:= T_\text{scaled}^\sqrt{10} \cdot T_\text{oscillated}\left( X - X^\text{opt} \right)\\
S_{i} &:= \begin{cases}
10 Z_{i} & \text{if } i \text{ is odd and } Z_{i} > 0\\
Z_{i} & \text{otherwise}
\end{cases}
\end{align}$$

**BuecheRastriginFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).
- All uneven (1, 3, 5, ...) dimensions are set to their absolute value.

$$X_{i}^\text{opt} := \begin{cases}
\left|X_{i}\right| & \text{if } i \text{ is odd}\\
X_{i} & \text{otherwise}
\end{cases}$$

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *bueche-rastrigin-function*.