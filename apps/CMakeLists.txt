# Library standard
add_subdirectory("./toolbox")

# Students

# Experiemental
if(MPI_C_FOUND)
  add_subdirectory("./mpi")
endif()
add_subdirectory("./onlineOptimisation")
