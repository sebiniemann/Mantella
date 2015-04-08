#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_weierstrass_function.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_weierstrass_function
  ./bbob_weierstrass_function.exe
  rm bbob_weierstrass_function.exe
else
  c++ -std=c++11 bbob_weierstrass_function.cpp -larmadillo -o bbob_weierstrass_function
  ./bbob_weierstrass_function
  rm bbob_weierstrass_function
fi

rm bbob_weierstrass_function_surface.png bbob_weierstrass_function_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_weierstrass_function.m');exit;"

while [[ ! -s bbob_weierstrass_function_surface.png || ! -s bbob_weierstrass_function_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_weierstrass_function_surface.png bbob_weierstrass_function_contour.png ../../../assets/images/api-reference/