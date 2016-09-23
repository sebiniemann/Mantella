
.. cpp:function:: template <N1, N2> constexpr combinations()

  .. versionadded:: 1.x

  **Template parameters**

    * **N1** (``std::size_t``) - A number of elements.
    * **N2** (``std::size_t``) - A cycle size.

  **Returns**

    ``std::array<std::size_t, N2*mant::n_choose_k(N1, N2)>`` - The `combinations <https://en.wikipedia.org/wiki/Combination>`_ or an empty array if the number of combinations overflows.