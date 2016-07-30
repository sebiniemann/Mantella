#!/bin/bash

# Formatting
readonly RED_TEXT_COLOR=$(tput setaf 1)
readonly GREEN_TEXT_COLOR=$(tput setaf 2)
readonly MAGENTA_TEXT_COLOR=$(tput setaf 5)
readonly RESET_TEXT_COLOR=$(tput sgr0)
readonly REPLACE_LAST_LINE=$(echo ''; tput cuu1; tput el)

declare -i ANY_ERROR_OCCURED=0

# Further options
declare -i FIX_FORMATTING=0

print_help() {
  echo 'Performs several code checks.'
  echo ' '
  echo './.code.sh options...'
  echo ' '
  echo 'options:'
  echo '-h, --help                                  Shows this help.'
  echo '-f, --format [fix]                          Checks the code formatting rules.'
  echo '                                            Add "fix" to automatically fix formatting errors.'
  echo '-i, --include                               Checks the include rules.'
  echo '-s, --static                                Performs static code analysis.'
  echo '-a, --all                                   Checks all rules.'
}

do_format() {
  local FILES
  local NUMBER_OF_FILES
  local COUNTER
  
  FORMAT_ERROR_OCCURED=0

  echo "${MAGENTA_TEXT_COLOR}Checking format rules${RESET_TEXT_COLOR}"
  
  FILES=$(find src include test -not \( -path test/data -prune \) -type f)
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l)
  COUNTER=1
  
  while read -r FILE; do
    printf "[%3s%%] %s" "$(( (COUNTER * 100) / NUMBER_OF_FILES ))" "${FILE}"
    
    if [[ $(clang-format -output-replacements-xml "${FILE}" | grep -c "<replacement ") -ne 0 ]]; then
      if [[ ${FIX_FORMATTING} == 1 ]]; then
        clang-format "${FILE}" > '/tmp/.formatted'; cat '/tmp/.formatted' > "${FILE}"
        echo " ${RED_TEXT_COLOR}was automatically formatted.${RESET_TEXT_COLOR}"
      else
        echo " ${RED_TEXT_COLOR}is not properly formatted.${RESET_TEXT_COLOR} Please run '${GREEN_TEXT_COLOR}clang-format -i ${FILE}${RESET_TEXT_COLOR}'."
      fi
      FORMAT_ERROR_OCCURED=1
      ANY_ERROR_OCCURED=1
    else
      printf "%s" "${REPLACE_LAST_LINE}"
    fi

    COUNTER=$((++COUNTER))
  done <<< "${FILES}"
  
  if [[ ${FORMAT_ERROR_OCCURED} == 0 ]]; then
    echo "${GREEN_TEXT_COLOR}Everything is fine.${RESET_TEXT_COLOR}"
  fi
  
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR}"
}

do_include() {
  local FILES
  local NUMBER_OF_FILES
  local COUNTER
  
  INCLUDE_ERROR_OCCURED=0
  
  echo "${MAGENTA_TEXT_COLOR}Checking include rules${RESET_TEXT_COLOR}"
  
  FILES=$(find src -type f -name \*.cpp)
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l)
  COUNTER=1
  
  while read -r FILE; do
    printf "[%3s%%] %s" "$(( (COUNTER * 100) / NUMBER_OF_FILES ))" "${FILE}"
    
    OUTPUT=$(include-what-you-use -std=c++14 -Iinclude "${FILE}" 2>&1)
    if [[ "${OUTPUT}" =~ 'full include-list for' ]]; then
      echo " ${RED_TEXT_COLOR}did not pass the include rules.${RESET_TEXT_COLOR} Please run '${GREEN_TEXT_COLOR}include-what-you-use -std=c++14 -Iinclude ${FILE}${RESET_TEXT_COLOR}'."
      INCLUDE_ERROR_OCCURED=1
      ANY_ERROR_OCCURED=1
    else
      printf "%s" "${REPLACE_LAST_LINE}"
    fi
    
    COUNTER=$((++COUNTER))
  done <<< "${FILES}"
  
  if [[ ${INCLUDE_ERROR_OCCURED} == 0 ]]; then
    echo "${GREEN_TEXT_COLOR}Everything is fine.${RESET_TEXT_COLOR}"
  fi
  
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR}"
}

do_static() {
  local FILES
  local NUMBER_OF_FILES
  local COUNTER
  
  STATIC_ERROR_OCCURED=0
  
  echo "${MAGENTA_TEXT_COLOR}Performing static code analysis${RESET_TEXT_COLOR}"
  
  FILES=$(find src -type f -name \*.cpp)
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l)
  COUNTER=1
  
  while read -r FILE; do
    printf "[%3s%%] %s" "$(( (COUNTER * 100) / NUMBER_OF_FILES ))" "${FILE}"
    
    OUTPUT=$(clang-tidy "${FILE}" -- -std=c++14 -Iinclude)
    if [[ "${OUTPUT}" ]]; then
      echo "${OUTPUT}"
      STATIC_ERROR_OCCURED=1
      ANY_ERROR_OCCURED=1
    else
      printf "%s" "${REPLACE_LAST_LINE}"
    fi
    
    COUNTER=$((++COUNTER))
  done <<< "${FILES}"
  
  if [[ ${STATIC_ERROR_OCCURED} == 0 ]]; then
    echo "${GREEN_TEXT_COLOR}Everything is fine.${RESET_TEXT_COLOR}"
  fi
  
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR}"
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
      -i|--include)
        do_include
      ;;
      -s|--static)
        do_static
      ;;
      -a|--all)
        do_format
        do_include
        do_static
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

if [[ ${ANY_ERROR_OCCURED} == 1 ]]; then
  exit 1
fi
