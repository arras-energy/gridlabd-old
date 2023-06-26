# Syntax: create_ductbank [-i|--input]=INPUT.csv [-o|--output]=OUTPUT.glm [--PARAMETER=VALUE ...]
"""Create ductbank

Syntax:

	% gridlabd create_ductbank -[-input|i]=INPUT.csv -[-output|o]=OUTPUT.glm [--PARAMETER=VALUE ...]

"""

import sys, os
import json
from numpy import *

E_OK = 0
E_SYNTAX = 1

sys.argv.append("--name=test")

if len(sys.argv) == 1:
	print("Syntax: gridlabd create_ductbank [-i|--input]=INPUT.csv [-o|--output]=OUTPUT.glm [--PARAMETER=VALUE ...]",file=sys.stderr)
	exit(E_SYNTAX)

INPUT = sys.stdin
OUTPUT = sys.stdout

def error(msg,code=None):
	print(f"ERROR [create_ductbank]: {msg} (code %d)",file=sys.stderr)
	if type(code) is int:
		exit(code)

class Ductbank:

	name = None
	surface_temperature = 25.0 # degC
	warning_cable_temperature = 80.0 # degC
	alert_cable_temperature = 90.0 # degC"
	duct_ROH = 60.0 # W.cm/K
	insulation_ROH = 120.0 # W.cm/K
	insulation_thickness = 1.0 # cm
	air_ROH = 20.0 # W.cm/K
	rows = 3
	columns = 2
	channel_diameter = 5 # in
	horizontal_spacing = 1.5 # in
	vertical_spacing = 0.5 # in
	side_thickness = 1.5 # in
	top_thickness = 1.5 # in
	bottom_thickness = 1.5 # in
	cable_diameter = 1 # in
	channel_loading = [[3,0],[0,3],[3,0]] # cable count in each duct
	channel_heating = [[1,0],[0,1],[1,0]] # W/m 

	def A(self,i,j):
		d = 0.0
		# vertical spacing
		if i == 0:
			d += self.top_thickness
		elif i < self.rows-1:
			d += self.vertical_spacing
		if i == self.rows-1:
			d += self.top_thickness
		elif i < self.rows-1:
			d += self.vertical_spacing
		# horizontal spacing
		if j == 0:
			d += self.side_thickness
		elif i < self.columns-1:
			d += self.horizontal_spacing
		if j == self.columns-1:
			d += self.side_thickness
		elif i < self.columns-1:
			d += self.horizontal_spacing
		Ud = d*self.duct_ROH/100 
		Ui = self.insulation_ROH*self.insulation_thickness
		A = (self.channel_diameter/2)**2 * pi
		C = (self.cable_diameter/2)**2 * pi * self.channel_loading[i][j]
		if C > A:
			raise f"channel {i},{j} is overloaded"
		Ua = sqrt(A-C)/2.54 * self.air_ROH
		return 1/(1/Ud+1/Ui+1/Ua)

	def B(self,i,k):
		if k == 0: # temperature term
			R = 0
			for j in range(self.columns):
				if i > 0: # not top row
					if j == 0 or j == self.columns-1: # side (not sure whether 1 or both)
						d = 0.0
						if i == 0: # left side
							d += self.side_thickness
						if i == self.columns - 1: # right side
							d += self.side_thickness
						if d > 0:
							R += 100/(d*self.duct_ROH)
				else: # top row
					if j > 0 or j <self.columns-1: # not side duct
						R += 100/(self.top_thickness*self.duct_ROH)
					else: # side (not sure whether 1 or both)
						d = 0.0
						if i == 0: # left side
							d += self.side_thickness
						if i == self.columns - 1: # right side
							d += self.side_thickness
						if d > 0 :
							R +=  100/(d*self.duct_ROH/100 + self.top_thickness*self.duct_ROH)
			return 1/R if R > 0 else 0.0
		else: # heat gain term
			return 1 if i+1 == k else 0			

	def u(self,i):
		if i == 0: # ground coupling
			return self.surface_temperature
		else: # heat input from cables in duct
			return 0.0

ductbank = Ductbank()

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

	elif tag[0:2] == "--" and tag[2:] in dir(ductbank):

		setattr(ductbank,tag[2:],value)

	else:

		error(f"'{arg}' is invalid",E_SYNTAX)

print("object ductbank",file=OUTPUT)
print("{",file=OUTPUT)
for tag in dir(ductbank):
	if not tag.startswith("_"):
		value = getattr(ductbank,tag)
		if type(value) in [str,float,int,list] and value != None:
			print(f'    {tag} "{value}";',file=OUTPUT)
print("}",file=OUTPUT)


print("A =")
K = ductbank.rows*ductbank.columns
for i in range(ductbank.rows):
	for j in range(ductbank.columns):
		print(f"{ductbank.A(i,j):6.2g}",end=" ")
	print("",flush=True)

print("B = ")
for i in range(K+1):
	for j in range(K):
		print(f"{ductbank.B(i,j):6.2g}",end=" ")
	print("",flush=True)

print("u = ")
for i in range(K+1):
	print(f"{ductbank.u(i):6.2g}")
print("",flush=True)
