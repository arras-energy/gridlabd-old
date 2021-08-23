"""CYME 'mdb' to GridLAB-D 'glm' converter using OpenFIDO

"""

import sys, os
sys.path.append("/usr/local/bin")
import openfido

def convert(input_file,output_file=None,options={}):
	"""Convert a CYME MDB file to GLM using github/openfido/cyme-extract

	Parameters:
		input_file (str)	input MDB file name
		output_file (str)	output GLM file name
		options (dict)		options to change config and model options
	"""
	# openfido_config import
	try: 
	    import openfido_config as config
	except:
	    # default config
	    class config:
	        """Configuration options
	        """
	        verbose = False # print more messages as work is done
	        quiet = False # print fewer messages as work is done
	        orgname = "openfido" # default repo for workflows and pipelines
	        branch = "main" # default branch to use when downloading workflows and pipelines
	        cache = "/usr/local/share/openfido" # additional path for downloaded modules
	        apiurl = "https://api.github.com"
	        rawurl = "https://raw.githubusercontent.com"
	        giturl = "https://github.com"
	        traceback_file = "/dev/stderr"
	        pass
	for name, value in options.items():
		if hasattr(config, name):
			exec(f"config.{name} = '{value}'")
		else:
			raise Exception(f"'{name}' is not a valid mdb-cyme2glm converter configuration")
			# openfido.error(f"'{name}' is not a valid mdb-cyme2glm converter parameter")
	# openfido.debug(f"input_file = {input_file}")
	# openfido.debug(f"output_file = {output_file}")
	# openfido.debug(f"config = {config}")
	# openfido.debug(f"params = {params}")

	input_path = os.path.realpath(input_file)
	input_folder = os.path.dirname(input_path)
	input_name = os.path.basename(input_path)
	# openfido.debug(f"input_folder = {input_folder}")
	# openfido.debug(f"input_name = {input_name}")

	output_path = os.path.realpath(output_file)
	output_folder = os.path.dirname(output_path)
	output_name = os.path.basename(output_path)

	# openfido.debug(f"output_folder = {output_folder}")
	# openfido.debug(f"output_name = {output_name}")
	print(444444)
	openfido.run(["cyme-extract",f"{input_name}",f"{output_name}",
		f"input_folder={input_folder}",
		f"output_folder={output_folder}",
		f"extract=non-empty",
		f"outputs=glm",
		f"postproc=write_glm.py"],config)













