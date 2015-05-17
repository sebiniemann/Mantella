#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm optimisation_problem.output 2> /dev/null 

# Compiles and run the program
./optimisation_problem.core.sh

# Cleans up all temporary files
rm optimisation_problem