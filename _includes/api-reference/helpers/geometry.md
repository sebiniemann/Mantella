
**<small>arma::Mat&lt;double&gt;::fixed&lt;2, 2&gt;</small> get2DRotation( <small>double</small> R )** {% include noexcept %}

- Generates a 2-dimensional rotation matrix with rotation angle *R*.
- The rotation angle *R* is assumed to be measured in radians.
- **Note:** Post-multiplication with a column vector will rotate the vector in the opposite direction.

---
**<small>arma::Mat&lt;double&gt;::fixed&lt;3, 3&gt;</small> get3DRotation( <small>double</small> R, <small>double</small> P, <small>double</small> Y)** {% include noexcept %}

- Generates a 3-dimensional rotation matrix with roll angle *R*, pitch angle *P* and yaw angle *Y*.
- All rotation angles (*R*, *P* and *Y*), are assumed to be measured in radians.
- **Note:** Post-multiplication with a column vector will rotate the vector in the opposite directions.

---
**<small>arma::Col&lt;double&gt;::fixed&lt;2&gt;</small> getCircleCircleIntersection( <small>arma::Col&lt;double&gt;::fixed&lt;2&gt;</small> C1, <small>double</small> R1, <small>arma::Col&lt;double&gt;::fixed&lt;2&gt;</small> C2, <small>double</small> R2 )**

- Calculates the (lower) intersection points between two circles in a 2-dimensional space.
- The first circle is specified by its centre *C1* and radius *R1*.
- The second circle is specified by its centre *C2* and radius *R2*.
- **Note:** The behaviour of this helper function is only based on the usage within the library, for which all other results are invalid.
- **Requirement:** The number of elements in *C1* must be equal to the number of elements in *C2*.
- **Requirement:** The circles must intersect in exactly two points.

---
**<small>arma::Col&lt;double&gt;::fixed&lt;3&gt;</small> getCircleSphereIntersection( <small>arma::Col&lt;double&gt;::fixed<3&gt;</small> CC, <small>double</small> CR, <small>arma::Col&lt;double&gt;::fixed&lt;3&gt;</small> CN, <small>arma::Col&lt;double&gt;::fixed&lt;3&gt;</small> SC, <small>double</small> SR )**
      
- Calculates the (lower) intersection points between a circle and a sphere in a 3-dimensional space.
- The circle is specified by its centre *CC*, its radius *CR* and a normal vector *CN*, standing orthonormal to the circle.
- The sphere is specified by its centre *SC* and its radius *SR*.
- **Note:** The behaviour of this helper function is only based on the usage within the library, for which all other results are invalid.
- **Requirement:** The number of elements in *CC* must be equal to the number of elements in *CN* and *SC*.
- **Requirement:** The circle and the sphere must intersect in exactly two points.