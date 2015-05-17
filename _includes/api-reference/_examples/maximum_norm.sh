#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm samples.mat 2> /dev/null
rm maximum_norm.png 2> /dev/null

# Compiles and run the program
./maximum_norm.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./maximum_norm.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s maximum_norm.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv maximum_norm.png ../../../assets/images/api-reference/

# Cleans up temporary files
rm maximum_norm
rm samples.mat
