
.. cpp:function:: template <N> constexpr two_subsets_partitions()

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **N** (``std::size_t``) - A number of elements.

  **Returns**
   
    ``std::array<std::size_t, 2*mant::stirling_partition_number(N, 2)>`` - All `set-partitions <https://en.wikipedia.org/wiki/Partition_of_a_set>`_ of ``N`` numbers into ``2`` sets or an empty array if the number of partitions overflows.