import re
import os
import sys
import subprocess
import math
import shutil

class bcolors:
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

#TODO Summing up the above: The output should be similar to `.code.sh -f`

if not os.path.isfile('./conf.py'):
  print "Could not find Sphinx's configuration file. Make sure to start this script within Mantella's documentation root path."
  exit(1)

if not os.path.exists('./tmp'):
  os.makedirs('./tmp')
  
if not os.path.exists('./assert/images'):
  os.makedirs('./assert/images')
  
if not os.path.exists('./assert/examples'):
  os.makedirs('./assert/examples')

an_error_occured=False

hppfiles = []
for path, subdirs, files in os.walk('../include/mantella0_bits/'):
  for file in files:
    if file.endswith('.hpp'):
       hppfiles = hppfiles + [(os.path.join(path, file), os.path.join(path.replace('../include/mantella0_bits', './api_reference', 1), file.split('.')[0] + '.rst'))]

# file[0] = source file; file[1] = destination file
for file in hppfiles:
  print bcolors.OKBLUE + '[%3d%%] '% (100.0/len(hppfiles) * (hppfiles.index(file) + 1)) + bcolors.ENDC + os.path.basename(file[1]),
  sys.stdout.flush()

  headerfile = open(file[0], 'r')
  comments = ''.join(re.findall(re.compile(ur'[ ]*\/\*\*(.+?)(?=\*\/)', re.DOTALL), headerfile.read()))
  headerfile.close()
  
  # Continues with the next file if no comments where found
  if not comments: 
    print bcolors.FAIL + '  No comments found' + bcolors.ENDC
    sys.stdout.flush()
    continue

  if not os.path.exists(os.path.dirname(file[1])):
    os.makedirs(os.path.dirname(file[1]))
      
  #TODO Add comments to the regex below and add a large comment block describing the structure returned by `re.findall` (this will be done by Sebastian)
  with open(file[1], 'w') as docfile:
    for part in re.findall(re.compile(ur'(.*?)(?=(?:[ ]*\.\. code-block::|$))(([ ]*)\.\. code-block:: ([^ ]+)(?:.*?(?=:name:):name: ([^ ]+)|)(?:\n\3  :[^\n]+)*((?:\n\3  [^\n]+|\n[ ]*)+)\n|)', re.DOTALL), comments):


     docfile.write(part[0])
     
     if part[1]:
      #TODO Add plots (see the discussion about combining Mantella with a plotting library - maybe in another language)
      if 'c++' in part[3]:
        docfile.write(part[1])
        if part[4]:
          example = open('./assert/examples/' + part[4], 'w+')
          example.write(part[5].replace('\n' + part[2] + '  ','\n').strip(' \t\n\r'))
        else:
          example = open('./tmp/example.cpp', 'w+')
          example.write(part[5])
        example.close()
          
        # Compile generate code file
        output = subprocess.Popen(['c++', '-std=c++14', '-march=native', '-O3', example.name, '-lblas', '-llapack', '-o', './tmp/example'], stderr = subprocess.PIPE)
        output.wait()
        if output.returncode != 0:
         an_error_occured=True
         print bcolors.FAIL + '  Failure during compilation: \n' + bcolors.ENDC + output.stderr.read()
         continue
          
         # Execute generate code file  
        output = subprocess.Popen(['./tmp/example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output.wait()
        if output.returncode != 0:
          an_error_occured=True
          print bcolors.FAIL + '  Failure during execution: \n' + bcolors.ENDC + output.stderr.read()
          continue
          
        # Write result from code file in .rst
        docfile.write('\n' + part[2] + '.. code-block:: none\n')
        for line in output.stdout.read().split('\n'):
          docfile.write('\n' + part[2] + '  ' + line)
        docfile.write('\n')
          
      if 'image' in part[3]:
        
        image = re.findall(re.compile(ur'^(.*)\n[ ]*:octave:\n[ ]*(.*)$', re.DOTALL), part[5])
        example = open('./tmp/example.cpp', 'w+')
        example.write(image[0][0])
        example.close()
        
        docfile.write('\n' + part[2] + '.. code-block:: c++' + image[0][0])
        
        # Compile generate code file
        output = subprocess.Popen(['c++', '-std=c++14', '-march=native', '-O3', example.name, '-lblas', '-llapack', '-o', './tmp/example'], stderr = subprocess.PIPE)
        output.wait()
        if output.returncode != 0:
         an_error_occured=True
         print bcolors.FAIL + '  Failure during compilation: \n' + bcolors.ENDC + output.stderr.read()
         continue
        
        # Execute generate code file  
        output = subprocess.Popen(['./tmp/example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output.wait()
        if output.returncode != 0:
          an_error_occured=True
          print bcolors.FAIL + '  Failure during execution: \n' + bcolors.ENDC + output.stderr.read()
          continue
        
        example = open('./tmp/generate', 'w+')
        example.write('name = "./assert/images/' + part[4] + '";')
        example.write(image[0][1])
        example.close()
        
        docfile.write('\n' + part[2] + '.. code-block:: octave \n' + image[0][1])
        
        # Execute generate octave file 
        output = subprocess.Popen(['octave', './tmp/generate'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output.wait()
        if output.returncode != 0:
          an_error_occured=True
          print bcolors.FAIL + '  Failure during genarate image: \n' + bcolors.ENDC + output.stderr.read()
          continue
        
        docfile.write('\n' + part[2] + '.. image:: ../assert/images/' + part[4] + '\n')
      
    docfile.close()
    print '\x1b[2K \r',

if os.path.exists('./tmp'): shutil.rmtree('./tmp')
if an_error_occured: sys.exit(1)
else: sys.exit(0)