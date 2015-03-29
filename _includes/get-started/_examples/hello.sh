#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 hello.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o hello
  ./hello.exe > hello.output
else
  c++ -std=c++11 hello.cpp -larmadillo -o hello
  ./hello > hello.output
fi
