#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm X.mat Y.mat Z.mat 2> /dev/null
rm bbob_schaffers_f7_function_ill_conditioned_surface.png bbob_schaffers_f7_function_ill_conditioned_contour.png 2> /dev/null

# Compiles and run the program
./bbob_schaffers_f7_function_ill_conditioned.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./bbob_schaffers_f7_function_ill_conditioned.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s bbob_schaffers_f7_function_ill_conditioned_surface.png || ! -s bbob_schaffers_f7_function_ill_conditioned_contour.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv bbob_schaffers_f7_function_ill_conditioned_surface.png bbob_schaffers_f7_function_ill_conditioned_contour.png ../../../assets/images/api-reference/

# Cleans up temporary files
rm bbob_schaffers_f7_function_ill_conditioned
rm X.mat Y.mat Z.mat
