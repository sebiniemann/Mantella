#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_linear_slope.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_linear_slope
  ./bbob_linear_slope.exe
  rm bbob_linear_slope.exe
else
  ./bbob_linear_slope.core.sh
  rm bbob_linear_slope
fi

rm bbob_linear_slope_surface.png bbob_linear_slope_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_linear_slope.m');exit;"

while [[ ! -s bbob_linear_slope_surface.png || ! -s bbob_linear_slope_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_linear_slope_surface.png bbob_linear_slope_contour.png ../../../assets/images/api-reference/