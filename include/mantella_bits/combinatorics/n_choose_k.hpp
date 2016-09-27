#pragma once

/**
.. cpp:function:: template <N1, N2> constexpr n_choose_k()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N1** (``std::size_t``) - The number of elements (:math:`n`).
    * **N2** (``std::size_t``) - The number of elements to choose (:math:`n`).

  **Returns**
   
    ``std::size_t`` - The `binomial coefficient <https://en.wikipedia.org/wiki/Binomial_coefficient>`_ :math:`\binom{\text{N1}}{\text{N2}}` or ``0`` if it overflows.
    
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      std::cout << "0 over 0 = " << n_choose_k<0, 0>() << "\n";
      std::cout << "0 over 1 = " << n_choose_k<0, 1>() << "\n";
      std::cout << "1 over 0 = " << n_choose_k<1, 0>() << "\n";
      std::cout << "3 over 3 = " << n_choose_k<3, 3>() << "\n";
      std::cout << "6 over 3 = " << n_choose_k<6, 3>() << "\n";
      std::cout << "100 over 10 = " << n_choose_k<100, 10>() << "\n";
      std::cout << std::endl;
      
      return 0;
    }
  
  .. note::
  
    The result will also be ``0`` (per definition of the binomial coefficient) if ``N2 > N1`` holds true.
*/
template <typename T1, typename T2>
constexpr std::size_t n_choose_k(
    T1 n,
    const T2 k);

//
// Implementation
//

template <typename T1, typename T2>
constexpr std::size_t n_choose_k(
    T1 n,
    const T2 k) {
  if (k > n) {
    return 0;
  } else if (k == 0 || n == k) {
    return 1;
  } else if (k == 1) {
    return n;
  }

  /* Instead of calculating
   *
   *      n!
   * -------------
   * ((n-k)! * k!)
   * 
   * directly, we can also write it as
   * 
   * /     / / / n * (n-1) \         \         \     \
   * |     | | | --------- | * (n-2) |         |     |
   * |     | | \     2     /         | * (n-3) |     |
   * | ... | | --------------------- |         | ... |
   * |     | \           3           /         |     |  * (k+1)
   * |     | --------------------------------- |     |
   * |     \                 4                 /     |
   * \                         ...                   /
   * ----------------------------------------------------------
   *                              k
   *
   * This avoids temporarily storing large integers and allows us to calculate greater binomial coefficients, before hitting an overflow.
   * In fact, the largest number we are temporarily storing by using this method is exactly `k` times larger than the resulting binomial coefficient.
   * **Note:** The dividends are always divisible with a zeroed remainder, because by multiplying `n` consecutive numbers, one is always a multiply of `n`.
   */

  std::size_t binomial_coefficient = n;
  for (T2 l = 2; l <= k; ++l) {
    // **Note:** Since the results of the left term will be floored, we might be stricter than we have to be, but will not miss an overflow.
    if (binomial_coefficient > std::numeric_limits<std::size_t>::max() / (n - 1)) {
      return 0;
    }

    binomial_coefficient *= --n;
    binomial_coefficient /= l;
  }

  return binomial_coefficient;
}

//
// Tests
//

#if defined(MANTELLA_BUIL_TESTS)
TEST_CASE("n_choose_k", "[combinatorics][n_choose_k]") {
  CHECK(n_choose_k(0, 0) == 0);
  CHECK(n_choose_k(1, 0) == 0);
  CHECK(n_choose_k(3, 3) == 1);
  CHECK(n_choose_k(6, 3) == 20);
  CHECK(n_choose_k(100, 100) == 0);
}
#endif