import glob
import os
import re
import shutil
import subprocess
import sys
from contextlib import contextmanager

@contextmanager
def cd(dir):
    prevdir = os.getcwd()
    os.chdir(os.path.expanduser(dir))
    try:
        yield
    finally:
        os.chdir(prevdir)

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

os.makedirs('./.tmp', exist_ok=True)
os.makedirs('./static', exist_ok=True)
os.makedirs('./static/examples', exist_ok=True)
os.makedirs('./static/images', exist_ok=True)
os.makedirs('./static/animations', exist_ok=True)

an_error_occured = False

# Finds all header files inside `../include`
# *Note:* This is only done separately to extract the provide a progress bar, wherefore the number of files needs to be
# known beforehand.
files = []
for file in glob.glob('../include/*/*.hpp'):
  files.append([file, os.path.join('./api_reference', *(file.replace('.hpp', '.rst').split(os.path.sep)[3:]))])
for file in glob.glob('../include/*/*/*.hpp'):
  files.append([file, os.path.join('./api_reference', *(file.replace('.hpp', '.include').split(os.path.sep)[3:]))])
  
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
  print(Colors.NOTICE + '[{:3d}%]'.format(int(100.0 * (files.index(file) + 1) / len(files))) + Colors.END + ' ' + os.path.basename(file[1]), end="", flush=True)
  

  headerfile = open(file[0], mode='r', encoding='utf-8')
  comments = ''.join(re.findall(r'[ ]*\/\*\*(.+?)(?=\*\/)', headerfile.read(), re.DOTALL))
  headerfile.close()

  if not comments: 
    an_error_occured = True
    print(' ' + Colors.ERROR + 'No comments found' + Colors.END)
    continue

  # Create subdirectory if missing
  os.makedirs(os.path.dirname(file[1]), exist_ok=True)

  with open(file[1], mode='w', encoding='utf-8') as docfile:
    # for part in re.findall(r''
      # '(.*?)(?=(?:[ ]*\.\. code-block::|$))' # Matches anything until a `.. code-block::` occurs.
      # '('
        # '([ ]*)\.\. code-block:: ([^ ]+)' # Matches the type of `.. code-block::` (`c++`, `image` or anything else).
        # '(?:.*?(?=:name:):name: ([^ ]+)|)' # Skips anything before `:name:` occurs and matches the name.
        # '(?:\n\3  :[^\n]+)*' # Skips all further lines that have `.. code-block::` indentation but start with ':'.
        # '((?:\n\3  [^\n]+|\n[ ]*)+)\n|' # Matches all lines having `.. code-block::` indentation and empty lines.
      # ')', comments, re.DOTALL):
    for part in re.findall(r'(.*?)(?=(?:[ ]*\.\. code-block::|$))(([ ]*)\.\. code-block:: ([^ ]+)(?:.*?(?=:name:):name: ([^ ]+)|)(?:\n\3  :[^\n]+)*((?:\n\3  [^\n]+|\n[ ]*)+)\n|)', comments, re.DOTALL):
      docfile.write(part[0])

      if part[1]:
        # Processes C++ blocks
        if 'c++' in part[3]:
          if part[4]:
            example = open('./static/examples/' + part[4], mode='w+')
            example.write(part[5].replace('\n' + part[2] + '  ', '\n').strip(' \t\n\r'))
            example.close()
          
          with cd("./.tmp"):
            example = open('./example.cpp', mode='w+')
            example.write(part[5])
            example.close()
            
            # Compile generate code file
            output = subprocess.Popen(['c++', '-std=c++14', '-I../../include', example.name, '-o', './example'], stderr = subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during compilation: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue

           # Execute generate code file  
            output = subprocess.Popen(['./example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during execution: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue
              
            docfile.write(part[2] + '.. container:: example')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    <details>')
            docfile.write('\n' + part[2] + '      <summary>Code example</summary>')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '  .. code-block:: c++')
            for line in part[5].split('\n'):
              docfile.write('  ' + line + '\n')
            docfile.write(part[2] + '  .. code-block:: none')
            docfile.write('\n')
            for line in str(output.stdout.read(), encoding='utf-8').split('\n'):
              docfile.write('\n' + part[2] + '    ' + line)
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    </details>')
            docfile.write('\n\n')

        # Processes image blocks
        if 'image' in part[3]:        
          image = re.findall(r'^(.*)\n[ ]*:octave:\n[ ]*(.*)$', part[5], re.DOTALL)
          
          with cd("./.tmp"):
            example = open('./example.cpp', mode='w+')
            example.write(image[0][0])
            example.close()

            # Compile generate code file
            output = subprocess.Popen(['c++', '-std=c++14', '-I../../include', example.name, '-o', './example'], stderr = subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during compilation: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue

            # Execute generate code file  
            output = subprocess.Popen(['./example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during execution: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue

            example = open('./generate.m', mode='w+')
            example.write('name = "../static/images/' + part[4] + '";')
            example.write(image[0][1])
            example.close()

            # Execute generate octave file 
            output = subprocess.Popen(['octave', './generate.m'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during genarate image: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue
            
            docfile.write(part[2] + '.. image:: ../static/images/' + part[4])
            docfile.write('\n')
            docfile.write('\n' + part[2] + '.. container:: image')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    <details>')
            docfile.write('\n' + part[2] + '      <summary>Code example</summary>')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '  .. code-block:: c++')
            for line in image[0][0].split('\n'):
              docfile.write('  ' + line + '\n')
            docfile.write(part[2] + '  .. code-block:: octave')
            for line in image[0][1].split('\n'):
              docfile.write('\n' + '  ' + line)
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    </details>')
            docfile.write('\n\n')
            
        # Processes animation blocks
        if 'animation' in part[3]:        
          animation = re.findall(r'^(.*)\n[ ]*:octave:\n[ ]*(.*)$', part[5], re.DOTALL)
          
          with cd("./.tmp"):
            example = open('./example.cpp', mode='w+')
            example.write(animation[0][0])
            example.close()

            # Compile generate code file
            output = subprocess.Popen(['c++', '-std=c++14', '-I../../include', example.name, '-o', './example'], stderr = subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during compilation: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue

            # Execute generate code file  
            output = subprocess.Popen(['./example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during execution: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue

            example = open('./generate.m', mode='w+')
            example.write('name ="'+ part[4].split('.')[0] + '_";')
            example.write(animation[0][1])
            example.close()

            # Execute generate octave file 
            output = subprocess.Popen(['octave', './generate.m'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during genarate image: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue
              
            # Execute generate animations file 
            output = subprocess.Popen(['ffmpeg', '-an', '-i', part[4].split('.')[0] + '_%d.png', '-framerate', '3', '-pix_fmt', 'yuv420p', '-movflags', 'faststart', '../static/animations/' + part[4]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              an_error_occured = True
              print(Colors.ERROR + '  Failure during genarate animations: \n' + Colors.END + str(output.stderr.read(), encoding='utf-8'))
              continue
            
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    <video width="320" height="240" controls>')
            docfile.write('\n' + part[2] + '      <source src="../_animations/' + part[4] + '" type="video/mp4">')
            docfile.write('\n' + part[2] + '    </video>')
            docfile.write('\n')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '.. container:: image')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    <details>')
            docfile.write('\n' + part[2] + '      <summary>Code example</summary>')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '  .. code-block:: c++')
            for line in animation[0][0].split('\n'):
              docfile.write('  ' + line + '\n')
            docfile.write(part[2] + '  .. code-block:: octave')
            for line in animation[0][1].split('\n'):
              docfile.write('\n' + '  ' + line)
            docfile.write('\n' + part[2] + '  .. raw:: html')
            docfile.write('\n')
            docfile.write('\n' + part[2] + '    </details>')
            docfile.write('\n\n')

    docfile.close()
    print('\x1b[2K \r', end="")

if os.path.exists('./.tmp'):
  shutil.rmtree('./.tmp')

if an_error_occured:
  sys.exit(1)

sys.exit(0)