"""Convert generable CSV table into a list of GLM objects
Valid options:
  class:   specifies the default class to use when class is not specified in the table
  module:  specifies the modules to be loaded before defining objects
"""
import csv
import re
import sys, getopt

def error(msg):
    print(f'ERROR [csv-table2glm-object]: {msg}',file=sys.stderr)
    sys.exit(1)

def warning(msg):
    print(f'WARNING [csv-table2glm-object]: {msg}',file=sys.stderr)

def convert (input_name, output_name=None, options={} ) : 
	if not output_name:
		if input_name.endswith(".csv"):
			output_name = input_name.replace(".csv",".glm")
		else:
			output_name = input_name + ".glm"
	with open(input_name, "r", newline='') as csvfile:
		configurations = csv.reader(csvfile, delimiter=',')
		glm = open(output_name, "w")
		glm.write(f"// {__file__} {input_name} {output_name} {options} \n")
		if "module" in options.keys():
			for module in options["module"].split(","):
				glm.write(f"module {module};\n")
		for i, row in enumerate(configurations):
			if i == 0 : 
				headers = row
				if "class" in headers:
					class_index = headers.index("class")
				elif "class" not in options.keys():
					error("no class specified either in CSV or command options")
				else:
					class_index = None
			else : 
				if class_index == None or not row[class_index] : 
					class_name = options['class']
				else:
					class_name = row[class_index]
				glm.write(f"object {class_name} ")
				glm.write("{ \n")
				for j,value in enumerate (row) : 
					if j!=class_index and headers[j] : 
						if not value and headers[j].strip()=="name" : 
							glm.write("\t" + headers[j].strip() + " " + class_name +"_" + str(i) + ";\n")
						else : 
							if value : 
								if re.findall('^\d+',value) or value.startswith('(') or '([0-9]*\ [*a-zA-Z+]*){0,1}?' in value and ',' not in value: 
									glm.write("\t" + headers[j].strip() + " " + value + ";\n")
								else : 
									glm.write("\t" + headers[j].strip() + " " + "\"" +value + "\"" + ";\n")
					for key,value in options.items():
						if not key in ["module","class"]:
							glm.write(f"\t{key} \"{value}\";\n")
				glm.write("}\n")
		glm.close()