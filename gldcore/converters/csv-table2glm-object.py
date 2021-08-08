"""Convert generable CSV table into a list of GLM objects

Valid options:

  class:   specifies the default class to use when class is not specified in the table
"""
import csv
import re
import sys, getopt

def error(msg):
    print(f'ERROR [converters/csv-table2glm-object]: {msg}',file=sys.stderr)
    sys.exit(1)

def warning(msg):
    print(f'WARNING [converters/csv-table2glm-object]: {msg}',file=sys.stderr)

def convert (p_configuration_in, p_configuration_out, options={} ) : 
	if "module" in options.keys():
		modulename = options["module"]
		del options["module"]
	else:
		modulename = None
	if "class" in options.keys():
		classname = options["class"]
		del options["class"]
	else:
		classname = None
	with open(p_configuration_in, newline='') as csvfile:
		configurations = csv.reader(csvfile, delimiter=',')
		p_config_out = open(p_configuration_out, "w")
		if modulename:
			for module in modulename.split(","):
				p_config_out.write(f"module {module};\n")
		for i, row in enumerate(configurations):
			if i == 0 : 
				headers = row
				if "class" in headers and classname:
					warning("class data overrides command line class '{classname}'")
					classname = None
			else : 
				if not classname:
					if not "class" in headers:
						error("missing required class specification in either data ('class' field) or command line (-C <class>)")
					else : 
						class_index = headers.index("class")
						classname = row[class_index]
				else:
					class_index = None

				p_config_out.write(f"object {classname} ")
				p_config_out.write("{\n")
				for j,value in enumerate (row) : 
					if j!=class_index and headers[j] : 
						if not value and headers[j].strip()=="name" : 
							p_config_out.write(f"\t{headers[j].strip()} {classname}:{i};\n")
						else : 
							if value : 
								if re.findall('^\d+',value) or value.startswith('(') or '([0-9]*\ [*a-zA-Z+]*){0,1}?' in value and ',' not in value: 
									p_config_out.write("\t" + headers[j].strip() + " " + value + ";\n")
								else : 
									p_config_out.write("\t" + headers[j].strip() + " " + "\"" +value + "\"" + ";\n")
				for prop,value in options.items():
					p_config_out.write(f"\t{prop} \"{value}\";\n") 
				p_config_out.write("}\n")
		p_config_out.close()
