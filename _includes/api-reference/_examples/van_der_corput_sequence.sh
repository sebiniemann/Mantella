#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm van_der_corput_sequence.output 2> /dev/null 

# Compiles and run the program
./van_der_corput_sequence.core.sh

# Cleans up temporary files
rm van_der_corput_sequence