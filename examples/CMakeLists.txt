if(MPI_C_FOUND)
  add_subdirectory("./mpi")
  add_subdirectory("./parallelOptimisation")
endif()

add_subdirectory("./onlineOptimisation")
