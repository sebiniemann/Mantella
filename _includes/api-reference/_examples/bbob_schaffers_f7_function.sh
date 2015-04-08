#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_schaffers_f7_function.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_schaffers_f7_function
  ./bbob_schaffers_f7_function.exe
  rm bbob_schaffers_f7_function.exe
else
  c++ -std=c++11 bbob_schaffers_f7_function.cpp -larmadillo -o bbob_schaffers_f7_function
  ./bbob_schaffers_f7_function
  rm bbob_schaffers_f7_function
fi

rm bbob_schaffers_f7_function_surface.png bbob_schaffers_f7_function_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_schaffers_f7_function.m');exit;"

while [[ ! -s bbob_schaffers_f7_function_surface.png || ! -s bbob_schaffers_f7_function_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_schaffers_f7_function_surface.png bbob_schaffers_f7_function_contour.png ../../../assets/images/api-reference/