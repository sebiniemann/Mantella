**<small>arma::Mat&lt;double&gt;</small> getHaltonSequence( <small>arma::Col&lt;unsigned int&gt;</small> B, <small>arma::Col&lt;unsigned int&gt;</small> S, <small>unsigned int</small> N )**

- Generates a multi-dimensional quasi-random sequence with length *N*, based on the Halton-algorithm. The number of columns is equal to the number of bases in *B* as well as seeds in *S*.
- For the *i*-te sequence, the first \\(S_i\\) elements are skipped.
- The *(k, i)*-te element of the result matrix is the *k*-te element of the *i*-te sequence.
- **Note:** Each base within *B* should be a (unique) prime number, however this is left unchecked.
- **Note:** If *B* holds only a single element, the result is equal to `.getVanDerCorputSequence(B(0), S(0), N)`.
- **Requirement:** The number of elements in *B* must be equal to the number of elements in *S*.
- **Requirement:** Each base within *B* must be greater than or equal to 2.
- **Requirement:** The number of elements *N* must be greater than or equal to 1.

---
**<small>arma::Col&lt;double&gt;</small> getVanDerCorputSequence( <small>unsigned int</small> B, <small>unsigned int</small> S, <small>unsigned int</small> N )**

- Generates a one-dimensional quasi-random sequence with length *N*, based on the Van-Der-Corput-algorithm.
- **Note:** The base should be a prime number, however this is left unchecked.
- **Requirement:** The base *B* must be greater than or equal to 2.
- **Requirement:** The number of elements *N* must be greater than or equal to 1.