import re
import os
import glob

# iterate over all files at include dir './include/mantella_bits/*'
for path, subdirs, files in os.walk("./include/mantella_bits/"):
	for file in files:
		if file.endswith(".hpp"):
	
			# Read all in header file
			headerfile = open(os.path.join(path, file), 'r')
			content = headerfile.read()
			headerfile.close()
		
		
			# Pase all comments from content
			comments = "".join(re.findall(re.compile(ur'[ ]*\/\*\*(.+?)(?=\*\/)', re.DOTALL), content))
			if (comments == ""):
				print "No comments in file: " + file
				continue # Nothing todo when no comment in file
		
			rst = "" # String for .rst file
			topic = re.findall(re.compile(ur'(.*?)(?=(?:\.\. code-block::|$))([ ]*\.\. code-block:: ((?:\w|\+)+)(?:.*?(?=:name:):name: (\w+)|).*?(?=\n   [^:])\n(.+?)(?=\n[^ \n]+)|)', re.DOTALL), comments)  
		
			for selection in topic:
				rst = rst + selection[0] #selection[0] = before code block
				if (selection[1] != ""): #selection[1] = code block 
					if (selection[2] == "c++"): #selection[2] = codestyle
					# Create .cpp file in include
						if (selection[3] != ""): #selection[3] = code identifier
							cppfile = open("./doc/include/" + selection[3]+".cpp",'w')
							cppfile.write(selection[4]) #selection[4] = code
			
					# Compile and get result
					#TODO
					output = "\n..   code-block:: none\n" + "   the result is: 42\n\n"
		
					rst = rst+selection[1]+output
				
			# Save .rst file		
			rstfile = open("./doc/" + file.split(".")[0] + ".rst", 'w')
			rstfile.write(rst)
			rstfile.close()
			print "Successful generate .rst file for: " + file