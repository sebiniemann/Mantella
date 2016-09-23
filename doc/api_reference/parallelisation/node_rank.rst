
.. cpp:function:: node_rank()

  .. versionadded:: 1.x

  **Returns**
   
    ``int`` - The current node rang or ``0`` if MPI is not used.
    
  .. note::
  
    If MPI is supported, the behaviour will be the same as calling ``MPI_Comm_rank(MPI_COMM_WORLD, &node_rank)``.