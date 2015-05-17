#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm samples.mat 2> /dev/null
rm euclidean_distance.png 2> /dev/null

# Compiles and run the program
./euclidean_distance.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./euclidean_distance.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s euclidean_distance.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv euclidean_distance.png ../../../assets/images/api-reference/

# Cleans up temporary files
rm euclidean_distance
rm samples.mat
