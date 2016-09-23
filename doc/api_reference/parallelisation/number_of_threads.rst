
.. cpp:function:: number_of_threads()

  .. versionadded:: 1.x

  **Returns**
   
    ``int`` - The (maximal) number of threads or ``1`` if OpenMP is not used.
    
  .. note::
  
    If OpenMP is supported, the behaviour will be the same as calling ``omp_get_max_threads()``.