#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 hash.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o hash
  ./hash.exe
  rm hash.exe
else
  ./hash.core.sh
  rm hash
fi

rm hash.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./hash.m');exit;"

while [[ ! -s hash.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv hash.png ../../../assets/images/api-reference/