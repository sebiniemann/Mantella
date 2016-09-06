import re
import os

path_to_include = "./include/mantella_bits/"
path_to_rst_files = "./doc/"
path_to_doc_includes = "./doc/doc_includes/examples/"

# iterate over all files at include dir 
for file in os.listdir(path_to_include):
	if file.endswith(".hpp"):
		# Read all in header file
		headerfile = open(path_to_include + file, 'r')
		content = headerfile.read()
		headerfile.close()
		
		rst = ""
		# Pase all comments from content
		comments = "".join(re.findall(re.compile(ur'[ ]*\/\*\*(.+?)(?=\*\/)', re.DOTALL), content))
		topic = re.findall(re.compile(ur'(.*?)(?=(?:\.\. code-block::|$))([ ]*\.\. code-block:: ((?:\w|\+)+)(?:.*?(?=:name:):name: (\w+)|).*?(?=\n   [^:])\n(.+?)(?=\n[^ \n]+)|)', re.DOTALL), comments)
		for test in topic:
			rst = rst + test[0]
			if (test[1] != ""):	
				if (test[2] == "c++"):
				# Create .cpp file in include
					if (test[4] != ""):
						cppfile = open(path_to_doc_includes+test[3]+".cpp",'w')
						cppfile.write(test[4])
			
				# Compile and get result
				#TODO
				output = "\n.. code-block:: none\n" + "   the result is: 42\n\n"
		
				rst = rst+test[1]+output
		rstfile = open(path_to_rst_files + file.split(".")[0] + ".rst", 'w')
		rstfile.write(rst)
		rstfile.close()