#pragma once

/**
.. cpp:function:: template <N1, N2> constexpr stirling_partition_number()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N1** (``std::size_t``) - The number of elements.
    * **N2** (``std::size_t``) - The number of subsets.

  **Returns**
   
    ``std::size_t`` - The `Stirling partition number <https://en.wikipedia.org/wiki/Stirling_numbers_of_the_second_kind>`_ :math:`S(\text{N1}, \text{N2})` or ``0`` if it overflows.
    
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      std::cout << "S(0, 0) = " << stirling_partition_number<0, 0>() << "\n";
      std::cout << "S(0, 1) = " << stirling_partition_number<0, 1>() << "\n";
      std::cout << "S(1, 0) = " << stirling_partition_number<1, 0>() << "\n";
      std::cout << "S(3, 3) = " << stirling_partition_number<3, 3>() << "\n";
      std::cout << "S(6, 3) = " << stirling_partition_number<6, 3>() << "\n";
      std::cout << "S(100, 10) = " << stirling_partition_number<100, 10>() << "\n";
      std::cout << std::endl;
      
      return 0;
    }
    
  .. note::
  
    The result will also be ``0`` (per definition of the Stirling partition number) if either ``N1`` or ``N2`` is ``0``.
*/
template <typename T1, typename T2>
constexpr std::size_t stirling_partition_number(
    const T1 number_of_elements,
    const T2 number_of_parts);

//
// Implementation
//

template <typename T1, typename T2>
constexpr std::size_t stirling_partition_number(
    const T1 number_of_elements,
    const T2 number_of_parts) {
  if (number_of_elements == 0 || number_of_parts == 0 || number_of_elements < number_of_parts) {
    return 0;
  } else if (number_of_parts == 1 || number_of_elements == number_of_parts) {
    return 1;
  }

  /* Instead of calculating the second Stirling number directly based on its explicit formula
   *
   *  1     k               / k \
   * --- * sum (-1)^(k-l) * |   | * l^n
   *  k!   l=1              \ l /
   *
   * we wrote it as
   *
   *  k                   l^(n-1)
   * sum (-1)^(k-l) * ---------------
   * l=1              (l-1)! * (k-l)!
   *
   * This avoids temporarily storing large integers and allows us to calculate greater Stirling numbers, before hitting an overflow.
   * However, the fractions are not representable as an integer in most cases, wherefore we perform the whole computation based with floating point types.
   */

  long double stirling_partition_number = 0;
  for (T2 l = 1; l <= number_of_parts; ++l) {
    stirling_partition_number += (number_of_parts % 2 == l % 2 ? 1.0 : -1.0) * std::pow(l, static_cast<long double>(number_of_elements - 1)) / static_cast<long double>(factorial(l - 1) * factorial(number_of_parts - l));
  
    if (stirling_partition_number > std::numeric_limits<std::size_t>::max()) {
      return 0;
    }
  }
  
  // Fixes rounding errors due to the limited precision.
  return static_cast<std::size_t>(std::round(stirling_partition_number));
}