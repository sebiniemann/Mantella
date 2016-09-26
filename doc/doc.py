import re
import os
import sys
import subprocess
import math

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

#TODO Summing up the above: The output should be similar to `.code.sh -f`

if not os.path.isfile('./conf.py'):
  print "Could not find Sphinx's configuration file. Make sure to start this script within Mantella's documentation root path."
  exit(1)

if not os.path.exists('./tmp'):
  os.makedirs('./tmp')

an_error_occured=False

hppfiles = []
for path, subdirs, files in os.walk('../include/mantella_bits/'):
  for file in files:
    if file.endswith('.hpp'):
       hppfiles = hppfiles + [(os.path.join(path, file), os.path.join(path.replace('../include/mantella_bits', './api_reference', 1), file.split('.')[0] + '.rst'))]

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
 
  docfile = open(file[1], 'w')
      
  #TODO Add comments to the regex below and add a large comment block describing the structure returned by `re.findall` (this will be done by Sebastian)
  for part in re.findall(re.compile(ur'(.*?)(?=(?:[ ]*\.\. code-block::|$))(([ ]*)\.\. code-block:: ([^ ]+)(?:.*?(?=:name:):name: ([^ ]+)|)(?:\n\3  :[^\n]+)*((?:\n\3  [^\n]+|\n[ ]*)+)|)', re.DOTALL), comments):
    docfile.write(part[0] + part[1])
        
    if part[1]:
      #TODO Add plots (see the discussion about combining Mantella with a plotting library - maybe in another language)
      if 'c++' in part[3]:
        if part[4]:
          example = open('./tmp/' + part[4] + '.cpp', 'w+')
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
          
    if 'image' in part[3]:
      print 'image detected'
      #^(.*)\n[ ]*:octave:\n[ ]*(.*)$
      
    docfile.close()
    print '\x1b[2K \r',

if os.path.exists('./doc/tmp'):  os.removedirs('./doc/tmp')
if an_error_occured: sys.exit(os.EX_SOFTWARE)