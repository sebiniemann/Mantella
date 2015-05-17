Provides a single class to handle the initialisation/seeding of the random number generators of the C++ standard library and the Armadillo C++ linear algebra library.

For the C++ standard library, the 64-bit Mersenne Twister 19937 (`std::mt19937_64`) is used. Armadillo C++ will also use `std::mt19937_64`, if provided with a C++11-compatible compiler.

It is recommended to call all C++ standard library random functions, like `std::uniform_real_distribution<double>(0.0, 1.0)`, with `mant::Rng::getGenerator())` as random number generator. Any function from Armadillo C++, like `arma::randu(...)`, can be used as usual.

---
**<small>std::mt19937\_64</small> Rng::getGenerator()** {% include noexcept %}

- Returns the random number generator, i.e. a reference to `std::mt19937_64`.

---
**<small>void</small> Rng::setSeed( <small>unsigned int</small> S )** {% include noexcept %}

- Sets the random generator provided by `Rng::getGenerator()` as well as Armadillo C++'s random generator to the seed *S*.

---
**<small>void</small> Rng::setRandomSeed()** {% include noexcept %}

- Initialises the random generator provided by `Rng::getGenerator()` as well as Armadillo C++'s random generator to the same randomly and uniformly drawn seed *S*.
- If used together with a MPI-library, ensure to call `MPI_Init(...)` prior to this function, as it uses local, MPI-based information to provided each instance with its own random seed, avoiding the degeneration of paralleled randomness.

---
**<small>unsigned int</small> Rng::getSeed()** {% include noexcept %}

- Returns the seed used for `Rng::getGenerator()` as well as Armadillo C++'s random generator.
- **Note:** If neither `Rng::setRandomSeed()` nor `Rng::setSeed(...)` was called prior to this, the result will be anything.