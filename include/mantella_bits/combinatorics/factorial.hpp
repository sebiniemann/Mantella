#pragma once

/**
.. cpp:function:: template <N> constexpr factorial()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N** (``std::size_t``) - A number.

  **Returns**
   
    ``std::size_t`` - The `factorial <https://en.wikipedia.org/wiki/Factorial>`_ :math:`\text{N}!` or ``0`` if it overflows.
    
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      std::cout << "0! = " << factorial<0>() << "\n";
      std::cout << "1! = " << factorial<1>() << "\n";
      std::cout << "2! = " << factorial<2>() << "\n";
      std::cout << "3! = " << factorial<3>() << "\n";
      std::cout << "100! = " << factorial<3>() << "\n";
      std::cout << std::endl;
      
      return 0;
    }
*/
template <std::size_t N> constexpr factorial();