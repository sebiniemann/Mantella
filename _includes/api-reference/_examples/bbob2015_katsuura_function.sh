#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob2015_katsuura_function.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob2015_katsuura_function
  ./bbob2015_katsuura_function.exe
  rm bbob2015_katsuura_function.exe
else
  c++ -std=c++11 bbob2015_katsuura_function.cpp -larmadillo -o bbob2015_katsuura_function
  ./bbob2015_katsuura_function
  rm bbob2015_katsuura_function
fi

rm bbob2015_katsuura_function_surface.png bbob2015_katsuura_function_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob2015_katsuura_function.m');exit;"

while [[ ! -s bbob2015_katsuura_function_surface.png || ! -s bbob2015_katsuura_function_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob2015_katsuura_function_surface.png bbob2015_katsuura_function_contour.png ../../../assets/images/api-reference/