import re
import os
import subprocess

# iterate over all files at include dir './include/mantella_bits/*'
for path, subdirs, files in os.walk("./include/mantella_bits/"):
	for file in files:
		if file.endswith(".hpp"):
	
			# Read all in header file
			headerfile = open(os.path.join(path, file), 'r')
					
			# Pase all comments from header file
			comments = "".join(re.findall(re.compile(ur'[ ]*\/\*\*(.+?)(?=\*\/)', re.DOTALL), headerfile.read()))
			headerfile.close()
			
			# Nothing todo when no comment in file
			if (comments == ""): 
				print "No comments in file: " + file
				continue
			
			# Open .rst file for header file and added headline
			rstfile = open("./doc/" + file.split(".")[0] + ".rst", 'w')
			rstfile.write(file.split(".")[0] + "\n" + "="*len(file.split(".")[0]) + "\n")
			
			# Split comments
			topic = re.findall(re.compile(ur'(.*?)(?=(?:\.\. code-block::|$))([ ]*\.\. code-block:: ((?:\w|\+)+)(?:.*?(?=:name:):name: (\w+)|).*?(?=\n   [^:])\n(.+?)(?=\n[^ \n]+)|)', re.DOTALL), comments)  
			
			for selection in topic:
				rstfile.write(selection[0]) #selection[0] = before code block
				
				if (selection[1] != ""): #selection[1] = code block 
					if (selection[2] == "c++"): #selection[2] = codestyle
					# Create .cpp file in include
						if (selection[3] != ""): #selection[3] = code identifier
							cppfile = open("./doc/examples/" + selection[3]+".cpp",'w')
							cppfile.write(selection[4]) #selection[4] = code
							cppfile.close()
							
			
					# Save code block for compile and execute
					codefile = open("./doc/code.cpp", 'w')
					codefile.write(selection[4])
					codefile.close()
					
					# Compile generate code file
					output = subprocess.Popen(['c++', '-std=c++11', '-march=native', '-O3', './doc/code.cpp', '-larmadillo', '-lmantella', '-o', './doc/executecode'], stderr=subprocess.PIPE)
					output.wait()
					
					if (output.returncode != 0):
						print "Failure during compilation: " + file +"\nWith exeption: \n\n" + output.stderr.read()
						rstfile.close()
						os.system("rm ./doc/code.cpp ./doc/"+ file.split(".")[0] + ".rst")
						exit()
					
					# Execute generate code file	
					output = subprocess.Popen(['./doc/executecode'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
					output.wait()
					if (output.returncode != 0):
						print "Failure during execution: " + file +"\nWith exeption: \n\n" + output.stderr.read()
						rstfile.close()
						os.system("rm ./doc/code.cpp ./doc/executecode ./doc/"+ file.split(".")[0] + ".rst")
						exit()
					
					# Remove temp files
					os.system("rm ./doc/code.cpp ./doc/executecode")
					
					# Write result from code file in .rst
					rstfile.write(selection[1] + "\n.. code-block:: none\n")
					for line in output.stdout.read().split("\n"):
						rstfile.write("\n   " + line)
				
			# Save .rst file		
			rstfile.close()
			print "Successful generate .rst file for: " + file
			exit()