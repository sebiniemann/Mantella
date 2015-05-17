#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm samples.mat 2> /dev/null
rm hamming_distance.png 2> /dev/null

# Compiles and run the program
./hamming_distance.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./hamming_distance.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s hamming_distance.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv hamming_distance.png ../../../assets/images/api-reference/

# Cleans up temporary files
rm hamming_distance
rm samples.mat
