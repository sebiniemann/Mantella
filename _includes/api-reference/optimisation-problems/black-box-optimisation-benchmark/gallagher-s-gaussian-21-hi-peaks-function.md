$$\begin{align}
F(X) &:= T_\text{oscillated} \left( 10 - \max_{i = 1}^{21} \left( W_i \exp \left(-\frac{1}{2N} \left( X - Y_{i} \right) \cdot \left(R^T \cdot C_{i} \cdot R \cdot \left( X - Y_{i} \right) \right) \right) \right) \right)^{2} + F_\text{penality}(X)\\
W_i &:= \begin{cases}
1.1 + 8 \cdot \frac{i - 2}{19} & \text{for } i = 2, \ldots, 21 \\
10 & \text{for  } i = 1
\end{cases}\\
A_i &:= \begin{cases}
\text{Uniformly drawn from } \left\{ 1000^{\frac{2j}{19}} : j \in \left\{ 0, \ldots, 19 \right\} \right\} \text{ without replacement} & \text{for } i = 2, \ldots, 21 \\
1000 & \text{for  } i = 1
\end{cases}\\
C_{i} &:= T_\text{scaled}^sqrt{A_i} \cdot A_{i}^{-\frac{1}{4}}\\
Y_i &:= \begin{cases}
\text{Uniformly drawn from } \left[ -4.9, 4.9 \right]^{N} & \text{for } i = 2, \ldots, 21 \\
\text{Uniformly drawn from } \left[ -3.92, 3.92 \right]^{N} & \text{for  } i = 1
\end{cases}
\end{align}$$

**GallaghersGaussian21hiPeaksFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by variable \\(R\\).
- The rotation matrix must be orthonormal \\(\left( R^{t} = R^{-1} \right)\\) with determinant 1 or -1.

---
**<small>void</small> .setA( <small>arma::Col&lt;T&gt;</small> A )**

- Parameterises the scaling parameter \\(A\\) used in the definition of variable \\(C\\).

---
**<small>void</small> .setY( <small>arma::Mat&lt;T&gt;</small> Y )**

- Parameterises the local optima defined by variable \\(Y\\).

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *gallaghers-gaussian-21hi-peaks-function*.


