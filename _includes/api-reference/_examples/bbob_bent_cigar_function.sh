#!/bin/sh
if [[ "$(uname -o)" = "Msys" ]]; then
  c++ -std=c++11 bbob_bent_cigar_function.cpp -LC:/Programme/OpenBLAS/lib -lopenblas -IC:/Programme/Armadillo/include -IC:/Programme/Mantella/include -o bbob_bent_cigar_function
  ./bbob_bent_cigar_function.exe
  rm bbob_bent_cigar_function.exe
else
  c++ -std=c++11 bbob_bent_cigar_function.cpp -larmadillo -o bbob_bent_cigar_function
  ./bbob_bent_cigar_function
  rm bbob_bent_cigar_function
fi

rm bbob_bent_cigar_function_surface.png bbob_bent_cigar_function_contour.png 2> /dev/null 
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_bent_cigar_function.m');exit;"

while [[ ! -s bbob_bent_cigar_function_surface.png || ! -s bbob_bent_cigar_function_contour.png ]]; do
  sleep 2
done
sleep 5

rm X.mat Y.mat Z.mat
mv bbob_bent_cigar_function_surface.png bbob_bent_cigar_function_contour.png ../../../assets/images/api-reference/