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
	with open(p_configuration_in, "r", newline='') as csvfile:
		configurations = csv.reader(csvfile, delimiter=',')
		p_config_out = open(p_configuration_out, "a")
		p_config_out.truncate(0)
		p_config_out.write("// Objects \n")
		module_list = []		
		for i, row in enumerate(configurations):
			if i == 0 : 
				headers = row
				if "class" in headers:
					if classname:
						warning(f"class data overrides option 'class={classname}'")
					class_index = headers.index("class")
				else:
					class_index = None
			else : 
				if "class" not in headers and not options: 
					error("No class name found, please edit your CSV to include class or add -C <class name> to your input command")
				else : 
					class_index=headers.index("class")
				if not row[class_index] : 
					class_name = options['class']
				if row[class_index] : 
					class_name = row[class_index]
				if "." in class_name :
					class_spec = class_name.split(".")[0]
					if class_spec not in module_list: 
						p_config_out.write(f"module {class_spec};\n")
						module_list.append(class_spec)
				p_config_out.write(f"object {class_name} ")
				p_config_out.write("{ \n")
				for j,value in enumerate (row) : 
					if j!=class_index and headers[j] : 
						if not value and headers[j].strip()=="name" : 
							p_config_out.write(f"\t{headers[j].strip()} {oclass}:{i};\n")
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
