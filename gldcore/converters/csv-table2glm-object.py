"""Convert Anticipation data sheet in CSV to an pole vulnerability GLM template parameter set
"""
# TODO: Strings only should be in ""
import csv

def pole_configuration_converter (p_configuration_in, p_configuration_out, options={} ) : 
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
					p_config_out.write(f"object {options['class']} ")
				p_config_out.write("{ \n")
				for j,value in enumerate (row) : 
					if value : 
						p_config_out.write("\t" + headers[j] + " " + value + ";\n")
				p_config_out.write("}\n")
		p_config_out.close()


