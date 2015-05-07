**<small>arma::Mat&lt;double&gt;</small> getRandomRotationMatrix( <small>unsigned int</small> N )**

- Generates a randomly and uniformly drawn *N*-dimensional rotation matrix.
- **Note:** The rotation of each dimension will be uniformly distributed.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
**<small>arma::Col&lt;unsigned int&gt;</small> getRandomPermutation( <small>unsigned int</small> N, <small>unsigned int</small> K )**

- Generates a randomly and uniformly drawn permutation of length *K* from *N* elements.
- All elements in the permutation will be unique and within [0, *N* - 1].
- **Requirement:** The length *P* must be greater than or equal to 1.
- **Requirement:** The length *P* must be less than or equal to the number of elements *N*.

---
**<small>arma::Col&lt;unsigned int&gt;</small> getRandomPermutation( <small>unsigned int</small> N )**
{% include noexcept %}

- Generates a randomly and uniformly drawn permutation of length *N* from *N* elements.
- All elements in the permutation will be unique and within [0, *N* - 1].
- **Note:** This is equal to `.getRandomPermutation(N, N)`.
- **Requirement:** The number of elements *N* must be greater than or equal to 1.