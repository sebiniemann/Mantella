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
template <std::size_t N1, std::size_t N2> constexpr std::size_t stirling_partition_number();