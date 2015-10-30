#!/bin/bash

RED_TEXT_COLOR=$(tput setaf 1)
GREEN_TEXT_COLOR=$(tput setaf 2)
MAGENTA_TEXT_COLOR=$(tput setaf 5)
RESET_TEXT_COLOR=$(tput sgr0)

if [ -z "${1}" ] || [ "$1" == "format" ]; then
  echo "${MAGENTA_TEXT_COLOR}Checking format rules${RESET_TEXT_COLOR}";
  FILES=$(find src include -type f);
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l);
  COUNTER=1;
  
  echo "${FILES}" | while read file; do
    printf "[%3s%%] " "$(( (COUNTER * 100) / NUMBER_OF_FILES ))"
    
    if [[ `clang-format-3.6 -output-replacements-xml "${file}" | grep "<replacement " | wc -l` -ne 0 ]]; then
      echo "${RED_TEXT_COLOR}${file}${RESET_TEXT_COLOR} is not properly formatted. Please run '${GREEN_TEXT_COLOR}clang-format -i ${file}${RESET_TEXT_COLOR}'.";
    else
      printf "\r";
    fi;
    
    COUNTER=$((++COUNTER));
  done;
  
  # The whitespace at the end is intentional, in order to override the progress bar ('printf "[%3s%%]"' from above), which is 6 characters long.
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR} ";
fi;
  
if [ -z "${1}" ] || [ "${1}" == "include" ]; then
  echo "${MAGENTA_TEXT_COLOR}Checking include rules${RESET_TEXT_COLOR}";
  FILES=$(find src -type f);
  NUMBER_OF_FILES=$(echo "${FILES}" | wc -l);
  COUNTER=1;
  
  echo "${FILES}" | while read file; do
    printf "[%3s%%] " "$(( (COUNTER * 100) / NUMBER_OF_FILES ))"
    
    OUTPUT=$(iwyu -std=c++11 -Weverything -Wno-c++98-compat -Wno-weak-vtables -Wno-exit-time-destructors -Wno-global-constructors -Wno-padded -Wno-unused-parameter -I include ${file} 2>&1);
    if [[ ! "${OUTPUT}" =~ " has correct " ]]; then
      echo "${RED_TEXT_COLOR}${file}${RESET_TEXT_COLOR} did not pass the include rules. Please run '${GREEN_TEXT_COLOR}iwyu -std=c++11 -I include ${file}${RESET_TEXT_COLOR}'.";
    else
      printf "\r";
    fi;
    
    COUNTER=$((++COUNTER));
  done;
  
  # The whitespace at the end is intentional, in order to override the progress bar ('printf "[%3s%%]"' from above), which is 6 characters long.
  echo "${MAGENTA_TEXT_COLOR}done.${RESET_TEXT_COLOR} ";
fi;