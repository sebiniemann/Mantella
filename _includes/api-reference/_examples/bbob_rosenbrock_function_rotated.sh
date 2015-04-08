#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_rosenbrock_function_rotated.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_rosenbrock_function_rotated
  ./bbob_rosenbrock_function_rotated.exe
  rm bbob_rosenbrock_function_rotated.exe
else
  c++ -std=c++11 bbob_rosenbrock_function_rotated.cpp -larmadillo -o bbob_rosenbrock_function_rotated
  ./bbob_rosenbrock_function_rotated
  rm bbob_rosenbrock_function_rotated
fi

rm bbob_rosenbrock_function_rotated_surface.png bbob_rosenbrock_function_rotated_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_rosenbrock_function_rotated.m');exit;"

while [[ ! -s bbob_rosenbrock_function_rotated_surface.png || ! -s bbob_rosenbrock_function_rotated_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_rosenbrock_function_rotated_surface.png bbob_rosenbrock_function_rotated_contour.png ../../../assets/images/api-reference/