#!/bin/sh
rm optimisation_problem.output 2> /dev/null 

if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 optimisation_problem.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o optimisation_problem
  ./optimisation_problem.exe
  rm optimisation_problem.exe
else
  ./optimisation_problem.core.sh
  rm optimisation_problem
fi