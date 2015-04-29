#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_step_ellipsoidal_function.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_step_ellipsoidal_function
  ./bbob_step_ellipsoidal_function.exe
  rm bbob_step_ellipsoidal_function.exe
else
  ./bbob_step_ellipsoidal_function.core.sh
  rm bbob_step_ellipsoidal_function
fi

rm bbob_step_ellipsoidal_function_surface.png bbob_step_ellipsoidal_function_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_step_ellipsoidal_function.m');exit;"

while [[ ! -s bbob_step_ellipsoidal_function_surface.png || ! -s bbob_step_ellipsoidal_function_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_step_ellipsoidal_function_surface.png bbob_step_ellipsoidal_function_contour.png ../../../assets/images/api-reference/