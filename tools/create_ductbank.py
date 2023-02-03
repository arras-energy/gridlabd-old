"""Create ductbank

Syntax:

	% gridlabd create_ductbank -[-input|i]=INPUT.csv -[-output|o]=OUTPUT.glm [--PARAMETER=VALUE ...]

"""

import sys, os
import json

E_OK = 0
E_SYNTAX = 1

sys.argv.append("--name=test")

if len(sys.argv) == 1:
	print("Syntax: gridlabd create_ductbank [-[-input|i]=INPUT.csv]] [-[-output|o]=OUTPUT.glm] [PARAMETER=VALUE ...]",file=sys.stderr)
	exit(E_SYNTAX)

PARAMETERS = dict(
	name = None,
	surface_temperature = 25.0, # degC
	warning_cable_temperature = 80.0, # degC
	alert_cable_temperature = 90.0, # degC"
	duct_ROH = 60.0, # W.cm/K
	insulation_ROH = 120.0, # W.cm/K
	rows = 3,
	columns = 2,
	channel_diameter = 5, # in
	horizontal_spacing = 1.5, # in
	vertical_spacing = 0.5, # in
	side_thickness = 1.5, # in
	top_thickness = 1.5, # in
	bottom_thickness = 1.5, # in
	)

INPUT = sys.stdin
OUTPUT = sys.stdout

def error(msg,code=None):
	print(f"ERROR [create_ductbank]: {msg} (code %d)",file=sys.stderr)
	if type(code) is int:
		exit(code)

for arg in sys.argv[1:]:

	if "=" in arg:

		tag,value = arg.split("=")

	else:

		tag = arg
		value = True

	if tag in ["-h", "--help", "help"]:

		print(__doc__,file=OUTPUT)

	elif tag in ["-i", "--input"]:

		INPUT = open(value,"r")

	elif tag in ["-o","--output"]:

		OUTPUT = open(value,"w")

	elif tag[0:2] == "--" and tag[2:] in PARAMETERS:

		PARAMETERS[tag[2:]] = value

	else:

		error(f"'{arg}' is invalid",E_SYNTAX)

print("object ductbank",file=OUTPUT)
print("{",file=OUTPUT)
for tag,value in PARAMETERS.items():
	if value != None:
		print(f'    {tag} "{value}";',file=OUTPUT)
print("}",file=OUTPUT)
