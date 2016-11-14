#!/bin/bash

# Global variables
declare -i AN_ERROR_OCCURED=0
readonly MANTELLA_MAJOR_VERSION=$(expr "$(grep "#define MANTELLA_VERSION_MAJOR" "$(find include -name "mantella*" -type f)")" : '.*\#define MANTELLA_VERSION_MAJOR \([0-9]*\)')
## Formatting
readonly ERROR_COLOR='\033[91m'
readonly OK_COLOR='\033[92m'
readonly NOTICE_COLOR='\033[94m'
readonly RESET_COLOR='\033[0m'
readonly REPLACE_LAST_LINE=$(echo ''; tput cuu1; tput el)
## Sub-options
declare INSTALL_DIR="/usr/include"
declare -i SKIP_DOC_PREPROCESSING=0

print_help() {
  echo -e 'Performs several code checks.'
  echo -e ' '
  echo -e "./$(basename "$0") options..."
  echo -e ' '
  echo -e 'options:'
  echo -e '-h, --help                  Shows this help.'
  echo -e '-i, --install [dir]         Installs the library.'
  echo -e "                            Set \"dir\" to specify the installation directory (default is \"${INSTALL_DIR}\")."
  echo -e '-t, --test                  Compiles and runs unit tests.'
  echo -e '-d, --doc [skip]            Builds the documentation.'
  echo -e "                            Set \"skip\" to skip generating Sphinx files from the header files."
  echo -e '-b, --benchmark             Compiles and runs benchmarks.'
}

finish_up() {
  if (( AN_ERROR_OCCURED == 0 )); then
    echo -e "${OK_COLOR}Everything is fine.${RESET_COLOR}"
  else
    echo -e "${ERROR_COLOR}An error occurred (see above).${RESET_COLOR}"
  fi
  
  echo -e "${NOTICE_COLOR}done.${RESET_COLOR}"
}

do_install() {
  echo -e "${NOTICE_COLOR}Installing Mantella to \"${INSTALL_DIR}\".${RESET_COLOR}"
  
  echo -e "copy ./include/mantella* -> ${INSTALL_DIR}"
  if ! cp -R ./include/mantella* "${INSTALL_DIR}"; then AN_ERROR_OCCURED=1; finish_up; return; fi
  
  finish_up
}

do_test() {
  echo -e "${NOTICE_COLOR}Compiling and running tests.${RESET_COLOR}"
  
  cd ./test || exit 1
  if [ ! -d "./build" ]; then mkdir build; fi
  cd ./build || exit 1
  
  if ! cmake -DCMAKE_BUILD_TYPE=Debug ..; then AN_ERROR_OCCURED=1; finish_up; return; fi
  if ! make clean tests; then AN_ERROR_OCCURED=1; finish_up; return; fi
  if ! ./tests; then AN_ERROR_OCCURED=1; finish_up; return; fi
  
  cd ../.. || exit 1
  
  finish_up
}

do_doc() {
  echo -e "${NOTICE_COLOR}Building documentation.${RESET_COLOR}"
  
  cd ./doc || exit 1
  
  if (( SKIP_DOC_PREPROCESSING != 1 )); then
    if ! python3 ./.pre_processing.py; then AN_ERROR_OCCURED=1; finish_up; return; fi
  fi
  if ! sphinx-build -E -a . ./_html; then AN_ERROR_OCCURED=1; finish_up; return; fi

  if [ ! -d "./_html/_animations" ]; then mkdir ./_html/_animations; fi
  if ! cp ./.animations/* ./_html/_animations/; then AN_ERROR_OCCURED=1; finish_up; return; fi
  
  cd .. || exit 1
  
  finish_up
}

do_benchmark() {
  echo -e "${NOTICE_COLOR}Compiling and running benchmarks.${RESET_COLOR}"
  
  if [ -z "$(pidof dockerd)" ]; then
    if ! service docker start; then AN_ERROR_OCCURED=1; finish_up; return; fi
  fi
  
  cd ./benchmark || exit 1
  
  local -a LIBRARIES=('mantella')
  for LIBRARY in "${LIBRARIES[@]}"; do
    cd "./${LIBRARY}" || exit 1
    
    if [ "${LIBRARY}" != 'mantella' ]; then
      if [ -z "$(docker images -q "benchmark/${LIBRARY}")" ]; then
        if ! docker build -t "benchmark/${LIBRARY}":latest .; then AN_ERROR_OCCURED=1; finish_up; return; fi
      fi
      if [ -z "$(docker ps -q -f name="benchmark_${LIBRARY}")" ]; then
        if ! docker run -v "$(pwd):/${LIBRARY}" -w "/${LIBRARY}" --name "benchmark_${LIBRARY}" -t -d "benchmark/${LIBRARY}"; then AN_ERROR_OCCURED=1; finish_up; return; fi
      fi
      
      docker exec "benchmark_${LIBRARY}" /bin/bash -c " \
        if [ ! -d './build' ]; then mkdir build; fi && \
        cd ./build && \
        cmake .. && \
        make clean benchmark && \
        ./benchmark && \
        cp benchmark.mat ..
      "
    else
      if [ ! -d './build' ]; then
        mkdir build;
      fi
      cd ./build || exit 1
      cmake ..
      make clean benchmark 
      ./benchmark
      cp benchmark.mat ..
      cd .. || exit 1
    fi
      
    cd .. || exit 1
  done
  
  octave benchmark.m
  
  cd .. || exit 1
  
  finish_up
}

if [ ! -f "./include/mantella${MANTELLA_MAJOR_VERSION}" ]; then
  echo -e "${ERROR_COLOR}Could not find Mantella. Make sure to start this script within Mantella's root path.${RESET_COLOR}"
  exit 1
fi

if [[ $# -lt 1 ]]; then
  print_help
  exit 1
else 
  while [[ $# -gt 0 ]]; do
    OPTION="$1"
    case $OPTION in
      -h|--help)
        print_help
        exit 0
      ;;
      -i|--install)
        if [[ "$2" && ! "$2" =~ ^- ]]; then
          INSTALL_DIR=$2; shift
        fi;
        do_install
      ;;
      -t|--test)
        do_test
      ;;
      -d|--doc)
        if [[ "$2" && ! "$2" =~ ^- ]]; then
          SKIP_DOC_PREPROCESSING=1; shift
        fi;
        do_doc
      ;;
      -b|--benchmark)
        do_benchmark
      ;;
      *)
        error "Unexpected option ${OPTION}"
        print_help
        exit 1
      ;;
    esac
    shift 
  done
fi

if (( AN_ERROR_OCCURED != 0 )); then
  exit 1
fi
