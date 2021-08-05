"""Convert Anticipation data sheet in CSV to an pole vulnerability GLM template parameter set
"""
import csv
import re
import sys, getopt

def error(msg):
    print(f'ERROR   {msg}')
    sys.exit(1)

def convert (p_configuration_in, p_configuration_out, options={} ) : 
	with open(p_configuration_in, newline='') as csvfile:
		configurations = csv.reader(csvfile, delimiter=',')
		p_config_out = open(p_configuration_out, "a")
		p_config_out.truncate(0)
		p_config_out.write("// Objects \n")		
		for i, row in enumerate(configurations):
			if i == 0 : 
				headers = row
			else : 
				if "class" not in headers and not options: 
					error("No class name found, please edit your CSV to include class or add -C <class name> to your input command")
				else : 
					class_index=headers.index("class")
				if "." in class_name:
					class_spec = class_name.split(".")
					p_config_out.write(f"module {class_spec[0]};\n")
				if not row[class_index] : 
					p_config_out.write(f"object {options['class']} ")
					class_name = options['class']
				if row[class_index] : 
					p_config_out.write(f"object {row[class_index]} ")
					class_name = row[class_index]
				p_config_out.write("{ \n")
				for j,value in enumerate (row) : 
					if j!=class_index and headers[j] : 
						if not value and headers[j].strip()=="name" : 
							p_config_out.write("\t" + headers[j].strip() + " " + class_name +"_" + str(i) + ";\n")
						else : 
							if value : 
								if re.findall('^\d+',value) or value.startswith('(') or '([0-9]*\ [*a-zA-Z+]*){0,1}?' in value and ',' not in value: 
									p_config_out.write("\t" + headers[j].strip() + " " + value + ";\n")
								else : 
									p_config_out.write("\t" + headers[j].strip() + " " + "\"" +value + "\"" + ";\n")
				p_config_out.write("}\n")
		p_config_out.close()
