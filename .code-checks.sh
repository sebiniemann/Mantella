#!/bin/bash

RED_TEXT_COLOR=$(tput setaf 1)
GREEN_TEXT_COLOR=$(tput setaf 2)
MAGENTA_TEXT_COLOR=$(tput setaf 5)
RESET_TEXT_COLOR=$(tput sgr0)

if [ ! -f "./include/mantella" ]; then
    echo "${RED_TEXT_COLOR}Could not find Mantella. Make sure to start this script within Mantella's root path.${RESET_TEXT_COLOR}";
    exit 1;
fi;

ANY_ERROR_OCCURED=0;
if [ -z "${1}" ] || [ "$1" == "format" ]; then
  FORMAT_ERROR_OCCURED=0;

  echo "${MAGENTA_TEXT_COLOR}Checking format rules${RESET_TEXT_COLOR}";
  FILES=$(find src include -type f);
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l);
  COUNTER=1;
  
  while read file; do
    printf "[%3s%%] " "$(( (COUNTER * 100) / NUMBER_OF_FILES ))"
    
    if [[ `clang-format -output-replacements-xml "${file}" | grep "<replacement " | wc -l` -ne 0 ]]; then
      echo "${RED_TEXT_COLOR}${file}${RESET_TEXT_COLOR} is not properly formatted. Please run '${GREEN_TEXT_COLOR}clang-format -i ${file}${RESET_TEXT_COLOR}'.";
      FORMAT_ERROR_OCCURED=1;
      ANY_ERROR_OCCURED=1;
    else
      printf "\r";
    fi;

    COUNTER=$((++COUNTER));
  done <<< "${FILES}";
  
  if [[ ${FORMAT_ERROR_OCCURED} == 0 ]]; then
    echo "${GREEN_TEXT_COLOR}Everything is fine.${RESET_TEXT_COLOR}";
  fi;
  
  # The whitespace at the end is intentional, in order to override the progress bar ('printf "[%3s%%]"' from above), which is 6 characters long.
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR} ";
fi;
  
if [ -z "${1}" ] || [ "${1}" == "include" ]; then
  INCLUDE_ERROR_OCCURED=0;
  
  echo "${MAGENTA_TEXT_COLOR}Checking include rules${RESET_TEXT_COLOR}";
  FILES=$(find src -type f);
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l);
  COUNTER=1;
  
  while read file; do
    printf "[%3s%%] " "$(( (COUNTER * 100) / NUMBER_OF_FILES ))"
    
    OUTPUT=$(include-what-you-use -std=c++11 -I include ${file} 2>&1);
    if [[ "${OUTPUT}" =~ "full include-list for" ]]; then
      echo "${RED_TEXT_COLOR}${file}${RESET_TEXT_COLOR} did not pass the include rules. Please run '${GREEN_TEXT_COLOR}include-what-you-use -std=c++11 -I include ${file}${RESET_TEXT_COLOR}'.";
      INCLUDE_ERROR_OCCURED=1;
      ANY_ERROR_OCCURED=1;
    else
      printf "\r";
    fi;
    
    COUNTER=$((++COUNTER));
  done <<< "${FILES}";
  
  if [[ ${INCLUDE_ERROR_OCCURED} == 0 ]]; then
    echo "${GREEN_TEXT_COLOR}Everything is fine.${RESET_TEXT_COLOR}";
  fi;
  
  # The whitespace at the end is intentional, in order to override the progress bar ('printf "[%3s%%]"' from above), which is 6 characters long.
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR} ";
fi;

if [[ ${ANY_ERROR_OCCURED} == 1 ]]; then
  exit 1;
fi;