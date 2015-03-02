$$F(X) :=  T_\text{scaled}^{1000000} \cdot T_{oscillated} \left( X - X^\text{opt} \right)^{2}$$

**EllipsoidalFunction( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *ellipsoidal-function*.


