#pragma once

/**
.. cpp:function:: template <N1, N2> constexpr multicombinations()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N1** (``std::size_t``) - A number of elements.
    * **N2** (``std::size_t``) - A cycle size.

  **Returns**
   
    ``std::array<std::size_t, N2*mant::n_choose_k(N1+N2-1, N2)>`` - The `multicombinations <https://en.wikipedia.org/wiki/Combination#Number_of_combinations_with_repetition>`_ or an empty array if the number of multicombinations overflows.
*/
template <std::size_t N1, std::size_t N2> constexpr std::array<std::size_t, N2*n_choose_k(N1+N2-1, N2)> multicombinations();