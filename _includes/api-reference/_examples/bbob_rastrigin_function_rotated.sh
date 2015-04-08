#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob2015_rastrigin_function_rotated.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob2015_rastrigin_function_rotated
  ./bbob2015_rastrigin_function_rotated.exe
  rm bbob2015_rastrigin_function_rotated.exe
else
  c++ -std=c++11 bbob2015_rastrigin_function_rotated.cpp -larmadillo -o   
  ./bbob2015_rastrigin_function_rotated
  rm bbob2015_rastrigin_function_rotated
fi

rm bbob2015_rastrigin_function_rotated_surface.png bbob2015_rastrigin_function_rotated_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob2015_rastrigin_function_rotated.m');exit;"

while [[ ! -s bbob2015_rastrigin_function_rotated_surface.png || ! -s bbob2015_rastrigin_function_rotated_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob2015_rastrigin_function_rotated_surface.png bbob2015_rastrigin_function_rotated_contour.png ../../../assets/images/api-reference/