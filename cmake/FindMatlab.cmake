find_program(MATLAB_MEX mex.bat mex.sh)
if (MATLAB_MEX)
  # The `mex` command always requires a proper (ending with `.c`) C-file to output the compile flags, even if its only a dry run.
  file(WRITE ${CMAKE_CACHEFILE_DIR}/mex_dummy.c "Dummy file")
  execute_process(COMMAND ${MATLAB_MEX} -v -n ${CMAKE_CACHEFILE_DIR}/mex_dummy.c OUTPUT_VARIABLE MATLAB_MEX_FLAGS)
  file(REMOVE ${CMAKE_CACHEFILE_DIR}/mex_dummy.c)

  string(REGEX REPLACE ".*\tDEFINES : ([^\n]+).*" "\\1" MATLAB_DEFINES ${MATLAB_MEX_FLAGS})
  if (MATLAB_DEFINES)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_DEFINES ${MATLAB_DEFINES})
    separate_arguments(MATLAB_DEFINES WINDOWS_COMMAND ${MATLAB_DEFINES})
  endif()
  string(REGEX REPLACE ".*\tINCLUDE : ([^\n]+).*" "\\1" MATLAB_INCLUDE ${MATLAB_MEX_FLAGS})
  if (MATLAB_INCLUDE)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_INCLUDE ${MATLAB_INCLUDE})
    string(REGEX REPLACE "-I" "" MATLAB_INCLUDE ${MATLAB_INCLUDE})
    separate_arguments(MATLAB_INCLUDE WINDOWS_COMMAND ${MATLAB_INCLUDE})
  endif()
  string(REGEX REPLACE ".*\tCFLAGS : ([^\n]+).*" "\\1" MATLAB_CFLAGS ${MATLAB_MEX_FLAGS})
  if (MATLAB_CFLAGS)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_CFLAGS ${MATLAB_CFLAGS})
    separate_arguments(MATLAB_CFLAGS WINDOWS_COMMAND ${MATLAB_CFLAGS})
  endif()
  string(REGEX REPLACE ".*\tCOPTIMFLAGS : ([^\n]+).*" "\\1" MATLAB_COPTIMFLAGS ${MATLAB_MEX_FLAGS})
  if (MATLAB_COPTIMFLAGS)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_COPTIMFLAGS ${MATLAB_COPTIMFLAGS})
    separate_arguments(MATLAB_COPTIMFLAGS WINDOWS_COMMAND ${MATLAB_COPTIMFLAGS})
  endif()
  string(REGEX REPLACE ".*\tLDFLAGS : ([^\n]+).*" "\\1" MATLAB_LDFLAGS ${MATLAB_MEX_FLAGS})
  if (MATLAB_LDFLAGS)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_LDFLAGS ${MATLAB_LDFLAGS})
  endif()
  string(REGEX REPLACE ".*\tLDTYPE : ([^\n]+).*" "\\1" MATLAB_LDTYPE ${MATLAB_MEX_FLAGS})
  if (MATLAB_LDTYPE)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_LDTYPE ${MATLAB_LDTYPE})
  endif()
  string(REGEX REPLACE ".*\tLDOPTIMFLAGS : ([^\n]+).*" "\\1" MATLAB_LDOPTIMFLAGS ${MATLAB_MEX_FLAGS})
  if (MATLAB_LDOPTIMFLAGS)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_LDOPTIMFLAGS ${MATLAB_LDOPTIMFLAGS})
  endif()
  string(REGEX REPLACE ".*\tLINKEXPORT : ([^\n]+).*" "\\1" MATLAB_LINKEXPORT ${MATLAB_MEX_FLAGS})
  if (MATLAB_LINKEXPORT)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_LINKEXPORT ${MATLAB_LINKEXPORT})
  endif()
  string(REGEX REPLACE ".*\tLINKLIBS : ([^\n]+).*" "\\1" MATLAB_LINKLIBS ${MATLAB_MEX_FLAGS})
  if (MATLAB_LINKLIBS)
    string(REGEX REPLACE "(\\\\)" "/" MATLAB_LINKLIBS ${MATLAB_LINKLIBS})
  endif()
  string(REGEX REPLACE ".*\tLDEXT : ([^\n]+).*" "\\1" MATLAB_LDEXT ${MATLAB_MEX_FLAGS})

  set(MATLAB_COMPILE_OPTIONS ${MATLAB_DEFINES} ${MATLAB_CFLAGS} ${MATLAB_COPTIMFLAGS})
  set(MATLAB_INCLUDE_DIR ${MATLAB_INCLUDE})
  set(MATLAB_LINK_OPTIONS ${MATLAB_LDFLAGS} ${MATLAB_LDTYPE} ${MATLAB_LDOPTIMFLAGS} ${MATLAB_LINKEXPORT})
  set(MATLAB_LIBRARIES ${MATLAB_LINKLIBS})
  set(MATLAB_SUFFIX ${MATLAB_LDEXT})
  
  unset(MATLAB_DEFINES)
  unset(MATLAB_INCLUDE)
  unset(MATLAB_CFLAGS)
  unset(MATLAB_COPTIMFLAGS)
  unset(MATLAB_LDFLAGS)
  unset(MATLAB_LDTYPE)
  unset(MATLAB_LDOPTIMFLAGS)
  unset(MATLAB_LINKEXPORT)
  unset(MATLAB_LINKLIBS)
  unset(MATLAB_LDEXT)
endif()

if (WIN32)
  set(MATLAB_MEX_FILES_DIR $ENV{HOMEDRIVE}$ENV{HOMEPATH}/Documents/MATLAB)
else()
  set(MATLAB_MEX_FILES_DIR $ENV{HOME}/Documents/MATLAB)
endif()

find_package_handle_standard_args(Matlab
  FOUND_VAR MATLAB_FOUND
  REQUIRED_VARS MATLAB_MEX MATLAB_MEX_FILES_DIR MATLAB_COMPILE_OPTIONS MATLAB_INCLUDE_DIR MATLAB_LINK_OPTIONS MATLAB_LIBRARIES MATLAB_SUFFIX)

mark_as_advanced (
  MATLAB_MEX
  MATLAB_MEX_FILES_DIR
  MATLAB_COMPILE_OPTIONS
  MATLAB_INCLUDE_DIR
  MATLAB_LINK_OPTIONS
  MATLAB_LIBRARIES
  MATLAB_SUFFIX)