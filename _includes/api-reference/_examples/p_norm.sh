#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm samples.mat 2> /dev/null
rm p_norm.png 2> /dev/null

# Compiles and run the program
./p_norm.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./p_norm.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s p_norm.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv p_norm.png ../../../assets/images/api-reference/

# Cleans up temporary files
rm p_norm
rm samples.mat
