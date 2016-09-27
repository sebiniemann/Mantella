#!/bin/bash

# Global variables
declare -i AN_ERROR_OCCURED=0
readonly MANTELLA_MAJOR_VERSION=$(expr "$(cat ./include/mantella)" : '.*\#define MANTELLA_VERSION_MAJOR \([0-9]*\)')
## Formatting
readonly RED_TEXT_COLOR=$(tput setaf 1)
readonly GREEN_TEXT_COLOR=$(tput setaf 2)
readonly MAGENTA_TEXT_COLOR=$(tput setaf 5)
readonly RESET_TEXT_COLOR=$(tput sgr0)
readonly REPLACE_LAST_LINE=$(echo ''; tput cuu1; tput el)
## Sub-options
declare -i FIX_FORMATTING=0
declare INSTALL_DIR="/usr/include"
declare -i USE_OPENMP=0
declare -i USE_MPI=0

print_help() {
  echo 'Performs several code checks.'
  echo ' '
  echo "./$(basename "$0") options..."
  echo ' '
  echo 'options:'
  echo '-h, --help                  Shows this help.'
  echo '-f, --format [fix]          Checks the code formatting rules.'
  echo '                            Add "fix" to automatically fix formatting errors.'
  echo '-i, --install [dir]         Installs the library.'
  echo "                            Set \"dir\" to specify the installation directory (default is \"${INSTALL_DIR}\")."
  echo '-t, --test [openmp] [mpi]   Builds and runs unit tests.'
  echo '                            Add "openmp" to use OpenMP within tests.'
  echo '                            Add "mpi" to use MPI within tests.'
  echo '-d, --doc                   Builds the documentation.'
}

finish_up() {
  if (( AN_ERROR_OCCURED == 0 )); then
    echo "${GREEN_TEXT_COLOR}Everything is fine.${RESET_TEXT_COLOR}"
  else
    echo "${RED_TEXT_COLOR}An error occurred (see above).${RESET_TEXT_COLOR}"
  fi
  
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR}"
}

do_format() {
  echo "${MAGENTA_TEXT_COLOR}Checking format rules${RESET_TEXT_COLOR}"
  
  local FILES
  FILES=$(find ./include ./test -not \( -path ./test/build -prune \) -type f)
  local NUMBER_OF_FILES
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l)
  local COUNTER=1
  
  while read -r FILE; do
    printf "[%3s%%] %s" "$(( (COUNTER * 100) / NUMBER_OF_FILES ))" "${FILE}"
    
    if [[ $(clang-format -output-replacements-xml "${FILE}" | grep -c "<replacement ") -ne 0 ]]; then
      if [[ ${FIX_FORMATTING} == 1 ]]; then
        clang-format "${FILE}" > '/tmp/.formatted'; cat '/tmp/.formatted' > "${FILE}"
        echo " ${RED_TEXT_COLOR}was automatically formatted.${RESET_TEXT_COLOR}"
      else
        echo " ${RED_TEXT_COLOR}is not properly formatted.${RESET_TEXT_COLOR} Please run '${GREEN_TEXT_COLOR}clang-format -i ${FILE}${RESET_TEXT_COLOR}'."
      fi
      AN_ERROR_OCCURED=1
    else
      printf "%s" "${REPLACE_LAST_LINE}"
    fi

    COUNTER=$((++COUNTER))
  done <<< "${FILES}"
  
  finish_up
}

do_install() {
  echo "${MAGENTA_TEXT_COLOR}Installing Mantella to \"${INSTALL_DIR}\"${RESET_TEXT_COLOR}"
  
  if [ ! -d "${INSTALL_DIR}/mantella${MANTELLA_MAJOR_VERSION}_bits/" ]; then
    mkdir "${INSTALL_DIR}/mantella${MANTELLA_MAJOR_VERSION}_bits/"
  fi
  
  local FILES
  FILES=$(find ./include/mantella_bits -type f)
  local DESTINATION=${INSTALL_DIR}/mantella${MANTELLA_MAJOR_VERSION}_bits
  while read -r FILE; do
    echo "${FILE} -> ${DESTINATION}/$(basename ${FILE})"
    mkdir -p "$(dirname ${FILE})"
    if ! cp "$FILE" "${DESTINATION}/"; then AN_ERROR_OCCURED=$?; fi
  done <<< "${FILES}"
  
  echo "./include/mantella -> ${INSTALL_DIR}/mantella${MANTELLA_MAJOR_VERSION}"
  if ! cp "./include/mantella" "${INSTALL_DIR}/mantella${MANTELLA_MAJOR_VERSION}"; then AN_ERROR_OCCURED=$?; fi
  
  finish_up
}

do_test() {
  echo "${MAGENTA_TEXT_COLOR}Building and running tests${RESET_TEXT_COLOR}"
  
  cd ./test || exit 1
  if [ ! -d "./build" ]; then
    mkdir build
  fi
  cd ./build || exit 1
  
  if ! cmake ..; then AN_ERROR_OCCURED=$?; fi
  
  if (( AN_ERROR_OCCURED == 0)); then
    if ! make clean tests; then AN_ERROR_OCCURED=$?; fi
  fi
  
  if (( AN_ERROR_OCCURED == 0)); then
    if ! ./tests; then AN_ERROR_OCCURED=$?; fi
  fi
  
  finish_up
}

do_doc() {
  echo "${MAGENTA_TEXT_COLOR}Building documentation${RESET_TEXT_COLOR}"
  
  cd ./doc || exit 1
  
  if ! python ./.prepare_doc.py; then AN_ERROR_OCCURED=$?; fi
  
  if (( AN_ERROR_OCCURED == 0)); then
    if ! sphinx-build -a . ./_html; then AN_ERROR_OCCURED=$?; fi
  fi
  
  finish_up
}

if [ ! -f './include/mantella' ]; then
  echo "${RED_TEXT_COLOR}Could not find Mantella. Make sure to start this script within Mantella's root path.${RESET_TEXT_COLOR}"
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
      -f|--format)
        if [ "$2" == 'fix' ]; then
          FIX_FORMATTING=1; shift
        fi;
        do_format
      ;;
      -i|--install)
        if [[ ! "$2" =~ ^- ]]; then
          INSTALL_DIR=$2; shift
        fi;
        do_install
      ;;
      -t|--test)
        do_test
        if [ "$2" == 'openmp' ]; then
          USE_OPENMP=1; shift
        fi;
        if [ "$2" == 'mpi' ]; then
          USE_MPI=1; shift
        fi;
      ;;
      -d|--doc)
        do_doc
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
