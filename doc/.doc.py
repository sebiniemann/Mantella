import re
import os
import subprocess

#TODO Add colour to console outputs
#TODO Add progress indication
#TODO Overwrite each line if no error occurred
#TODO Summing up the above: The output should be similar to `.code.sh -f`
for path, subdirs, files in os.walk('./include/mantella_bits/'):
  for file in files:
    if file.endswith('.hpp'):
      print file
    
      headerfile = open(os.path.join(path, file), 'r')
      comments = ''.join(re.findall(re.compile(ur'[ ]*\/\*\*(.+?)(?=\*\/)', re.DOTALL), headerfile.read()))
      headerfile.close()
      
      # Continues with the next file if no comments where found
      if not comments: 
        print '  No comments found'
        continue
      
      docpath = path.replace('./include/mantella_bits/', './doc/api_reference/', 1)
      if not os.path.exists(docpath):
        os.makedirs(docpath)
 
      docfile = open(docpath + '/' + file.split('.')[0] + '.rst', 'w')
      #TODO Add comments to the regex below and add a large comment block describing the structure returned by `re.findall` (this will be done by Sebastian)
      for part in re.findall(re.compile(ur'(.*?)(?=(?:[ ]*\.\. code-block::|$))(([ ]*)\.\. code-block:: ([^ ]+)(?:.*?(?=:name:):name: (\w+)|).*?(?=\n[ ]+[^:])(.*?)(?=(?:\n\3[^ ]|$))|)', re.DOTALL), comments):
        docfile.write(part[0] + part[1])
        
        if part[1]:
          #FIXME part[3] is broken (will return newlines together with the language identifier)
          #TODO Add plots (see the discussion about combining Mantella with a plotting library - maybe in another language)
          if part[3] == 'c++':
            if part[4]:
              example = open('./doc/tmp/' + part[4] + '.cpp', 'w')
            else:
              example = open('./doc/tmp/example.cpp', 'w')
            example.write(part[5])
            example.close()
          
            # Compile generate code file
            output = subprocess.Popen(['c++', '-std=c++14', '-march=native', '-O3', './doc/tmp/' + example.name, '-lblas', '-llapack', '-o', './doc/tmp/example'], stderr = subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              print '  Failure during compilation: ' + output.stderr.read()
              continue
          
            # Execute generate code file  
            output = subprocess.Popen(['./doc/tmp/example'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            output.wait()
            if output.returncode != 0:
              print '  Failure during execution: ' + output.stderr.read()
              continue
          
            # Write result from code file in .rst
            docfile.write('\n' + part[2] + '.. code-block:: none\n')
            for line in output.stdout.read().split('\n'):
              docfile.write('\n' + part[2] + '  ' + line)
      docfile.close()

#TODO Remove the whole `tmp` folder after everything is done (instead of removing single files one by one)