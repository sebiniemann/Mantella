$$F(X) :=  T_\text{scaled}^{1000000} \cdot T_{oscillated} \left( R \cdot \left( X - X^\text{opt} \right) \right)^{2}$$

**EllipsoidalFunctionRotated( <small>unsigned int</small> N )**

- Creates an *N*-dimensional optimisation problem instance of this class.
- The problem must have at least 1 dimension.

---
**<small>void</small> .setXOpt( <small>arma::Col&lt;T&gt;</small> X )**

- Parameterises the transition by variable \\(X^\text{opt}\\).

---
**<small>void</small> .setR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by variable \\(R\\).
- The rotation matrix must be orthonormal (\\(R^{t} = R^{-1}\\)) with determinant 1 or -1.

---
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, i.e. *ellipsoidal-function-rotated*.


