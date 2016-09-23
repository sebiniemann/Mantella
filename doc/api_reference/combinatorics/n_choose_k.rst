
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