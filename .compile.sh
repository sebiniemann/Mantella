#!/bin/bash

RED_TEXT_COLOR=$(tput setaf 1)
GREEN_TEXT_COLOR=$(tput setaf 2)
MAGENTA_TEXT_COLOR=$(tput setaf 5)
RESET_TEXT_COLOR=$(tput sgr0)
RESET_LINE=$(tput el)

if [ ! -f "_config.yml" ]; then
    echo "${RED_TEXT_COLOR}Could not find examples. Make sure to start this script from the Jekyll root.${RESET_TEXT_COLOR}";
    exit 1;
fi;

CXX=clang++

CXX_FLAGS=""
CXX_FLAGS="${CXX_FLAGS} -std=c++11"
CXX_FLAGS="${CXX_FLAGS} -march=native"
CXX_FLAGS="${CXX_FLAGS} -O3"

# Enables warnings
CXX_FLAGS="${CXX_FLAGS} -Werror"
CXX_FLAGS="${CXX_FLAGS} -Weverything"
CXX_FLAGS="${CXX_FLAGS} -Wno-c++98-compat"
CXX_FLAGS="${CXX_FLAGS} -Wno-weak-vtables"
CXX_FLAGS="${CXX_FLAGS} -Wno-exit-time-destructors"
CXX_FLAGS="${CXX_FLAGS} -Wno-global-constructors"
CXX_FLAGS="${CXX_FLAGS} -Wno-padded"
CXX_FLAGS="${CXX_FLAGS} -Wno-unused-parameter"
# Ignore warnings for unknown options, as we make use of options introduced by more recent Clang versions.
CXX_FLAGS="${CXX_FLAGS} -Wno-unknown-warning-option"
# Ignore warnings resulting from <mpi.h>
CXX_FLAGS="${CXX_FLAGS} -Wno-c++98-compat-pedantic"
CXX_FLAGS="${CXX_FLAGS} -Wno-old-style-cast"
CXX_FLAGS="${CXX_FLAGS} -Wno-extra-semi"
CXX_FLAGS="${CXX_FLAGS} -Wno-reserved-id-macro"
CXX_FLAGS="${CXX_FLAGS} -Wno-deprecated"
CXX_FLAGS="${CXX_FLAGS} -Wno-cast-align"

echo "${MAGENTA_TEXT_COLOR}Compiling and running all examples${RESET_TEXT_COLOR}";
FILES=$(find _includes/examples/ -type f -name \*.cpp);
NUMBER_OF_FILES=$(echo "${FILES}" | wc -l);
COUNTER=1;

while read FILE; do
  printf "[%3s%%] %s\r" "$(( (COUNTER * 100) / NUMBER_OF_FILES ))" "${FILE}"

  ${CXX} ${CXX_FLAGS} ${FILE} -larmadillo -lmantella
  ./a.out > "${FILE}.output"
  rm a.out
  
  COUNTER=$((++COUNTER));
  printf ${RESET_LINE}
done <<< "${FILES}";
echo "${GREEN_TEXT_COLOR}Done.${RESET_TEXT_COLOR}";
