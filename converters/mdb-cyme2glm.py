"""CYME 'mdb' to GridLAB-D 'glm' converter using OpenFIDO

"""

import sys, os
sys.path.append("/usr/local/bin")
import openfido

def convert(input_file,output_file=None,options={}):
	"""Convert a CYME MDB file to GLM using github/openfido/cyme-extract

Parameters:

- input_file (str)    input MDB file name (required)

- output_file (str)   output GLM file name (default None)

- options (dict)      options to change config and model options (default {})

Options:

- `FILES=<pattern>`   restricts the names of the database to extract (default *.mdb)

- `TABLES=<pattern>`  extract only the listed tables (default *)

- `POSTPROC={write_glm.py,network_graph.py,voltage_profile.py}`: 
                      run postprocessing routines (default none)

- `EXTRACT={all,non-empty}`:
                      extracts all or only non-empty tables (default all)

- `TIMEZONE=<country>/<city>`:
                      changes localtime to use specified timezone (default UTC)

- `OUTPUTS=<extension>[,<extension>[,...]]`:
                      extensions to save (default "zip", "csv", "png", "glm", "json")

Post-processor options:

	network_graph.py: Draw network graph

	- PNG_POSTPROC   must be set to "network_graph.py"

	- PNG_FIGNAME    name of figure (default "network_graph.png")

	- PNG_FIGSIZE    PNG image dimensions (default "9x6")

	- PNG_NODESIZE   size of nodes (default "10")

	- PNG_NODECOLOR  color nodes (default "byphase")

	- PNG_FONTSIZE   size of label font (default "8")

	- PNG_ROOTNODE   root node (required for multipartite and shell graphs)

	- PNG_LAYOUT     graph layout (default "nodexy")

		Supported layouts:

		- nodexy         use XY coordinates in CYME node table

		- circular       nodes on a circle

		- kamada_kawai   use path length minimization

		- planar         avoid edge intersections

		- random         uniformly in a unit square

		- shell          concentric circles

		- spring         force-directed graph

		- spectral       eigenvector of graph Laplacian

		- spiral         spiral layout

		- multipartite   layers by distance from root node

	write_glm.py

	- GLM_NOMINAL_VOLTAGE=<voltage>		Nominal voltage (default None)

	- GLM_NETWORK_PREFIX=<string>		Network name prefix (default None)

	- GLM_INCLUDE=<glm-name>			,config.glm

	- GLM_MODIFY,modify.csv

	- GLM_DEFINE,SOLUTIONDUMP=yes

	- GLM_ASSUMPTIONS,include

	voltage_profile.py

	- VP_LIMIT=<percent>        limit voltage range to percent specified

	- VP_SIZE=<width>x<height>  image size in inches

	- VP_DPI=<resolution>       image resolution in pixels per inch

	- VP_NODES={True,False}     label branching nodes
"""

	# print("Running",__name__,"...",file=sys.stderr,flush=True)
	# print(input_file,file=sys.stderr,flush=True)
	# print(output_file,file=sys.stderr,flush=True)
	# print(options,file=sys.stderr,flush=True)

	for name, value in options.items():
		if name in openfido.config.keys():
			openfido.config[name] = value
		# elif name in openfido.params.keys():
		# 	openfido.params[name] = value
		else:
			# raise Exception(f"'{name}' is not a valid mdb-cyme2glm converter configuration")
			openfido.error(f"'{name}' is not a valid mdb-cyme2glm converter configuration")
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
	try:
		openfido.run(["cyme-extract",f"{input_name}",f"{output_name}",
			f"input_folder={input_folder}",
			f"output_folder={output_folder}",
			f"extract=non-empty",
			f"outputs=glm",
			f"postproc=write_glm.py"])
	except:
		import traceback
		print(f"ERROR [mdb-cyme2glm]: {traceback.print_exc()}")
		sys.exit(10)












