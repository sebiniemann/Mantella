#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_composite_griewank_rosenbrock_function_f8f2.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_composite_griewank_rosenbrock_function_f8f2
  ./bbob_composite_griewank_rosenbrock_function_f8f2.exe
  rm bbob_composite_griewank_rosenbrock_function_f8f2.exe
else
  ./bbob_composite_griewank_rosenbrock_function_f8f2.core.sh
  rm bbob_composite_griewank_rosenbrock_function_f8f2
fi

rm bbob_composite_griewank_rosenbrock_function_f8f2_surface.png bbob_composite_griewank_rosenbrock_function_f8f2_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_composite_griewank_rosenbrock_function_f8f2.m');exit;"

while [[ ! -s bbob_composite_griewank_rosenbrock_function_f8f2_surface.png || ! -s bbob_composite_griewank_rosenbrock_function_f8f2_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_composite_griewank_rosenbrock_function_f8f2_surface.png bbob_composite_griewank_rosenbrock_function_f8f2_contour.png ../../../assets/images/api-reference/