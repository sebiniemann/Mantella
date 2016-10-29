import re
import os
import sys
import subprocess
import math
import shutil

# Global variables
## Formatting
class Colors:
    NOTICE = '\033[94m'
    OK = '\033[92m'
    ERROR = '\033[91m'
    END = '\033[0m'

if not os.path.isfile('./conf.py'):
  print("Could not find Sphinx's configuration file. Make sure to start this script within Mantella's documentation root path.")
  exit(1)

os.makedirs('./tmp', exist_ok=True)
os.makedirs('./assert', exist_ok=True)
os.makedirs('./assert/examples', exist_ok=True)
os.makedirs('./assert/images', exist_ok=True)

an_error_occured = False

# Finds all header files inside `../include`
# *Note:* This is only done separately to extract the provide a progress bar, wherefore the number of files needs to be
# known beforehand.
files = []
for path, subdirs, subfiles in os.walk('../include'):
  for file in subfiles:
    if file.endswith('.hpp'):
      files.append([os.path.join(path, file), os.path.join('./api_reference', *(path.split(os.path.sep)[3:]), file.split('.')[0] + '.rst')])

# Iterates over each header and:
# 1. Checks whether the files contains an documentation block (i.e. `/** ... */`) or not (only documented files are 
#    further processed)
# 2. Looks for `.. code-block::[c++|image]` blocks
#    a) `c++` blocks are compiled and executed. Their output is appended (with its own `.. code-block::` block) to the  
#        code example.
#    b) `image` blocks are also compiles and executed. However, we also expect an `:octave:` delimiter, separating the 
#       data generation in C++ from the visualisation by Octave. The black is then replaced with the image by Octave,
#       appended by its data generation and visualisation (each in its own `.. code-block::` block).
for file in files:
  print(Colors.NOTICE + '[{:3d}%]'.format(int(100.0 * (files.index(file) + 1) / len(files))) + Colors.END + ' ' + os.path.basename(file[1]), end="")

  headerfile = open(file[0], mode='r', encoding='utf-8')
  comments = ''.join(re.findall(r'[ ]*\/\*\*(.+?)(?=\*\/)', headerfile.read(), re.DOTALL))
  headerfile.close()

  if not comments: 
    print(' ' + Colors.ERROR + 'No comments found' + Colors.END)
    continue

  # Create subdirectory if missing
  os.makedirs(os.path.dirname(file[1]), exist_ok=True)

  with open(file[1], 'w') as docfile:
    for part in re.findall(r''
      '(.*?)(?=(?:[ ]*\.\. code-block::|$))' # Matches anything until a `.. code-block::` occurs.
      '('
        '([ ]*)\.\. code-block:: ([^ ]+)' # Matches the type of `.. code-block::` (`c++`, `image` or anything else).
        '(?:.*?(?=:name:):name: ([^ ]+)|)' # Skips anything before `:name:` occurs and matches the name.
        '(?:\n\3  :[^\n]+)*' # Skips all further lines that have `.. code-block::` indentation but start with ':'.
        '((?:\n\3  [^\n]+|\n[ ]*)+)\n|' # Matches all lines having `.. code-block::` indentation and empty lines.
      ')', comments, re.DOTALL):
      docfile.write(part[0])

      if part[1]:
        # Processes C++ blocks
        if 'c++' in part[3]:
          docfile.write(part[1])

          if part[4]:
            example = open('./assert/examples/' + part[4], mode='w+')
            example.write(part[5].replace('\n' + part[2] + '  ','\n').strip(' \t\n\r'))
          else:
            example = open('./tmp/example.cpp', mode='w+')
            example.write(part[5])
          example.close()

          # Compile generate code file
          output = subprocess.Popen(['c++', '-std=c++14', '-march=native', '-O3', example.name, '-lblas', '-llapack', '-o', './tmp/example'], stderr = subprocess.PIPE)
          output.wait()
          if output.returncode != 0:
            an_error_occured = True
            print(Colors.ERROR + '  Failure during compilation: \n' + Colors.END + output.stderr.read())
            continue

           # Execute generate code file  
          output = subprocess.Popen(['./tmp/example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
          output.wait()
          if output.returncode != 0:
            an_error_occured = True
            print(Colors.ERROR + '  Failure during execution: \n' + Colors.END + output.stderr.read())
            continue

          # Write result from code file in .rst
          docfile.write('\n' + part[2] + '.. code-block:: none\n')
          for line in output.stdout.read().split('\n'):
            docfile.write('\n' + part[2] + '  ' + line)
          docfile.write('\n')

        # Processes image blocks
        if 'image' in part[3]:        
          image = re.findall(r'^(.*)\n[ ]*:octave:\n[ ]*(.*)$', part[5], re.DOTALL)
          example = open('./tmp/example.cpp', mode='w+')
          example.write(image[0][0])
          example.close()

          docfile.write('\n' + part[2] + '.. code-block:: c++' + image[0][0])

          # Compile generate code file
          output = subprocess.Popen(['c++', '-std=c++14', '-march=native', '-O3', example.name, '-lblas', '-llapack', '-o', './tmp/example'], stderr = subprocess.PIPE)
          output.wait()
          if output.returncode != 0:
            an_error_occured = True
            print(Colors.ERROR + '  Failure during compilation: \n' + Colors.END + output.stderr.read())
            continue

          # Execute generate code file  
          output = subprocess.Popen(['./tmp/example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
          output.wait()
          if output.returncode != 0:
            an_error_occured = True
            print(Colors.ERROR + '  Failure during execution: \n' + Colors.END + output.stderr.read())
            continue

          example = open('./tmp/generate', mode='w+')
          example.write('name = "./assert/images/' + part[4] + '";')
          example.write(image[0][1])
          example.close()

          docfile.write('\n' + part[2] + '.. code-block:: octave \n' + image[0][1])

          # Execute generate octave file 
          output = subprocess.Popen(['octave', './tmp/generate'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
          output.wait()
          if output.returncode != 0:
            an_error_occured = True
            print(olors.ERROR + '  Failure during genarate image: \n' + Colors.END + output.stderr.read())
            continue

          docfile.write('\n' + part[2] + '.. image:: ../assert/images/' + part[4] + '\n')

    docfile.close()
    print('\x1b[2K \r', end="")

if os.path.exists('./tmp'):
  shutil.rmtree('./tmp')

if an_error_occured:
  sys.exit(1)

sys.exit(0)