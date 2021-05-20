"""Convert Anticipation data sheet in CSV to an pole vulnerability GLM template parameter set
"""
import csv
import re

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
				if not row[0] : 
					print(options)
					p_config_out.write(f"object {options['class']} ")
					class_name = options['class']
				if row[0] : 
					p_config_out.write(f"object {row[0]} ")
					class_name = row[0]
				p_config_out.write("{ \n")
				for j,value in enumerate (row) : 
					if j>0 : 
						if not value and headers[j].strip()=="name" : 
							p_config_out.write("\t" + headers[j].strip() + " " + class_name +"_" + str(i) + ";\n")
						else : 
							if value : 
								if re.findall('^\d+',value) or value.startswith('(') or ' ' in value: 
									p_config_out.write("\t" + headers[j].strip() + " " + value + ";\n")
								else : 
									p_config_out.write("\t" + headers[j].strip() + " " + "\"" +value + "\"" + ";\n")
				p_config_out.write("}\n")
		p_config_out.close()