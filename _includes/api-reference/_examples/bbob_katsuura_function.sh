#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm X.mat Y.mat Z.mat 2> /dev/null
rm bbob_katsuura_function_surface.png bbob_katsuura_function_contour.png 2> /dev/null

# Compiles and run the program
./bbob_katsuura_function.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_katsuura_function.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s bbob_katsuura_function_surface.png || ! -s bbob_katsuura_function_contour.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv bbob_katsuura_function_surface.png bbob_katsuura_function_contour.png ../../../assets/images/api-reference/

# Cleans up temporary files
rm bbob_katsuura_function
rm X.mat Y.mat Z.mat
