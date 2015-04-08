#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob2015_schaffers_f7_function_ill_conditioned.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob2015_schaffers_f7_function_ill_conditioned
  ./bbob2015_schaffers_f7_function_ill_conditioned.exe
  rm bbob2015_schaffers_f7_function_ill_conditioned.exe
else
  c++ -std=c++11 bbob2015_schaffers_f7_function_ill_conditioned.cpp -larmadillo -o bbob2015_schaffers_f7_function_ill_conditioned
  ./bbob2015_schaffers_f7_function_ill_conditioned
  rm bbob2015_schaffers_f7_function_ill_conditioned
fi

rm bbob2015_schaffers_f7_function_ill_conditioned_surface.png bbob2015_schaffers_f7_function_ill_conditioned_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob2015_schaffers_f7_function_ill_conditioned.m');exit;"

while [[ ! -s bbob2015_schaffers_f7_function_ill_conditioned_surface.png || ! -s bbob2015_schaffers_f7_function_ill_conditioned_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob2015_schaffers_f7_function_ill_conditioned_surface.png bbob2015_schaffers_f7_function_ill_conditioned_contour.png ../../../assets/images/api-reference/