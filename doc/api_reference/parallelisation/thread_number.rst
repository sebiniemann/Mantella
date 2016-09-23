
.. cpp:function:: thread_number()

  .. versionadded:: 1.x

  **Returns**
   
    ``int`` - The current thread number or ``0`` if OpenMP is not used.
    
  .. note::
  
    If OpenMP is supported, the behaviour will be the same as calling ``omp_get_thread_num()``.