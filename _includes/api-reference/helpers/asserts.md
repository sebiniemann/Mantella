Each `is...`-function is intended to support asserts and exception-handling, by either returning a boolean or throw an exception. 

For internal checks that should never fail, i.e. only occur during debug build, we recommend to use `assert(is...)`, and `is...` for all public interfaces, where it should throw an exception.

---
**<small>bool</small> isRotationMatrix( <small>arma::Mat&lt;double&gt;</small> X )** {%include noexcept %}

- Returns true if *X* is a rotation matrix, and false otherwise.

---
**<small>void</small> isRotationMatrix( <small>std::string</small> S, <small>arma::Mat&lt;double&gt;</small> X )**

- Throws an exception if *X* is **not** a rotation matrix, and does *nothing* otherwise.
- *S* is used to name the tested matrix, i.e. *S* must be a rotation matrix, i.e. it must be square, orthonormal and its determinant be either 1 or -1.
- **Note:** The first letter of *S* will be printed as given and not automatically converted into upper-case.
- **Note:** This interface accepts all possible inputs, but is not marked as {% include noexcept %}, as throwing exceptions is a valid result for this function.

---
**<small>bool</small> isEqual( <small>std::size\_t</small> A, <small>std::size\_t</small> B )** {%include noexcept %}

- Returns true if *A* is equal to *B*, and false otherwise.

---
**<small>void</small> isEqual( <small>std::string</small> S, <small>std::size\_t</small> A, <small>std::string</small> T, <small>std::size\_t</small> B )**

- Throws an exception if *A* is **not** equal to *B*, and does *nothing* otherwise.
- *S* and *T* are used to name the compared values, i.e. *S* must be equal to *T*.
- **Note:** The first letter of *S* will be printed as given and not automatically converted into upper-case.
- **Note:** This interface accepts all possible inputs, but is not marked as {% include noexcept %}, as throwing exceptions is a valid result for this function.