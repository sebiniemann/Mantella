Getting started
===============
Prerequirements
-------------------
You must have installed mantella and either MatLab or octave on your PC.  

Compile the mex file
------------------------
Using octave  
1. Open console and go to `mantellaLibraryPath/BBOBMexfiles/`.  
2. Start octave.  
3. Execute command: `mkoctfile --mex -std=c++11 -I../include -L../lib/ sphereFunction.cpp -lmantella`.  
4. Run mex file: `result = sphereFunction([4,5,4])`.  
5. The correct output for this example is `result = 16`.  

Using MatLab  
2. Using C++ compiler(MinGW): `mex -setup cpp`.  
3. start command: `mex sphereFunction.cpp`.  
4. Run mex file: `result = sphereFunction([4,5,4])`.  
5. The correct output for this example is `result = 16`.  