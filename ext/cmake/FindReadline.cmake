find_path(READLINE_INCLUDE_DIRS readline/readline.h)
find_library(READLINE_LIBRARIES NAMES readline)

if (READLINE_INCLUDE_DIRS AND READLINE_LIBRARIES)
  set (READLINE_FOUND TRUE)
endif ()

if (READLINE_INCLUDE_DIRS AND EXISTS "${READLINE_INCLUDE_DIRS}/readline/readline.h")
  file(STRINGS "${READLINE_INCLUDE_DIRS}/readline/readline.h"
               READLINE_MAJOR_VERSION
       REGEX "^#define RL_VERSION_MAJOR.*[0-9]+$")
  file(STRINGS "${READLINE_INCLUDE_DIRS}/readline/readline.h"
               READLINE_MINOR_VERSION
       REGEX "^#define RL_VERSION_MINOR.*[0-9]+$")

  string(REGEX REPLACE "^.*RL_VERSION_MAJOR.*([0-9]+).*$" 
                       "\\1" 
                       READLINE_MAJOR_VERSION
                       "${READLINE_MAJOR_VERSION}")
  string(REGEX REPLACE "^.*RL_VERSION_MINOR.*([0-9]+).*$" 
                       "\\1" 
                       READLINE_MINOR_VERSION
                       "${READLINE_MINOR_VERSION}")

  set(READLINE_VERSION "${READLINE_MAJOR_VERSION}.${READLINE_MINOR_VERSION}")
endif ()

if (READLINE_FOUND)
  if (READLINE_FIND_VERSION VERSION_GREATER READLINE_VERSION)
    message(FATAL_ERROR "Found version ${READLINE_VERSION} of GNU Readline at ${READLINE_LIBRARIES}, but version ${READLINE_FIND_VERSION} required")
  endif ()
  if (NOT READLINE_FIND_QUIETLY)
    message(STATUS "Found GNU Readline: ${READLINE_LIBRARIES} (found version \"${READLINE_VERSION}\")")
  endif ()
else ()
  if (READLINE_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find GNU Readline")
  endif ()
endif ()
