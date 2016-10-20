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
declare INSTALL_DIR="/usr/include"

print_help() {
  echo 'Performs several code checks.'
  echo ' '
  echo "./$(basename "$0") options..."
  echo ' '
  echo 'options:'
  echo '-h, --help                  Shows this help.'
  echo '-i, --install [dir]         Installs the library.'
  echo "                            Set \"dir\" to specify the installation directory (default is \"${INSTALL_DIR}\")."
  echo '-t, --test                  Builds and runs unit tests.'
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

do_install() {
  echo "${MAGENTA_TEXT_COLOR}Installing Mantella to \"${INSTALL_DIR}\"${RESET_TEXT_COLOR}"
  
  echo "copy ./include/mantella* -> ${INSTALL_DIR}"
  if ! cp -R ./include/mantella* "${INSTALL_DIR}"; then AN_ERROR_OCCURED=$?; fi
  
  finish_up
}

do_test() {
  echo "${MAGENTA_TEXT_COLOR}Building and running tests${RESET_TEXT_COLOR}"
  
  cd ./test || exit 1
  if [ ! -d "./build" ]; then mkdir build; fi
  cd ./build || exit 1
  
  # TODO Use OpenMP if requested
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
