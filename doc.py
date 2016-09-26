import re
import os
import sys
import subprocess

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

error = False
verbose = False
if len(sys.argv) > 1 :
	if sys.argv[1] == '-v': verbose = True

#TODO Summing up the above: The output should be similar to `.code.sh -f`
for path, subdirs, files in os.walk('./include/'):
  p = int(100/len(files))
  for file in files:
    if file.endswith('.hpp'):
      print bcolors.OKBLUE + '[%3d%%] '% (p * (files.index(file)+1)) + bcolors.HEADER + path.split('/')[-1] + ' ' + bcolors.ENDC + file,
      sys.stdout.flush()

      headerfile = open(os.path.join(path, file), 'r')
      comments = ''.join(re.findall(re.compile(ur'[ ]*\/\*\*(.+?)(?=\*\/)', re.DOTALL), headerfile.read()))
      headerfile.close()
      
      # Continues with the next file if no comments where found
      if not comments: 
        print bcolors.WARNING + '  No comments found' + bcolors.ENDC
        error = True
        continue

      docpath = path.replace('./include/mantella_bits/', './doc/', 1)
      if not os.path.exists(docpath):
        os.makedirs(docpath)

      docfile = open(docpath + '/' + file.split('.')[0] + '.rst', 'w')
      #TODO Add comments to the regex below and add a large comment block describing the structure returned by `re.findall` (this will be done by Sebastian)
      for part in re.findall(re.compile(ur'(.*?)(?=(?:[ ]*\.\. code-block::|$))(([ ]*)\.\. code-block:: ([^ ]+)(?:.*?(?=:name:):name: ([^ ]+)|)(?:\n\3  :[^\n]+)*((?:\n\3  [^\n]+|\n[ ]*)+)|)', re.DOTALL), comments):
        docfile.write(part[0] + part[1])

        if part[1]:
          #TODO Add plots (see the discussion about combining Mantella with a plotting library - maybe in another language)
          if "c++" in part[3]:
            if part[4]:
              example = open('./doc/tmp/' + part[4] + '.cpp', 'w')
            else:
              example = open('./doc/tmp/example.cpp', 'w')
            example.write(part[5])
            example.close()

            # Compile generate code file
            output = subprocess.Popen(['c++', '-std=c++14', '-march=native', '-O3', './doc/tmp/' + example.name, '-lblas', '-llapack', '-o', './doc/tmp/example'], stderr = subprocess.PIPE)
            output.wait()
            if (output.returncode != 0):
              print bcolors.FAIL + '  Failure during compilation: \n' + bcolors.ENDC
              if verbose : print output.stderr.read()
              error = True
              continue

            # Execute generate code file  
            output = subprocess.Popen(['./doc/tmp/example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if (output.returncode != 0):
              print bcolors.FAIL + '  Failure during execution: \n' + bcolors.ENDC 
              if verbose : print output.stderr.read()
              error = True
              continue

            # Write result from code file in .rst
            docfile.write('\n' + part[2] + '.. code-block:: none\n')
            for line in output.stdout.read().split('\n'):
              docfile.write('\n' + part[2] + '  ' + line)
              
      docfile.close()
      print '\x1b[2K \r',
      sys.stdout.flush()

if os.path.exists('./doc/tmp'):  os.removedirs('./doc/tmp')
if error: sys.exit(os.EX_SOFTWARE)
else: sys.exit(os.EX_OK)