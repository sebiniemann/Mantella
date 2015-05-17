#!/bin/sh
# Cleans up previous output files (ignore errors, e.g. if the file is already removed)
rm halton_sequence.png 2> /dev/null 
rm pseudo_random_sequence.png 2> /dev/null 

# Compiles and run the program
./halton_sequence.core.sh

# Runs Matlab ...
matlab -nodisplay -nosplash -nodesktop -r "run('./halton_sequence.m');exit;"

# ... and waits till Matlab is actually finished.
while [[ ! -s halton_sequence.png || ! -s pseudo_random_sequence.png ]]; do
  sleep 2
done
sleep 5

# Moves the generated images into a web-accessable folder. 
mv halton_sequence.png ../../../assets/images/api-reference/
mv pseudo_random_sequence.png ../../../assets/images/api-reference/

# Cleans up all temporary files
rm halton_sequence
rm halton_sequence.mat
rm pseudo_random_sequence.mat