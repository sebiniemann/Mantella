
.. cpp:function:: number_of_nodes()

  .. versionadded:: 1.x

  **Returns**
   
    ``int`` - The number of MPI nodes or ``1`` if MPI is not used.
    
  .. note::
  
    If MPI is supported, the behaviour will be the same as calling ``MPI_Comm_size(MPI_COMM_WORLD, &number_of_nodes)``.