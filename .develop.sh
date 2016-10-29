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
  echo -e '-d, --doc                   Builds the documentation.'
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
  echo -e "${NOTICE_COLOR}Installing Mantella to \"${INSTALL_DIR}\"${RESET_COLOR}"
  
  echo -e "copy ./include/mantella* -> ${INSTALL_DIR}"
  if ! cp -R ./include/mantella* "${INSTALL_DIR}"; then AN_ERROR_OCCURED=1; fi
  
  finish_up
}

do_test() {
  echo -e "${NOTICE_COLOR}Compiling and running tests${RESET_COLOR}"
  
  cd ./test || exit 1
  if [ ! -d "./build" ]; then mkdir build; fi
  cd ./build || exit 1
  
  if ! cmake ..; then AN_ERROR_OCCURED=1; fi
  
  if (( AN_ERROR_OCCURED == 0)); then
    if ! make clean tests; then AN_ERROR_OCCURED=1; fi
  fi
  
  if (( AN_ERROR_OCCURED == 0)); then
    if ! ./tests; then AN_ERROR_OCCURED=1; fi
  fi
  
  cd ../../ || exit 1
  
  finish_up
}

do_doc() {
  echo -e "${NOTICE_COLOR}Building documentation${RESET_COLOR}"
  
  cd ./doc || exit 1
  
  if ! python3 ./.pre_processing.py; then AN_ERROR_OCCURED=1; fi
  
  if (( AN_ERROR_OCCURED == 0)); then
    if ! sphinx-build -a . ./_html; then AN_ERROR_OCCURED=1; fi
  fi
  
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
