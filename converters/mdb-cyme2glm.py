"""CYME 'mdb' to GridLAB-D 'glm' converter

Options:

- `config=CSV`: specify the name of CSV for converter configuration

- `cyme-tables`: print out the required CYME tables

- `data-dir=DIR`: specify the directory for storing CSV tables (default /tmp/gridlabd/mdb-cyme2glm/INPUT_FILE_NAME)

- `equipment_mdb=MDB`: specify the MDB for equipment data (default None)

- `network-ID=ID`: specify the network ID list to be converted (default [])

- `coordinateCSV=CSV`: specify the name of CSV for saving the node geographic location data (default None)

"""
app_version = 0

import sys, os, time
import getopt
import subprocess
import glob
import datetime as dt
from typing import Any
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import math
from math import sqrt, cos, sin, pi
import re
import hashlib
import csv
import pprint
pp = pprint.PrettyPrinter(indent=4,compact=True)
import traceback
from copy import copy
import numpy as np

#
# Application information
#
app_command = os.path.abspath(sys.argv[0])
app_workdir = os.getenv("PWD")
app_path = "/"+"/".join(app_command.split("/")[0:-1])

# #
# # Git information
# #
# # TODO: change this to use gitpython module
# #
# def command(cmd,lang="utf-8"):
# 	return subprocess.run(cmd.split(),stdout=subprocess.PIPE).stdout.decode(lang).strip()
# os.chdir(app_path)
# git_project = command("git config --local remote.origin.url")
# git_commit = command("git rev-parse HEAD")
# git_branch = command("git rev-parse --abbrev-ref HEAD")
# os.chdir(app_workdir)

#
# Load user configuration
#

config = pd.DataFrame({
	"GLM_NETWORK_PREFIX" : [""],
	"GLM_NETWORK_MATCHES" : [".*"],
	"GLM_NOMINAL_VOLTAGE" : [""],
	"GLM_INCLUDE" : [""],
	"GLM_DEFINE" : [""],
	"GLM_MODIFY" : [""],
	"GLM_ASSUMPTIONS" : ["include"],
	"GLM_VOLTAGE_FIX" : ["True"],
	"GLM_PHASE_FIX" : ["True"],
	"GLM_DISTRIBUTED_LOAD_CONFIG" : ["to"],
	"GLM_OUTPUT" : "/dev/stdout",
	"EXTRACT" : ["non-empty"],
	"ERROR_OUTPUT" : "/dev/stderr",
	"WARNING_OUTPUT" : "/dev/stderr",
	"WARNING" : ["True"], 
	"DEBUG" : ["False"],
	"QUIET" : ["False"],
	"VERBOSE" : ["False"],
	}).transpose().set_axis(["value"],axis=1)
config.index.name = "name" 

cyme_phase_name = {0:"ABCN", 1:"A", 2:"B", 3:"C", 4:"AB", 5:"AC", 6:"BC", 7:"ABC"} # CYME phase number -> phase names
glm_phase_code = {"A":1, "B":2, "C":4, "AB":3, "AC":5, "BC":6, "ABC":7} # GLM phase name -> phase number
glm_phase_name = {0:"ABCN", 1:"A",2:"B",3:"AB",4:"C",5:"AC",6:"BC",7:"ABC"} # GLM phase number -> phase name
cyme_phase_name_delta = {1:"AB", 2:"BC", 3:"AC", 7:"ABC"} # CYME phase number -> phase names for delta connection

#
# Device type mapping
#
cyme_devices = {
	1 : "UndergroundLine",
	2 : "OverheadLine",
	3 : "OverheadByPhase",
	4 : "Regulator",
	5 : "Transformer",
	6 : "Not used",
	7 : "Not used",
	8 : "Breaker",
	9 : "LVCB",
	10 : "Recloser",
	11 : "Not used",
	12 : "Sectionalizer",
	13 : "Switch",
	14 : "Fuse",
	15 : "SeriesCapacitor",
	16 : "SeriesReactor",
	17 : "ShuntCapacitor",
	18 : "ShuntReactor",
	19 : "Not used",
	20 : "SpotLoad",
	21 : "DistributedLoad",
	22 : "Miscellaneous",
	23 : "OverheadLineUnbalanced",
	24 : "ArcFurnace",
	25 : "CTypeFilter",
	26 : "DoubleTunedFilter",
	27 : "HighPassFilter",
	28 : "IdealConverter",
	29 : "NonIdealConverter",
	30 : "ShuntFrequencySource",
	31 : "Not used",
	32 : "SingleTunedFilter",
	33 : "InductionMotor",
	34 : "SynchronousMotor",
	35 : "InductionGenerator",
	36 : "SynchronousGenerator",
	37 : "ElectronicConverterGenerator",
	38 : "TransformerByPhase",
	39 : "ThreeWindingTransformer",
	40 : "NetworkEquivalent",
	41 : "Wecs",
	42 : "GroundingTransformer",
	43 : "MicroTurbine",
	44 : "Sofc",
	45 : "Photovoltaic",
	46 : "SeriesFrequencySource",
	47 : "AutoTransformer",
	48 : "ThreeWindingAutoTransformer",
	80 : "battery"
}
glm_devices = {
	1 : "underground_line",
	2 : "overhead_line",
	3 : "overhead_line",
	4 : "regulator",
	5 : "transformer",
	8 : "breaker",
	10 : "recloser",
	# 12 : "sectionalizer",
	13 : "switch",
	14 : "fuse",
	17 : "capacitor",
	20 : "load",
	21 : "load",
	23 : "overhead_line",
	38 : "single_transformer",
	45 : "Photovoltaic",
}

#
# CYME database access tools
#

# find records in a table (exact field match only)
def table_find(table,**kwargs):
	result = table
	for key,value in kwargs.items():
		result = result[result[key]==value]
	return result

# get the value in a table using a certain id or index
def table_get(table,id,column=None,id_column=None):
	if id_column == None or id_column == '*':
		if column == None or column == "*":
			return table.loc[id]
		else:
			return table.loc[id][column]
	else:
		for index, row in table.iterrows():
			if row[id_column] == id:
				if column == None or column == "*":
					return table.loc[index]
				else:
					return table.loc[index][column]
	return None

def load_cals(load_type,load_phase,connection,load_power1,load_power2,load_voltage,value_type=None):
	phase_number=int(load_phase)
	# load_voltage in kV
	if connection == 2: 
		# delta connection
		vol_real = float(load_voltage)*cos((1-phase_number)*pi*2.0/3.0+pi/6.0)*1000.0
		vol_imag = float(load_voltage)*sin((1-phase_number)*pi*2.0/3.0+pi/6.0)*1000.0
		line_phase_gain = sqrt(3.0) 
		if len(cyme_phase_name_delta[phase_number].replace('N','')) == 2:
			load_scale = 1
		elif len(cyme_phase_name_delta[phase_number].replace('N','')) == 3:
			load_scale = 3
		else:
			error(f'wrong load phase {load_phase} for delta connection', 10)
	else:
		# wye connecttion
		vol_real = float(load_voltage)*cos((1-phase_number)*pi*2.0/3.0)*1000.0
		vol_imag = float(load_voltage)*sin((1-phase_number)*pi*2.0/3.0)*1000.0
		line_phase_gain = 1
		load_scale = len(cyme_phase_name[phase_number].replace('N',''))
		if load_scale < 0 or load_scale > 3:
			error(f'wrong load phase {load_phase} for wye connection', 11)
	if value_type == 0:
		load_real = load_power1 * 1000.0
		load_imag = load_power2 * 1000.0
	elif value_type == 1:
		if load_power2 > 0:
			load_real = load_power1 * load_power2/100 * 1000.0
			load_imag = load_power1 * sqrt(1 - (load_power2/100)**2) * 1000.0
		else:
			load_real = -load_power1 * load_power2/100 * 1000.0
			load_imag = -load_power1 * sqrt(1 - (load_power2/100)**2) * 1000.0
	else:
			load_real = load_power1 * 1000
			if load_power2 > 0.0 or load_power2 < 0.0:
				load_imag = load_real/(load_power2/100.0)*sqrt(1-abs(load_power2/100)**2)
	vol_mag = float(load_voltage)*1000.0
	vol_complex = vol_real+vol_imag*(1j)
	if load_type == "Z":
		if (load_real*load_real + load_imag*load_imag) > 0:
			load_cals_results = vol_mag*line_phase_gain*vol_mag*line_phase_gain/(load_real+load_imag*(1j))/load_scale
			return load_cals_results
		else:
			return 0+0*(1j)
	elif load_type == "I":
		load_cals_results  = (load_real+load_imag*(1j))/(vol_complex*line_phase_gain)/load_scale
		return load_cals_results	
	else:
		# for constant power load, the imag part is negative
		load_cals_results = (load_real-load_imag*(1j))/load_scale
		return load_cals_results

def capacitor_phase_cals(KVARA,KVARB,KVARC):
	return int(KVARA > 0) + 2*int(KVARB > 0) + 3*int(KVARC > 0) + int((KVARA*KVARB > 0) or (KVARA*KVARC > 0) or (KVARB*KVARC > 0))

# Function that replaces characters not allowed in name with '_'
def fix_name(name):
	name = name.replace(' ', '_')
	name = name.replace('.','_')
	name = name.replace('-','_')
	name = name.replace('\\','_')
	name = name.replace('/','_')
	name = name.replace(':','_')
	name = name.replace('\'','_')
	name = name.replace('~','_')
	return name

def arrangeString(string):
	MAX_CHAR = 26
	char_count = [0] * MAX_CHAR
	s = 0

	for i in range(len(string)):
		if string[i] >= "A" and string[i] <= "Z":
			char_count[ord(string[i]) - ord("A")] += 1
		else:
			s += ord(string[i]) - ord("0")
	res = ""

	for i in range(MAX_CHAR):
		ch = chr(ord("A") + i)
		while char_count[i]:
			res += ch
			char_count[i] -= 1
	if s > 0:
		res += str(s)

	return res

def clean_phases(phases):
	p = ''
	if 'A' in phases:
		p = p + 'A'
	if 'B' in phases:
		p = p + 'B'
	if 'C' in phases:
		p = p + 'C'
	return p

# map CYME phases to GLM phases
cyme_phase_map = {
		1:'A',
		2:'B',
		3:'C',
		4:'AB',
		5:'AC',
		6:'BC',
		7:'ABC',
		}

# global variable tracking
data_folder = None
input_folder = None
output_folder = None
network_select = None
equipment_file = None
single_file = None
config_file = None
modify_file = None
settings = None
cyme_table = {}
cyme_equipment_table = {}
default_load_voltage = None
network_id = None
cyme_mdbname = None
geodata_file = None
WARNING = True
DEBUG = False
QUIET = False
VERBOSE = False
voltage_check_fix = False
phase_check_fix = False
error_count = 0
warning_count = 0
GLM_error_file = sys.stderr
GLM_warning_file = sys.stderr
GLM_output_file = sys.stdout
node_geodata = {}

# table for solar: "CYMDGGENERATIONMODEL", "CYMCONVERTER"
cyme_tables_required = [
	"CYMNETWORK","CYMHEADNODE","CYMNODE","CYMSECTION","CYMSECTIONDEVICE",
	"CYMOVERHEADBYPHASE","CYMOVERHEADLINEUNBALANCED","CYMEQCONDUCTOR",
	"CYMEQGEOMETRICALARRANGEMENT","CYMEQOVERHEADLINEUNBALANCED","CYMEQFUSE",
	"CYMSWITCH","CYMCUSTOMERLOAD","CYMLOAD","CYMSHUNTCAPACITOR","CYMFUSE",
	"CYMTRANSFORMER","CYMEQTRANSFORMER","CYMREGULATOR","CYMEQREGULATOR",
	"CYMOVERHEADLINE","CYMUNDERGROUNDLINE","CYMNODETAG","CYMEQCABLE",
	"CYMBREAKER","CYMCAPACITOREXTLTD","CYMCONSUMERCLASS",
	"CYMCTYPEFILTER","CYMTRANSFORMERBYPHASE","CYMRECLOSER","CYMEQOVERHEADLINE",
	"CYMSOURCE","CYMEQSHUNTCAPACITOR","CYMPHOTOVOLTAIC","CYMEQAVERAGEGEOARRANGEMENT","CYMSCHEMAVERSION"]

#
# Warning/error/verbose handling
#

def verbose(msg):
		print(f"VERBOSE [mdb-cyme2glm]: {msg}",flush=True)

def warning(msg):
	global warning_count
	warning_count += 1
	if WARNING:
		print(f"WARNING [mdb-cyme2glm]: {msg}",file=GLM_warning_file,flush=True)
	if VERBOSE:
		verbose(msg)

def error(msg,code=None):
	global error_count
	error_count += 1
	if DEBUG:
		raise Exception(msg)
	if not QUIET:
		print(f"ERROR [mdb-cyme2glm]: {msg}",file=GLM_error_file,flush=True)
	if type(code) is int:
		exit(code)

def debug(msg):
	if DEBUG:
		print(f"DEBUG [mdb-cyme2glm]: {msg}",file=GLM_output_file,flush=True)

def glm_output_print(msg):
	print(f"GLM_OUTPUT [mdb-cyme2glm]: {msg}",file=GLM_output_file,flush=True)
	if VERBOSE:
		verbose(msg)

def format_exception(errmsg,ref=None,data=None):
	tb = str(traceback.format_exc().replace('\n','\n  '))
	dd = str(pp.pformat(data).replace('\n','\n  '))
	return "\n  " + tb + "'" + ref  + "' =\n  "+ dd


def mdb2csv(input_file,output_dir,tables,extract_option):
	if os.path.exists(data_folder):
		os.system(f"rm -rf {data_folder}")
	os.system(f"mkdir -p {data_folder}")
	for table in tables:
		csvname = table[3:].lower()
		os.system(f"mdb-export {input_file} {table} > {output_dir}/{csvname}.csv")
		row_count = os.popen(f"wc -l {output_dir}/{csvname}.csv").read()
		if (int(row_count.strip().split(" ")[0]) <= 1) and extract_option != "all":
			os.remove(f"{output_dir}/{csvname}.csv")
	cyme_table = {}
	for filename in glob.iglob(f"{output_dir}/*.csv"):
		data = pd.read_csv(filename, dtype=str)
		name = os.path.basename(filename)[0:-4].lower()
		cyme_table[name] = data
	return cyme_table

def is_float(element):
	try:
		float(element)
		return True
	except ValueError:
		return False

def fix_unit(string,output_unit):
	if output_unit == "V":
		scale = 1.0
	elif output_unit == "kV":
		scale = 0.001
	else:
		error(f"cannot convert string {string} with unit {output_unit}.", 71)
	if "kV" in string:
		scale = scale  * 1000.0
		value_string = string.replace("kV","")
	elif "V" in string:
		scale = scale  * 1.0
		value_string = string.replace("V","")
	try:
		value = float(value_string) * scale
		return "%.4g" % value
	except:
		error(f"cannot convert string {string}.", 72)

def feeder_voltage_find(network_id,cyme_table):
	## set up feeder nominal voltage
	feeder_kVLN = None
	if os.path.exists(os.path.join(input_folder,'feeder_map_2020.csv')): ## feeder_map_2020.csv should be provided from NG
		df_feeder_master = pd.read_csv(os.path.join(input_folder,'feeder_map_2020.csv'))
		df_feeder_select = df_feeder_master[df_feeder_master['GIS CDF'] == network_id].copy()
	if "source" in cyme_table.keys():
		for index, source in cyme_table["source"].iterrows():
			if source['NetworkId'] == network_id:
				if 'DesiredVoltage' in source.keys() and is_float(source['DesiredVoltage']) and float(source['DesiredVoltage'])>0:
					feeder_kVLN = float(source['DesiredVoltage'])/sqrt(3)
				elif 'EquipmentId' in source.keys() and is_float(source['EquipmentId'].split('_')[-1]) and float(source['EquipmentId'].split('_')[-1])>0:
					feeder_kVLN = float(source['EquipmentId'].split('_')[-1])/sqrt(3)
				elif os.path.exists(os.path.join(input_folder,'feeder_map_2020.csv')) and is_float(df_feeder_select['APS Voltage (kV)']) and float(df_feeder_select['APS Voltage (kV)'])>0:
					feeder_kVLN = float(df_feeder_select['APS Voltage (kV)'].iloc[0])/sqrt(3)
				# elif "OperatingVoltageA" in source.keys() and "OperatingVoltageB" in source.keys() and "OperatingVoltageC" in source.keys():
				# 	feeder_kVLN = float(source['OperatingVoltageA'])/sqrt(3)
				else:
					return feeder_kVLN
				break
		return "%.4g" % feeder_kVLN
	return feeder_kVLN

#
# GLM file builder
#
class GLM:

	prefix = {
		# known powerflow class in gridlabd
		"billdump" : "BD_",
		"capacitor" : "CA_",
		"currdump" : "CD_",
		"emissions" : "EM_",
		"fault_check" : "FC_",
		"frequency_gen" : "FG_",
		"fuse" : "FS_",
		"impedance_dump" : "ID_",
		"inverter" : "IN_",
		"line" : "LN_",
		"line_configuration" : "LC_",
		"line_sensor" : "LS_",
		"line_spacing" : "LG_",
		"link" : "LK_",
		"load" : "LD_",
		"load_tracker" : "LT_",
		"meter" : "ME_",
		"motor" : "MO_",
		"node" : "ND_",
		"overhead_line" : "OL_",
		"overhead_line_conductor" : "OC_",
		"photovoltaic" : "PV_",
		"pole" : "PO_",
		"pole_configuration" : "PC_",
		"power_metrics" : "PM_",
		"powerflow_library" : "PL_",
		"powerflow_object" : "PO_",
		"pqload" : "PQ_",
		"recloser" : "RE_",
		"regulator" : "RG_",
		"regulator_configuration" : "RC_",
		"restoration" : "RS_",
		"sectionalizer" : "SE_",
		"series_reactor" : "SR_",
		"substation" : "SS_",
		"switch" : "SW_",
		"switch_coordinator" : "SC_",
		"transformer" : "TF_",
		"transformer_configuration" : "TC_",
		"triplex_line" : "XL_",
		"triplex_line_conductor" : "XC_",
		"triplex_line_configuration" : "XG_",
		"triplex_load" : "XD_",
		"triplex_meter" : "XM_",
		"triplex_node" : "XN_",
		"underground_line" : "UL_",
		"underground_line_conductor" : "UC_",
		"vfd" : "VF_",
		"volt_var_control" : "VV_",
		"voltdump" : "VD_",
	}

	def __init__(self,file,mode="w"):

		self.filename = file
		self.fh = open(file,mode)
		self.objects = {}
		self.assumptions = []
		self.refcount = {}

	# def __del__(self):
	# 	if self.object:
	# 		self.error("glm object was deleted before objects were output")

	def name(self,name,oclass=None):
		if type(name) is list: # composite name
			# name = "_".join(name).replace(".","").replace(":","")[0:63] # disallow special name characters
			name = "_".join(name)[0:63] # disallow special name characters
		if oclass: # name prefix based on class
			if not oclass in self.prefix.keys(): # name prefix not found
				prefix = f"Z{len(self.prefix.keys())}_"
				self.prefix[oclass] = prefix
				warning(f"{cyme_mdbname}@{network_id}: class '{oclass}' is not a known gridlabd powerflow class, using prefix '{prefix}' for names")
			else:
				prefix = self.prefix[oclass]
			name = prefix + name
		elif "0" <= name[0] <= "9": # fix names that start with digits
			name = "_" + name
		# return name.replace(" ","_").replace("-","_").replace(".","")[0:63] # remove white spaces from names
		return name[0:63] # remove white spaces from names

	def write(self,line):
		print(line,file=self.fh)

	def blank(self):
		self.write("")

	def print(self,message):
		self.write(f"#print {message}")

	def warning(self,message):
		self.write(f"#warning {message}")

	def error(self,message):
		self.write(f"#error {message}")

	def comment(self,*lines):
		for line in lines:
			self.write(f"// {line}")

	def set(self,name,value):
		self.write(f"#set {name}={value}")

	def define(self,name,value):
		self.write(f"#define {name}={value}")

	def include(self,name,brackets="\"\""):
		self.write(f"#include {brackets[0]}{name}{brackets[1]}")

	def module(self, name, parameters = {}):
		if not parameters:
			self.write(f"module {name};")
		else:
			self.write(f"module {name}")
			self.write("{")
			for tag, value in parameters.items():
					if type(value) is str:
						self.write(f"\t{tag} \"{value}\";")
					else:
						self.write(f"\t{tag} {value};")
			self.write("}")

	def clock(self, parameters = {}):
		if not parameters:
			error(f"clock needs parameters", 1)
		else:
			self.write(f"clock")
			self.write("{")
			for tag, value in parameters.items():
					if tag in ["timezone","starttime","stoptime"]:
						self.write(f"\t{tag} \"{value}\";")
					else:
						raise Exception(f"module clock not support parameter {tag}")
			self.write("}")

	def ifdef(self, name, call):
		self.write(f"#ifdef {name}")
		call()
		self.write("#endif")

	def ifndef(self, name, call):
		self.write(f"#ifndef {name}")
		call()
		self.write("#endif")

	def ifexist(self, name, call):
		self.write(f"#ifexist {name}")
		call()
		self.write("#endif")

	def object(self, oclass, name, parameters,overwrite=True):
		if name not in self.objects.keys():
			obj = {"name" : name}
			self.objects[name] = obj
		else:
			obj = self.objects[name]
		if "class" in obj.keys() and obj["class"] == "link" and oclass in ["fuse","underground_line","switch","overhead_line","transformer","single_transformer","regulator"]:
			# if obj is created based on a link object
			if oclass == "single_transformer":
				new_name = self.name(name+f"_{parameters['phases']}", "transformer") # new name
				oclass = "transformer"
			else:
				new_name = self.name(name, oclass) # new name
			new_obj = {"name" : new_name}
			self.objects[new_name] = new_obj
			for key, value in obj.items():
				if key != "name":
					new_obj[key] = value
			for key, value in parameters.items():
				if not overwrite and key in new_obj.keys() and new_obj[key] != value:
					error(f"object property '{key}={new_obj[key]}' merge conflicts with '{key}={value}'", 2)
				if value == None and key in new_obj.keys():
					del new_obj[key]
				else:
					new_obj[key] = value
			new_obj["class"] = oclass
			if "nominal_voltage" in new_obj.keys() and (new_obj["class"] == "underground_line" or new_obj["class"] == "overhead_line"):
				del new_obj["nominal_voltage"]
				if "phases" in new_obj.keys() and "N" not in new_obj["phases"]:
					new_obj["phases"] = new_obj["phases"] + "N"
			if new_name in self.refcount.keys():
				self.refcount[new_name] += 1
			else:
				self.refcount[new_name] = 1
			return new_obj
		else:
			for key, value in parameters.items():
				if not overwrite and key in obj.keys() and obj[key] != value:
					error(f"object property '{key}={obj[key]}' merge conflicts with '{key}={value}'", 3)
				if value == None and key in obj.keys():
					del obj[key]
				else:
					obj[key] = value
			obj["class"] = oclass
			if name in self.refcount.keys():
				self.refcount[name] += 1
			else:
				self.refcount[name] = 1
			return obj


	def delete(self,name):
		if self.refcount[name] == 1:
			del self.objects[name]
		elif self.refcount[name] > 1:
			self.refcount[name] -= 1


	def modify(self,object,property,value,comment=""):
		if comment:
			comment = " // " + str(comment)
		elif not type(comment) is str:
			comment = ""
		if type(value) is str:
			self.write(f"modify {object}.{property} \"{value}\";{comment}")
		else:
			self.write(f"modify {object}.{property} {value};{comment}")

	def assume(self,objname,propname,value,remark=""):
		self.assumptions.append([fix_name(objname),propname,value,remark])

	def close(self):
		
		# objects
		if self.objects:
			for name, parameters in self.objects.items():
				self.write(f"object {parameters['class']}")
				self.write("{")
				for tag, value in parameters.items():
					if tag != "class":
						if type(value) is str:
							self.write(f"\t{tag} \"{value}\";")
						else:
							self.write(f"\t{tag} {value};")
				self.write("}")
			self.objects = {}

		# assumptions
		if self.assumptions:
			if settings["GLM_ASSUMPTIONS"] == "save":
				filename = f"{settings['GLM_NETWORK_PREFIX']}{cyme_mdbname}_{network_id}_assumptions.glm"
				with open(f"{output_folder}/{filename}","w") as fh:
					print("// Assumptions for GLM conversion from database {cyme_mdbname} network {network_id}",file=fh)
					for row in self.assumptions:
						print(f"modify {fix_name(row[0])}.{row[1]} \"{row[2]}\"; // {row[3]}",file=fh)
			elif settings["GLM_ASSUMPTIONS"] == "include":
				self.blank()
				self.comment("","Assumptions","")
				for row in self.assumptions:
					self.modify(row[0],row[1],row[2],row[3])
			elif settings["GLM_ASSUMPTIONS"] == "warn":
				filename = f"{output_folder}/{cyme_mdbname}_{network_id}_assumptions.csv"
				warning(f"{cyme_mdbname}@{network_id}: {len(self.assumptions)} assumptions made, see '{filename}' for details")
				pd.DataFrame(self.assumptions).to_csv(filename,header=["object_name","property_name","value","remark"],index=False)
			elif settings["GLM_ASSUMPTIONS"] != "ignore":
				warning(f"GLM_ASSUMPTIONS={settings['GLM_ASSUMPTIONS']} is not valid (must be one of 'save','ignore','warn','include')")
		
		# modifications
		for modify in settings["GLM_MODIFY"].split():
			self.blank()
			self.comment("",f"Modifications from '{modify}'","")
			try:
				with open(f"{input_folder}/{modify}","r") as fh:
					reader = csv.reader(fh)
					for row in reader:
						if len(row) == 0:
							warning(f"No modifications from {modify}")
						elif 0 < len(row) < 3:
							warning(f"{modify}: row '{','.join(list(row))}' is missing one or more required fields")
						elif len(row) > 3:
							warning(f"{modify}: row '{','.join(list(row))}' has extra fields that will be ignored")
							self.modify(*row[0:3])
						else:
							self.modify(*row)
			except:
				pass

	# general glm model add function
	def add(self,oclass,device_id,data,version,**kwargs):
		try:
			call = getattr(self,"add_"+oclass)
			return call(device_id,data,version=version,**kwargs)
		except Exception as errmsg:
			warning(f"{cyme_mdbname}@{network_id}: unable to add gridlabd class '{oclass}' using CYME device '{device_id}': {errmsg} {format_exception(errmsg,device_id,data.to_dict())}")
			pass 	

	# add a link to glm file
	def add_link(self,section_id,section,version,**kwargs):
		phase = int(section["Phase"])
		from_node_id = section["FromNodeId"]
		to_node_id = section["ToNodeId"]
		device_dict = {}
		for index, device in table_find(cyme_table["sectiondevice"],SectionId=section_id).iterrows():
			device_id = device["DeviceNumber"]
			device_type = int(device["DeviceType"])
			if device_type in glm_devices.keys():
				device_name = self.name(device_id,"link")
				device_dict[device_id] = self.object("link", device_name , {
					"phases" : cyme_phase_name[phase],
					"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
					"from" : self.name(from_node_id,"node"),
					"to" : self.name(to_node_id,"node"),
					})
				if from_node_id not in kwargs["node_links"].keys():
					kwargs["node_links"][from_node_id] = []
				kwargs["node_links"][from_node_id].append(device_id)
				if to_node_id not in kwargs["node_links"].keys():
					kwargs["node_links"][to_node_id] = []
				kwargs["node_links"][to_node_id].append(device_id)
			else:
				warning(f"{cyme_mdbname}@{network_id}: {cyme_devices[device_type]} on section {section_id} has no corresponding GLM object")
		# print(device_dict)
		return device_dict

	# add node to glm file
	def add_node(self,node_id,node_links,device_dict,version,**kwargs):
		phase = 0
		if geodata_file:
			node_geodata_id = f"{node_id}_{network_id}"
			if node_geodata_id not in node_geodata.keys():
				all_node = kwargs["node_info"]["all_node"]
				try:
					node_X = float(all_node[all_node["NodeId"] == node_id]["X"].values)
					node_Y = float(all_node[all_node["NodeId"] == node_id]["Y"].values)
				except:
					warning(f'{cyme_mdbname}@{network_id}: cannot add coordinates for node_id')
					node_X = np.nan
					node_Y = np.nan
				node_geodata[node_geodata_id] = {
					"NotworkID" : network_id,
					"node" : node_id,
					"x" : node_X,
					"y" : node_Y,
				}
			else:
				raise Exception(f"{cyme_mdbname}@{network_id}: multiple definition for {node_id}")
		for device_id in node_links[node_id]:
			phase |= glm_phase_code[device_dict[device_id]["phases"]]
		obj = self.object("node", self.name(node_id,"node"), {
			"phases" : glm_phase_name[phase]+"N",
			"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
			})
		if node_id == table_get(cyme_table["headnode"],network_id,"NodeId","NetworkId"):
			obj["bustype"] = "SWING"
		else:
			obj["bustype"] = "PQ"
		return obj

	# add an overhead line based on a link
	def add_overhead_line(self,line_id,line,version):
		line_name = self.name(line_id,"link")
		length = float(line["Length"])
		if length == 0.0:
			length = 0.01
		line_conductor_id = line["LineId"]
		line_conductor = None
		if 'eqconductor' in cyme_equipment_table.keys():
			line_conductor = table_get(cyme_equipment_table['eqoverheadline'],line_conductor_id,None,'EquipmentId')
		# elif 'csvundergroundcable' in cyme_equipment_table.keys():
		# 	## TODO
		elif 'eqconductor' in cyme_table.keys():
			line_conductor = table_get(cyme_table['eqoverheadline'],line_conductor_id,None,'EquipmentId')
		if line_conductor is None:
			warning(f'{cyme_mdbname}@{network_id}: OH cable conductor "{line_conductor_id}" of line "{line_id}" is missing in CYME model.Use default settings.')
			line_conductor = {
				"PhaseConductorId" : "DEFAULT",
				"NeutralConductorId" : "DEFAULT",
				"ConductorSpacingId" : "DEFAULT",
			}
		conductorABC_id = line_conductor["PhaseConductorId"]
		conductorN_id = line_conductor["NeutralConductorId"]
		self.add_overhead_line_conductors([conductorABC_id,conductorN_id],version)
		spacing_id = line_conductor["ConductorSpacingId"]
		self.add_line_spacing(spacing_id,version)
		configuration_name = self.add_line_configuration([conductorABC_id,conductorABC_id,conductorABC_id,conductorN_id,spacing_id],version)
		return self.object("overhead_line", line_name, {
			"length" : "%.2f m"%length,
			"configuration" : configuration_name,
			})

	# add an overhead line by phase based on a link
	def add_overhead_line_phase(self,line_id,line,version):
		line_name = self.name(line_id,"link")
		length = float(line["Length"])
		if length == 0.0:
			length = 0.01
		conductorA_id = line["PhaseConductorIdA"]
		conductorB_id = line["PhaseConductorIdB"]
		conductorC_id = line["PhaseConductorIdC"]
		conductorN_id = line["NeutralConductorId"]
		self.add_overhead_line_conductors([conductorA_id,conductorB_id,conductorC_id,conductorN_id],version)
		spacing_id = line["ConductorSpacingId"]
		self.add_line_spacing(spacing_id,version)
		configuration_name = self.add_line_configuration([conductorA_id,conductorB_id,conductorC_id,conductorN_id,spacing_id],version)
		return self.object("overhead_line", line_name, {
			"length" : "%.2f m"%length,
			"configuration" : configuration_name,
			})

	# add an unbalanced overhead line based on a link
	def add_overhead_line_unbalanced(self,line_id,line,version):
		line_name = self.name(line_id,"link")
		configuration_id = line["LineId"]
		configuration_name = self.name(configuration_id,"line_configuration")
		length = float(line["Length"])
		if length == 0.0:
			length = 0.01
		if not configuration_name in self.objects.keys():
			configuration = table_get(cyme_table['eqoverheadlineunbalanced'],configuration_id,None,'EquipmentId')
			conductorA_id = configuration["PhaseConductorIdA"]
			conductorB_id = configuration["PhaseConductorIdB"]
			conductorC_id = configuration["PhaseConductorIdC"]
			conductorN_id = configuration["NeutralConductorId"]
			conductor_names = self.add_overhead_line_conductors([conductorA_id,conductorB_id,conductorC_id,conductorN_id],version)
			spacing_id = configuration["ConductorSpacingId"]
			spacing_name = self.add_line_spacing(spacing_id,version)
			self.object("line_configuration",configuration_name,{
				"conductor_A" : conductor_names[0],
				"conductor_B" : conductor_names[1],
				"conductor_C" : conductor_names[2],
				"conductor_N" : conductor_names[3],
				"spacing" : spacing_name,
				})
		return self.object("overhead_line", line_name, {
			"length" : "%.2f m"%length,
			"configuration" : configuration_name,
			})

	# add an underground line based on a link
	def add_underground_line(self,line_id,line,version):
		line_name = self.name(line_id,"link")
		if version == 5020:
			
			## TODO
			if float(line["Length"]) < 0.05:
				## SCE feeder UG line length unit is km
				warning(f"{cyme_mdbname}@{network_id}: length of line '{line_id}' may in km.")
			length = float(line["Length"])
		else:
			length = float(line["Length"])
		if length == 0.0:
			length = 0.01
		cable_conductor_id = line["CableId"]
		conductor_name = self.name(cable_conductor_id,"underground_line_conductor")
		if 'eqconductor' in cyme_equipment_table.keys():
			cable_conductor = table_get(cyme_equipment_table['eqconductor'],cable_conductor_id,None,'EquipmentId')
		# elif 'csvundergroundcable' in cyme_equipment_table.keys():
		# 	## TODO
		elif 'eqconductor' in cyme_table.keys():
			cable_conductor = table_get(cyme_table['eqconductor'],cable_conductor_id,None,'EquipmentId')
		if not conductor_name in self.objects.keys():
			if cable_conductor is None:
				warning(f"{cyme_mdbname}@{network_id}: UG cable conductor {cable_conductor_id} of line '{line_id}' is missing in CYME model, use default settings instead.")
				# only use default settings for now
				self.object("underground_line_conductor",conductor_name,{
					"outer_diameter" : "0.968 cm",
					"conductor_gmr" : "0.0319 cm",
					"conductor_diameter" : "0.968 cm",
					"conductor_resistance" : "0.139 Ohm/km",
					"neutral_gmr" : "0.00208 cm",
					"neutral_resistance" : "14.8722 Ohm/km",
					"neutral_diameter" : "0.0641 cm",
					"neutral_strands" : "16",
					"rating.summer.continuous" : "500 A",
					})
			else:
				gmr = float(cable_conductor["GMR"])
				r25 = float(cable_conductor["R25"])
				diameter = float(cable_conductor["Diameter"])
				nominal_rating = float(cable_conductor["FirstRating"])
				if nominal_rating == 0:
					nominal_rating = 1000				
				if r25 == 0:
					r25 = 0.00001
				if gmr == 0:
					gmr = 0.01
				obj = self.object("underground_line_conductor",conductor_name,{
					"outer_diameter" : "%.2f cm" % diameter,
					"conductor_gmr" : "%.2f cm" % gmr,
					"conductor_diameter" : "%.2f cm" % diameter,
					"conductor_resistance" : "%.5f Ohm/km" % r25,
					"neutral_gmr" : "0.00208 cm",
					"neutral_resistance" : "14.8722 Ohm/km",
					"neutral_diameter" : "0.0641 cm",
					"neutral_strands" : "16",
					"rating.summer.continuous" : "%.1f A" % nominal_rating,
					})
		try:
			line_phases = self.objects[line_name]['phases']
		except:
			error(f'cannot find the link objects for underground line {line_id}', 20)
		if "N" not in line_phases:
			line_phases = line_phases + "N"
		spacing_name = self.name(f'UL_{line_id}_{line_phases}',"line_spacing")
		if not spacing_name in self.objects.keys():
			# only use default settings for now
			UL_spacings = {}
			if 'A' in line_phases and 'B' in line_phases:
				UL_spacings['distance_AB'] = "0.1 m"
			if 'B' in line_phases and 'C' in line_phases:
				UL_spacings['distance_BC'] = "0.1 m"
			if 'A' in line_phases and 'C' in line_phases:
				UL_spacings['distance_AC'] = "0.1 m"
			if 'A' in line_phases:
				UL_spacings['distance_AN'] = "0.0477 m"
			if 'B' in line_phases:
				UL_spacings['distance_BN'] = "0.0477 m"
			if 'C' in line_phases:
				UL_spacings['distance_CN'] = "0.0477 m"
			self.object("line_spacing",spacing_name,UL_spacings)
		configuration_name = self.name(f'UL_{line_id}_{line_phases}',"line_configuration")
		if not configuration_name in self.objects.keys():
			UL_configs = {}
			if 'A' in line_phases:
				UL_configs['conductor_A'] = conductor_name
			if 'B' in line_phases:
				UL_configs['conductor_B'] = conductor_name
			if 'C' in line_phases:
				UL_configs['conductor_C'] = conductor_name
			if 'N' in line_phases:
				UL_configs['conductor_N'] = conductor_name
			UL_configs['spacing'] = spacing_name
			self.object("line_configuration",configuration_name,UL_configs)
		return self.object("underground_line", line_name, {
			"length" : "%.2f m"%length,
			"configuration" : configuration_name,
			})

	# add overhead line conductor library entry
	def add_overhead_line_conductors(self,conductors,version):
		conductor_names = []
		for conductor_id in conductors:
			conductor_name = self.name(conductor_id,"overhead_line_conductor")
			conductor = None
			if not conductor_name in self.objects.keys():
				if 'eqconductor' in cyme_equipment_table.keys():
					conductor = table_get(cyme_equipment_table['eqconductor'],conductor_id,None,'EquipmentId')
				elif 'eqconductor' in cyme_table.keys():
					conductor = table_get(cyme_table['eqconductor'],conductor_id,None,'EquipmentId')
				else:
					error(f"cannot add cable conductor {conductor_name} for version {version}", 21)
				if conductor is None:
					warning(f"{cyme_mdbname}@{network_id}: OH cable conductor {conductor_id} is missing in CYME model, use default settings instead.")
					# use default settings.
					obj = self.object("overhead_line_conductor",conductor_name,{
						"geometric_mean_radius" : "0.1 cm",
						"resistance" : "0.5 Ohm/km",
						"diameter" : "1 cm",
						"rating.summer.continuous" : "1000 A",
						"rating.winter.continuous" : "1000 A",
						"rating.summer.emergency" : "1000 A",
						"rating.winter.emergency" : "1000 A",
						})
				else:
					gmr = float(conductor["GMR"])
					r25 = float(conductor["R25"])
					diameter = float(conductor["Diameter"])
					nominal_rating = float(conductor["NominalRating"])
					# should set up NONE conductor rating and resistance as non-zero value
					# cannot use modify.csv to change the ratings fior OC_NONE
					if nominal_rating == 0:
						nominal_rating = 1000				
					if r25 == 0:
						r25 = 0.00001
					if gmr == 0:
						gmr = 0.01
					obj = self.object("overhead_line_conductor",conductor_name,{
						"geometric_mean_radius" : "%.2f cm" % gmr,
						"resistance" : "%.5f Ohm/km" % r25,
						"diameter" : "%.2f cm" % diameter,
						"rating.summer.continuous" : "%.1f A" % nominal_rating,
						"rating.winter.continuous" : "%.1f A" % nominal_rating,
						"rating.summer.emergency" : "%.1f A" % nominal_rating,
						"rating.winter.emergency" : "%.1f A" % nominal_rating,
						})
			conductor_names.append(conductor_name)
		return conductor_names

	# line spacing library object
	def add_line_spacing(self,spacing_id,version):
		spacing_name = self.name(spacing_id,"line_spacing")
		if not spacing_name in self.objects.keys():
			spacing = None
			if 'eqgeometricalarrangement' in cyme_equipment_table.keys():
				spacing = table_get(cyme_equipment_table['eqgeometricalarrangement'],spacing_id,None,'EquipmentId')
				if spacing is None and 'eqaveragegeoarrangement' in cyme_equipment_table.keys():
					spacing = table_get(cyme_equipment_table['eqaveragegeoarrangement'],spacing_id,None,'EquipmentId')
				if spacing is None:
					warning(f"cannot add cable spacing {spacing_id} for version {version}, use default settings")
					spacing = table_get(cyme_equipment_table['eqgeometricalarrangement'],"DEFAULT",None,'EquipmentId')
			elif 'eqgeometricalarrangement' in cyme_table.keys():
				spacing = table_get(cyme_table['eqgeometricalarrangement'],spacing_id,None,'EquipmentId')
				if spacing is None and 'eqaveragegeoarrangement' in cyme_table.keys():
					spacing = table_get(cyme_table['eqaveragegeoarrangement'],spacing_id,None,'EquipmentId')
				if spacing is None:
					warning(f"cannot add cable spacing {spacing_id} for version {version}, use default settings")
					spacing = table_get(cyme_table['eqgeometricalarrangement'],"DEFAULT",None,'EquipmentId')
			else:
				error(f"table 'eqgeometricalarrangement' for cable spacing is missing", 23)
			if spacing is None:
				error(f"cannot add cable spacing {spacing_id} for version {version}", 24)
			elif "GMDPhaseToPhase" in spacing.index:
				ABC2ABC = float(spacing["GMDPhaseToPhase"])
				ABC2N = float(spacing["GMDPhaseToNeutral"])
				ABC2E = float(spacing["AveragePhaseConductorHeight"])
				N2E = float(spacing["AverageNeutralConductorHeight"])
				self.object("line_spacing",spacing_name,{
					"distance_AB" : "%.2f m"%ABC2ABC,
					"distance_AC" : "%.2f m"%ABC2ABC,
					"distance_BC" : "%.2f m"%ABC2ABC,
					"distance_AN" : "%.2f m"%ABC2N,
					"distance_BN" : "%.2f m"%ABC2N,
					"distance_CN" : "%.2f m"%ABC2N,
					"distance_AE" : "%.2f m"%ABC2E,
					"distance_BE" : "%.2f m"%ABC2E,
					"distance_CE" : "%.2f m"%ABC2E,
					"distance_NE" : "%.2f m"%N2E,
					})
			elif "ConductorA_Horizontal" in spacing.index:
				Ax = float(spacing["ConductorA_Horizontal"])
				Ay = float(spacing["ConductorA_Vertical"])
				Bx = float(spacing["ConductorB_Horizontal"])
				By = float(spacing["ConductorB_Vertical"])
				Cx = float(spacing["ConductorC_Horizontal"])
				Cy = float(spacing["ConductorC_Vertical"])
				Nx = float(spacing["NeutralConductor_Horizontal"])
				Ny = float(spacing["NeutralConductor_Vertical"])
				ABx = Ax-Bx; ABy = Ay-By
				ACx = Ax-Cx; ACy = Ay-Cy
				BCx = Bx-Cx; BCy = By-Cy
				ANx = Ax-Nx; ANy = Ay-Ny
				BNx = Bx-Nx; BNy = By-Ny
				CNx = Cx-Nx; CNy = Cy-Ny
				self.object("line_spacing",spacing_name,{
					"distance_AB" : "%.2f m"%sqrt(ABx*ABx+ABy*ABy),
					"distance_AC" : "%.2f m"%sqrt(ACx*ACx+ACy*ACy),
					"distance_BC" : "%.2f m"%sqrt(BCx*BCx+BCy*BCy),
					"distance_AN" : "%.2f m"%sqrt(ANx*ANx+ANy*ANy),
					"distance_BN" : "%.2f m"%sqrt(BNx*BNx+BNy*BNy),
					"distance_CN" : "%.2f m"%sqrt(CNx*CNx+CNy*CNy),
					"distance_AE" : "%.2f m"%Ay,
					"distance_BE" : "%.2f m"%By,
					"distance_CE" : "%.2f m"%Cy,
					"distance_NE" : "%.2f m"%Ny,
					})
			else:
				error(f"data is missing for cable spacing {spacing_id}", 25)
		return spacing_name

	# line configuration library object
	def add_line_configuration(self,items,version):
		configuration_id = "_".join(items)
		configuration_name = self.name(configuration_id,"line_configuration")
		if not configuration_name in self.objects.keys():
			self.object("line_configuration",configuration_name,{
				"conductor_A" : self.name(items[0],"overhead_line_conductor"),
				"conductor_B" : self.name(items[1],"overhead_line_conductor"),
				"conductor_C" : self.name(items[2],"overhead_line_conductor"),
				"conductor_N" : self.name(items[3],"overhead_line_conductor"),
				"spacing" : self.name(items[4],"line_spacing")
				})
		return configuration_name

	# add a switch based on a link
	def add_switch(self,switch_id,switch,version):
		switch_name = self.name(switch_id,"link")
		phases = cyme_phase_name[int(switch["ClosedPhase"])]
		switch_config = {
		"operating_mode" : "BANKED"
		}
		for phase in phases:
			if phase != "N":
				switch_config[f'phase_{phase}_state'] = "CLOSED"
		return self.object("switch", switch_name, switch_config,overwrite=False)

	# add a breaker based on a link and a switch object
	def add_breaker(self,breaker_id,breaker,version):
		breaker_name = self.name(breaker_id,"link")
		phases = cyme_phase_name[int(breaker["ClosedPhase"])]
		breaker_config = {
		"operating_mode" : "BANKED"
		}
		for phase in phases:
			if phase != "N":
				breaker_config[f'phase_{phase}_state'] = "CLOSED"
		return self.object("switch", breaker_name, breaker_config,overwrite=False)

	# add a recloser based on a link and a switch object
	def add_recloser(self,recloser_id,recloser,version):
		recloser_name = self.name(recloser_id,"link")
		phases = cyme_phase_name[int(recloser["ClosedPhase"])]
		recloser_config = {
		"operating_mode" : "BANKED"
		}
		for phase in phases:
			if phase != "N":
				recloser_config[f'phase_{phase}_state'] = "CLOSED"
		return self.object("switch", recloser_name, recloser_config,overwrite=False)

	# add a fuse based on a link
	def add_fuse(self,fuse_id,fuse,version):
		fuse_name = self.name(fuse_id,"link")
		equipment_id = fuse["EquipmentId"]
		equipment = None
		if 'eqfuse' in cyme_equipment_table.keys():
			equipment = table_get(cyme_equipment_table['eqfuse'],equipment_id,None,'EquipmentId')
		elif 'eqfuse' in cyme_table.keys():
			equipment = table_get(cyme_table['eqfuse'],equipment_id,None,'EquipmentId')
		if equipment is None:
			# use default settings
			current_limit = "50000.0"
		else:
			current_limit = equipment["FirstRatedCurrent"]
		fuse_dict = {
			"current_limit" : f"{current_limit} A",
			"mean_replacement_time" : "3600.0",
		}
		return self.object("fuse", fuse_name, fuse_dict,overwrite=False)

	# add a load
	def add_load(self,load_id,load,version,**kwargs):
		section = kwargs["node_info"]["load_section"].squeeze()
		connection_type = kwargs["node_info"]["connection_type"]
		all_node = kwargs["node_info"]["all_node"]
		default_load_voltage = kwargs["node_info"]["load_voltage"]
		device_type = int(load["DeviceType"])
		value_type = int(load["LoadValueType"])
		if device_type == 20: # spot load is attached at from node of section
			parent_name = self.name(section["FromNodeId"],"node")
		elif device_type == 21: # distributed load is attached at to node of section
			parent_name = self.name(section["ToNodeId"],"node")
		else:
			error(f"CYME device type {device_type} is not supported as a load", 30)

		if parent_name not in self.objects.keys():
			# Definition for node "parent_name" is missing
			device_dict = kwargs["node_info"]["Device_Dicts"]
			node_links = kwargs["node_info"]["Node_Links"]
			self.add_node(parent_name[3:],node_links,device_dict,version,node_info={"all_node":all_node})

		# customer_id = load["CustomerNumber"]
		link_name = self.name(load_id,"link")
		if link_name in self.objects.keys(): # link is no longer needed
			self.delete(link_name)
		load_name = self.name(load_id,"load")
		device_type = int(load["DeviceType"])
		phase = cyme_phase_name[int(load["Phase"])]

		if load_name in self.objects.keys() and "phases" in self.objects[load_name]:
			phases = self.objects[load_name]["phases"] + phase
		else:
			phases = phase
		if device_type in glm_devices.keys():
			ConsumerClassId = load["ConsumerClassId"]
			# the default load unit in gridlabd is Volt-Amperes, or Amperes or Ohms
			load_value1 = float(load["LoadValue1"])
			load_value2 = float(load["LoadValue2"])
			# from the mdb file, type for constant power load is defined as PQ
			load_types = {"Z":"constant_impedance","I":"constant_current","PQ":"constant_power"}
			if ConsumerClassId in load_types.keys():
				load_cals_complex = load_cals(ConsumerClassId,load["Phase"],connection_type,\
					load_value1,load_value2,default_load_voltage,value_type)
				load_value1 = load_cals_complex.real
				load_value2 = -load_cals_complex.imag
				if (load_value1*load_value1 + load_value2*load_value2) > 0:
					load_dict = {
						"parent" : parent_name,
						"phases" : arrangeString(phases),
						"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
					}
					for i_phase in phase:
						load_dict[f"{load_types[ConsumerClassId]}_{i_phase}"] = "%.4g%+.4gj" % (load_value1,load_value2)
					return self.object("load",load_name,load_dict)
			elif ConsumerClassId in ["PV","SWING","SWINGPQ"]: 
				# GLM bus types allowed
				load_cals_complex = load_cals("Z",load["Phase"],connection_type,\
					load_value1,load_value2,default_load_voltage,value_type)
				load_value1 = load_cals_complex.real
				load_value2 = -load_cals_complex.imag
				if (load_value1*load_value1 + load_value2*load_value2) > 0:
					load_dict = {
						"parent" : parent_name,
						"phases" : arrangeString(phases),
						"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
						"bustype" : ConsumerClassId,
					}
					for i_phase in phase:
						load_dict[f"constant_impedance_{i_phase}"] = "%.4g%+.4gj" % (load_value1,load_value2)
					return self.object("load",load_name,load_dict)
			elif ConsumerClassId in ["CGSUB","Other","Industrial","Residential","NONE"]:
				# GLM bus types allowed
				load_cals_complex = load_cals("PQ",load["Phase"],connection_type,\
					load_value1,load_value2,default_load_voltage,value_type)
				load_value1 = load_cals_complex.real
				load_value2 = -load_cals_complex.imag
				if (load_value1*load_value1 + load_value2*load_value2) > 0:
					load_dict = {
						"parent" : parent_name,
						"phases" : arrangeString(phases),
						"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
					}
					for i_phase in phase:
						# load_dict[f"constant_power_{i_phase}"] = "%.4g%+.4gj" % (load_value1,load_value2)
						load_dict[f"constant_power_{i_phase}"] = "%.4g%+.4gj" % (0.01,0.01)
					return self.object("load",load_name,load_dict)
			else:
				warning(f"{cyme_mdbname}@{network_id}: load '{load_id}' on phase '{phase}' dropped because '{ConsumerClassId}' is not a supported CYME load type")
		else:
			warning(f"{cyme_mdbname}@{network_id}: load '{load_id}' on phase '{phase}' dropped because '{cyme_devices[device_type]}' is not a supported CYME device type")

	# add a capacitor
	def add_capacitor(self,capacitor_id,capacitor,version,**kwargs):
		section_id = table_get(cyme_table["sectiondevice"],capacitor_id,"SectionId","DeviceNumber")
		section = table_get(cyme_table["section"],section_id,None,"SectionId")
		section = kwargs["node_info"]["cap_section"].squeeze()
		from_name = self.name(section["FromNodeId"],"node")
		to_name = self.name(section["ToNodeId"],"node")
		equipment_id = capacitor["EquipmentId"]
		if 'eqtransformer' in cyme_equipment_table.keys():
			equipment = table_get(cyme_equipment_table["eqshuntcapacitor"],equipment_id,None,"EquipmentId")
		link_name = self.name(capacitor_id,"link")
		if link_name in self.objects.keys(): # link is no longer needed
			self.delete(link_name)
		KVARA = float(capacitor["KVARA"])
		if "SwitchedKVARA" in capacitor.keys(): # for NG MDB files
			KVARA = KVARA + float(capacitor["SwitchedKVARA"])
		KVARB = float(capacitor["KVARB"])
		if "SwitchedKVARB" in capacitor.keys(): # for NG MDB files
			KVARB = KVARB + float(capacitor["SwitchedKVARB"])
		KVARC = float(capacitor["KVARC"])
		if "SwitchedKVARC" in capacitor.keys(): # for NG MDB files
			KVARC = KVARC + float(capacitor["SwitchedKVARC"])
		if not KVARA + KVARB + KVARC > 0.0:
			warning(f"{cyme_mdbname}@{network_id}: capacitor {capacitor_id} has zero capacitance for all phases.")
			return
		KVLN = float(capacitor["KVLN"])
		ConnectionConfig = int(capacitor["ConnectionConfiguration"]) # 2 for delta and else for wye
		capacitor_name = self.name(capacitor_id,"capacitor")
		control = "MANUAL"
		self.assume(capacitor_name,"control",control,f"capacitor {fix_name(capacitor_id)} does not specify a control strategy, valid options are 'CURRENT', 'VARVOLT', 'VOLT', 'VAR', or 'MANUAL'")

		if "Phase" in capacitor.keys():
			phase = cyme_phase_name[int(capacitor["Phase"])]
		elif "ByPhase" in capacitor.keys():
			phase = cyme_phase_name[int(capacitor["ByPhase"])]
		else:
			warning(f"{cyme_mdbname}@{network_id}: capacitor {capacitor_id} does not specify, phase will be specified based on capacitance data")
			phase = cyme_phase_name[capacitor_phase_cals(KVARA,KVARB,KVARC)]

		capacitor_dict = {
				"parent" : from_name,
				"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
		}
		phase = ""
		if KVARA > 0.0:
			CAPACITOR_A = f"{KVARA} kVA"
			SWITCH_A  = "CLOSED"
			if ConnectionConfig == 2:
				phase = phase + "AB"
			else:
				phase = phase + "A"
		else:
			CAPACITOR_A = f"0 kVA"
			SWITCH_A  = "OPEN"
		if KVARB > 0.0:
			CAPACITOR_B = f"{KVARB} kVA"
			SWITCH_B  = "CLOSED"
			if ConnectionConfig == 2:
				phase = phase + "BC"
			else:
				phase = phase + "B"
		else:
			CAPACITOR_B = f"0 kVA"
			SWITCH_B  = "OPEN"
		if KVARC > 0.0:
			CAPACITOR_C = f"{KVARC} kVA"
			SWITCH_C  = "CLOSED"
			if ConnectionConfig == 2:
				phase = phase + "AC"
			else:
				phase = phase + "C"
		else:
			CAPACITOR_C = f"0 kVA"
			SWITCH_C  = "OPEN"
		phase = clean_phases(phase)
		if ConnectionConfig == 0 and "N" not in phase:
			phase = phase + "N"
		elif ConnectionConfig > 0 and "N" in phase:
			phase.replace("N","")
		capacitor_dict["phases"] = phase
		capacitor_dict["capacitor_A"] = CAPACITOR_A
		capacitor_dict["capacitor_B"] = CAPACITOR_B
		capacitor_dict["capacitor_C"] = CAPACITOR_C
		capacitor_dict["switchA"] = SWITCH_A
		capacitor_dict["switchB"] = SWITCH_B
		capacitor_dict["switchC"] = SWITCH_C
		capacitor_dict["control"] = "MANUAL"
		capacitor_dict["control"] = "MANUAL"
		return self.object("capacitor",capacitor_name,capacitor_dict)

	# add a transformer
	def add_transformer(self,transformer_id, transformer,version):
		DeviceType = int(transformer["DeviceType"])
		equipment_id = transformer["EquipmentId"]
		equipment = None
		if 'eqtransformer' in cyme_equipment_table.keys():
			equipment = table_get(cyme_equipment_table["eqtransformer"],equipment_id,None,"EquipmentId")
		elif 'eqtransformer' in cyme_table.keys():
			equipment = table_get(cyme_table["eqtransformer"],equipment_id,None,"EquipmentId")
		else:
			warning(f"{cyme_mdbname}@{network_id}: cannot find cyme table 'eqtransformer'.")
		if equipment is None:
			warning(f"{cyme_mdbname}@{network_id}: equipment {equipment_id} of transformer '{transformer_id}' is missing in CYME model, use default settings instead.")
			if 'eqtransformer' in cyme_equipment_table.keys():
				equipment = table_get(cyme_equipment_table["eqtransformer"],"DEFAULT",None,"EquipmentId")
			elif 'eqtransformer' in cyme_table.keys():
				equipment = table_get(cyme_table["eqtransformer"],"DEFAULT",None,"EquipmentId")
			NominalRatingKVA = float(equipment["NominalRatingKVA"])
			PosSeqImpedancePercent = float(equipment["PosSeqImpedancePercent"])
			XRRatio = float(equipment["XRRatio"])
			try:
					PrimarySecondaryVoltag= equipment_id.split("_")[1]
					PrimaryVoltageKVLL = float(PrimarySecondaryVoltag.split("/")[0])
					if PrimaryVoltageKVLL > 50:
						PrimaryVoltageKVLL = PrimaryVoltageKVLL/1000
					SecondaryVoltageKVLL = float(PrimarySecondaryVoltag.split("/")[1])
					if SecondaryVoltageKVLL > 50:
						SecondaryVoltageKVLL = SecondaryVoltageKVLL/1000
					primary_voltage = "%.4gkV" % (PrimaryVoltageKVLL/sqrt(3.0))
					secondary_voltage = "%.4gkV" % (SecondaryVoltageKVLL/sqrt(3.0))
			except:
				warning(f"{cyme_mdbname}@{network_id}: Connot get the PrimaryVoltageKVLL/SecondaryVoltageKVLL from the name of equipment {equipment_id}. Use default settings instead.")
				PrimaryVoltageKVLL = float(equipment["PrimaryVoltageKVLL"])
				SecondaryVoltageKVLL = float(equipment["SecondaryVoltageKVLL"])
		else:
			NominalRatingKVA = float(equipment["NominalRatingKVA"])
			PrimaryVoltageKVLL = float(equipment["PrimaryVoltageKVLL"])
			SecondaryVoltageKVLL = float(equipment["SecondaryVoltageKVLL"])
			PosSeqImpedancePercent = float(equipment["PosSeqImpedancePercent"])
			XRRatio = float(equipment["XRRatio"])
			primary_voltage = "%.4gkV" % (PrimaryVoltageKVLL/sqrt(3.0))
			secondary_voltage = "%.4gkV" % (SecondaryVoltageKVLL/sqrt(3.0))
		r = XRRatio / 100.0 / sqrt(1+XRRatio**2)
		x = r * XRRatio
		nominal_rating = "%.4gkVA" % (NominalRatingKVA)
		configuration_name = self.name([nominal_rating,primary_voltage,secondary_voltage,"R%.4g"%(r),"X%4g"%(x)], "transformer_configuration")
		if primary_voltage == secondary_voltage:
			secondary_voltage = "%.4gkV" % ((SecondaryVoltageKVLL+0.001)/sqrt(3.0))
			self.assume(configuration_name,"secondary_voltage",secondary_voltage,f"transformer {fix_name(transformer_id)} primary voltage is the same as secondary voltage")
		if r == 0.0:
			r = 0.000333
			x = 0.00222
			self.assume(configuration_name,"resistance",r,f"transformer {fix_name(transformer_id)} XRRatio is zero")
			self.assume(configuration_name,"reactance",x,f"transformer {fix_name(transformer_id)} XRRatio is zero")

		connect_type = "WYE_WYE"
		self.assume(configuration_name,"connect_type",connect_type,f"transformer '{fix_name(transformer_id)}' does not specify connection type")
		install_type = "PADMOUNT"
		self.assume(configuration_name,"install_type",install_type,f"transformer '{fix_name(transformer_id)}' does not specify install type")

		self.object("transformer_configuration", configuration_name, {
			"connect_type" : "WYE_WYE",
			"install_type" : "PADMOUNT",
			"power_rating" : "%.4gkVA" % (NominalRatingKVA),
			"primary_voltage" : primary_voltage,
			"secondary_voltage" : secondary_voltage,
			"resistance" : r,
			"reactance" : x,
			})
		# add a transformer based on a link
		link_name = self.name(transformer_id,"link")
		return self.object("transformer", link_name, {
			"nominal_voltage" : None,
			"phases" : "".join(sorted(set(self.objects[link_name]["phases"] + "N"))),
			"configuration" : configuration_name,
			})

	# add a single phase transformer
	def add_single_transformer(self,transformer_id, transformer,version):
		for n in range(1,4):
			equipment_id = transformer[f"PhaseTransformerID{n}"]
			if isinstance(equipment_id, str):
				if 'eqtransformer' in cyme_equipment_table.keys():
					equipment = table_get(cyme_equipment_table["eqtransformer"],equipment_id,None,"EquipmentId")
				elif 'eqtransformer' in cyme_table.keys():
					equipment = table_get(cyme_table["eqtransformer"],equipment_id,None,"EquipmentId")
				else:
					warning(f"{cyme_mdbname}@{network_id}: equipment {equipment_id} of transformer '{transformer_id}' is missing in CYME model, use default settings instead.")
					if 'eqtransformer' in cyme_equipment_table.keys():
						equipment = table_get(cyme_equipment_table["eqtransformer"],"DEFAULT",None,"EquipmentId")
					elif 'eqtransformer' in cyme_table.keys():
						equipment = table_get(cyme_table["eqtransformer"],"DEFAULT",None,"EquipmentId")
					else:
						error(f"cannot add single transformer.", 40)
				NominalRatingKVA = float(equipment["NominalRatingKVA"])
				PrimaryVoltageKVLL = float(equipment["PrimaryVoltageKVLL"])
				SecondaryVoltageKVLL = float(equipment["SecondaryVoltageKVLL"])
				PosSeqImpedancePercent = float(equipment["PosSeqImpedancePercent"])
				XRRatio = float(equipment["XRRatio"])
				r = XRRatio / 100.0 / sqrt(1+XRRatio**2)
				x = r * XRRatio
				nominal_rating = "%.4gkVA" % (NominalRatingKVA)
				primary_voltage = "%.4gkV" % (PrimaryVoltageKVLL/sqrt(3.0))
				secondary_voltage = "%.4gkV" % (SecondaryVoltageKVLL/sqrt(3.0))
				configuration_name = self.name([nominal_rating,primary_voltage,secondary_voltage,"R%.4g"%(r),"X%4g"%(x),cyme_phase_name[n]], "transformer_configuration")
				if primary_voltage == secondary_voltage:
					secondary_voltage = "%.4gkV" % ((SecondaryVoltageKVLL+0.001)/sqrt(3.0))
					self.assume(configuration_name,"secondary_voltage",secondary_voltage,f"transformer {fix_name(transformer_id)} primary voltage is the same as secondary voltage")
				if r == 0.0:
					r = 0.000333
					x = 0.00222
					self.assume(configuration_name,"resistance",r,f"transformer {fix_name(transformer_id)} XRRatio is zero")
					self.assume(configuration_name,"reactance",x,f"transformer {fix_name(transformer_id)} XRRatio is zero")
				connect_type = "SINGLE_PHASE"
				self.assume(configuration_name,"connect_type",connect_type,f"transformer '{fix_name(transformer_id)}' does not specify connection type")
				install_type = "PADMOUNT"
				self.assume(configuration_name,"install_type",install_type,f"transformer '{fix_name(transformer_id)}' does not specify install type")

				self.object("transformer_configuration", configuration_name, {
					"connect_type" : connect_type,
					"install_type" : install_type,
					"power_rating" : "%.4gkVA" % (NominalRatingKVA),
					"primary_voltage" : primary_voltage,
					"secondary_voltage" : secondary_voltage,
					"resistance" : r,
					"reactance" : x,
					})
				link_name = self.name(transformer_id,"link")
				self.object("single_transformer", link_name, {
					"nominal_voltage" : None,
					"phases" : "".join(sorted(set(cyme_phase_name[n] + "N"))),
					"configuration" : configuration_name,
					})

	# add a regulator
	def add_regulator(self, regulator_id, regulator, version):
		equipment_id = regulator["EquipmentId"]
		equipment = None
		if 'eqregulator' in cyme_equipment_table.keys():
			equipment = table_get(cyme_equipment_table["eqregulator"],equipment_id,None,"EquipmentId")
		elif 'eqregulator' in cyme_table.keys():
			equipment = table_get(cyme_table["eqregulator"],equipment_id,None,"EquipmentId")
		else:
			error(f"cannot find cyme table 'eqtransformer'.", 50)
		CTPrimaryRating = float(regulator["CTPrimaryRating"])
		PTRatio = float(regulator["PTRatio"])
		try:
			BandWidth = float(regulator["BandWidth"])
		except KeyError as err:
			warning(f"Regulator '{regulator_id}' doesn't define {err}, default value will be used")
			BandWidth = 2.0
		BoostPercent = float(regulator["BoostPercent"])
		BuckPercent = float(regulator["BuckPercent"])
		TapPositionA = float(regulator["TapPositionA"])
		TapPositionB = float(regulator["TapPositionB"])
		TapPositionC = float(regulator["TapPositionC"])
		ControlStatus = float(regulator["ControlStatus"])
		ReverseSensingMode = float(regulator["ReverseSensingMode"])
		ReverseThreshold = float(regulator["ReverseThreshold"])
		X = float(regulator["X"])
		Y = float(regulator["Y"])
		Status = int(regulator["Status"])
		Reversible = int(regulator["Reversible"])

		if equipment is None:
			if 'eqregulator' in cyme_equipment_table.keys():
				equipment = table_get(cyme_equipment_table["eqregulator"],"DEFAULT",None,"EquipmentId")
			elif 'eqregulator' in cyme_table.keys():
				equipment = table_get(cyme_table["eqregulator"],"DEFAULT",None,"EquipmentId")
		RatedKVA = float(equipment["RatedKVA"])
		RatedKVLN = float(equipment["RatedKVLN"])
		NumberOfTaps = int(equipment["NumberOfTaps"])

		connect_type = "WYE_WYE"
		Control = "OUTPUT_VOLTAGE"
		time_delay = "30s"
		band_center = "${GLM_NOMINAL_VOLTAGE}"
		band_width = "%.1gV" % (BandWidth)

		configuration_name = self.name([regulator_id,band_width,time_delay],"regulator_configuration")
		self.assume(configuration_name,"connect_type",connect_type,f"regulator '{fix_name(regulator_id)}' does not specify connection type")
		self.assume(configuration_name,"Control",Control,f"regulator '{fix_name(regulator_id)}' does not specify control type")
		self.assume(configuration_name,"time_delay",time_delay,f"regulator '{fix_name(regulator_id)}' does not specify time delay")
		self.assume(configuration_name,"band_center",band_center,f"regulator '{fix_name(regulator_id)}' does not specify band center")

		self.object("regulator_configuration", configuration_name, {
			"connect_type" : connect_type,
			"band_center" : band_center,
			"band_width" : band_width,
			"time_delay" : time_delay,
			"raise_taps" : "%.0f" % float(NumberOfTaps/2),
			"lower_taps" : "%.0f" % float(NumberOfTaps/2),
			"current_transducer_ratio" : "%.0f" % CTPrimaryRating,
			"power_transducer_ratio" : "%.0f" % PTRatio,
			"regulation" : "%.4f%%" % (BandWidth / (RatedKVLN*1000) * 100),
			"tap_pos_A" : "%.0f" % (TapPositionA),
			"tap_pos_B" : "%.0f" % (TapPositionB),
			"tap_pos_C" : "%.0f" % (TapPositionC),
			"Control" : Control
			})

		link_name = self.name(regulator_id,"link")
		regulator_name = self.name(link_name,"regulator")
		regulator_name = fix_name(regulator_name)
		sense_node = self.objects[link_name]["to"]
		self.assume(regulator_name,"sense_node",sense_node,f"regulator '{fix_name(regulator_id)}' does not specify sense node")
		return self.object("regulator", self.name(regulator_id,"link"), {
			"configuration" : configuration_name,
			"sense_node" : sense_node,
			})
	
	# add a PV system including PV panel, inverter, and meter
	def add_photovoltaic(self, photovoltaic_id, photovoltaic, version, **kwargs):
		section = kwargs["node_info"]["pv_section"].squeeze()
		all_node = kwargs["node_info"]["all_node"]
		parent_name = self.name(section["FromNodeId"],"node")
		inverter = table_get(cyme_table["dggenerationmodel"],photovoltaic_id,None,"DeviceNumber")
		rated_power = float(inverter["ActiveGeneration"])
		power_factor = float(inverter["PowerFactor"])/100
		if power_factor > 1:
			error(f"power factor for solar inverter {photovoltaic_id} is greater than 1.0", 70)
		efficiency = float(table_get(cyme_table["converter"],photovoltaic_id,"Efficiency","DeviceNumber"))/100.0
		if efficiency > 1:
			error(f"converter efficiency for solar inverter {photovoltaic_id} is greater than 1.0", 71)
		if parent_name not in self.objects.keys():
			# Definition of node "parent_name" is missing
			device_dict = kwargs["node_info"]["Device_Dicts"]
			node_links = kwargs["node_info"]["Node_Links"]
			self.add_node(parent_name[3:],node_links,device_dict,version,node_info={"all_node":all_node})

		pv_name = self.name(photovoltaic_id,"photovoltaic")
		inverter_name = self.name(photovoltaic_id,"inverter")
		meter_name = self.name(photovoltaic_id,"meter")
		phases = cyme_phase_name[int(photovoltaic["Phase"])]
		panel_efficiency = 0.2
		panel_area = math.ceil(13.3*rated_power/panel_efficiency) # got from NG converter, double-check is needed
		pv_dict = {
			"parent" : inverter_name,
			"generator_status" : "ONLINE",
			"generator_mode" : "SUPPLY_DRIVEN",
			"panel_type" : "MULTI_CRYSTAL_SILICON",
			"efficiency" : panel_efficiency,
			"area" : panel_area,
			"tilt_angle" : "45",
			"orientation_azimuth" : "180",
			"orientation" : "FIXED_AXIS",
		}
		self.object("solar", pv_name, pv_dict,overwrite=False)
		inverter_dict = {
			"parent" : meter_name,
			"generator_status" : "ONLINE",
			"generator_mode" : "CONSTANT_PF",
			"inverter_type" : "PWM",
			"inverter_efficiency" : efficiency,
			"phases" : phases,
			"power_factor" : power_factor,
			"rated_power" : rated_power*1000,
		}
		self.object("inverter", inverter_name, inverter_dict,overwrite=False)
		meter_dict = {
			"phases" : phases,
			"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
		}
		self.object("meter", meter_name, meter_dict,overwrite=False)

		line_name = self.name(photovoltaic_id,"overhead_line")
		conductorABC_id = "DEFAULT"
		conductorN_id = "DEFAULT"
		self.add_overhead_line_conductors([conductorABC_id,conductorN_id],version)
		spacing_id = "DEFAULT"
		self.add_line_spacing(spacing_id,version)
		configuration_name = self.add_line_configuration([conductorABC_id,conductorABC_id,conductorABC_id,conductorN_id,spacing_id],version)
		return self.object("overhead_line", line_name, {
			"phases" : phases,
			"from" : parent_name,
			"to" : meter_name,
			"length" : "1 m",
			"configuration" : configuration_name,
			})

	def node_checks(self, node_dict, node_links, device_dict, version,**kwargs): # check node objects
		list_of_from = []
		list_of_to = []
		list_of_parent = []
		all_node = kwargs["node_info"]["all_node"]
		for name in list(self.objects.keys()): # missing node: if a node object is used in link object but not been difined
			data = self.objects[name]
			if 'from' in data.keys():
				if data["from"] not in self.objects.keys():
					node_dict[data["from"]] = self.add_node(data["from"][3:], node_links, device_dict, version, node_info={"all_node":all_node})
				if data["from"] not in list_of_from:
					list_of_from.append(data["from"])
			if 'to' in data.keys():
				if data["to"] not in self.objects.keys():
					node_dict[data["to"]] = self.add_node(data["to"][3:], node_links, device_dict, version, node_info={"all_node":all_node})
				if data["to"] not in list_of_to:
					list_of_to.append(data["to"])
			if 'parent' in data.keys():
				if data["parent"] not in self.objects.keys():
					node_dict[data["parent"]] = self.add_node(data["parent"][3:], node_links, device_dict, version, node_info={"all_node":all_node})
				if data["parent"] not in list_of_parent:
					list_of_parent.append(data["parent"])
		for name in list(self.objects.keys()):
			data = self.objects[name]
			if "class" in data.keys() and data["class"] == "node":
				if data["name"] not in list_of_from and data["name"] not in list_of_to and data["name"] not in list_of_parent: # islanded nodes
					warning(f'{cyme_mdbname}@{network_id}: node {data["name"]}  is islanded.')
					self.delete(name)
				if "parent" in data.keys() and data["name"] == data["parent"]["name"]: # the object's parent is itself
					warning(f"{cyme_mdbname}@{network_id}: {name}'parent is itself.")
					self.delete(name)
			if 'from' in data.keys() and 'to' in data.keys() and data["from"] == data["to"]: # section loops back to itself
				warning(f'{cyme_mdbname}@{network_id}: section {name} loops back to itself.')
				self.delete(name)
		return node_dict

	def link_checks(self):	# check link objects
		done = False
		while not done:
			done = True
			for name in list(self.objects.keys()):
				try:
					data = self.objects[name]
					if "class" in data.keys() and data["class"] == "link": # link needs to be collapse
						self.delete(name)
						done = False
						break
					elif "class" in data.keys() and data["class"] in ["node","load"] and "parent" in data.keys(): # don't allow grandchild cyme_table["node"]
						parent_name = data["parent"]
						parent_data = self.objects[parent_name]
						if "class" in parent_data.keys() and parent_data["class"] in ["node","load"] and "parent" in parent_data.keys():
							grandparent = parent_data["parent"]
							data["parent"] = grandparent
							done = False
							break
				except Exception as exc:
					warning(format_exception("link removal failed",name,self.objects[name]))
					self.delete(name)
					pass

	def section_checks(self): # remove parallel section between two nodes
		multi_g = nx.MultiGraph()
		for name in list(self.objects.keys()):
			try:
				data = self.objects[name]
				if "from" in data.keys() and "to" in data.keys():
					if data["from"] not in multi_g:
						multi_g.add_node(data["from"])
					if data["to"] not in multi_g:
						multi_g.add_node(data["to"])
					multi_g.add_edge(data["from"],data["to"],edge_name=name,edge_phase=data["phases"].replace("N",""))
			except Exception as exc:
				warning(format_exception("connection removal failed",name,self.objects[name]))
				self.delete(name)
				pass
		for u in multi_g.nodes():
			for neighbor in multi_g.neighbors(u):
				if multi_g.number_of_edges(u,neighbor)>1:
					edge_data = {}
					for edge_id in multi_g[u][neighbor].keys():
						if multi_g[u][neighbor][edge_id]["edge_name"][0:2] not in edge_data.keys():
							edge_data[multi_g[u][neighbor][edge_id]["edge_name"][0:2]] = edge_id
						else:
							warning(f"{cyme_mdbname}@{network_id}: multiple {multi_g[u][neighbor][edge_id]['edge_name'][0:2]} devices connected between {u} and {neighbor}.")
							object_name = multi_g[u][neighbor][edge_id]["edge_name"]
							if object_name in self.objects.keys():
								glm_output_print(f"object_name is deleted {object_name}.")
								self.delete(object_name)
					# RG > TF > SW > FS > OL = UL
					if "RG" in edge_data.keys(): # one of the multi-edges is regulator
						if "OL" in edge_data.keys() or "UL" in edge_data.keys(): # add a node to handle both RG and OL/UL
							name_node_added = multi_g[u][neighbor][edge_data["RG"]]["edge_name"]
							self.object("node",self.name(name_node_added,"node"),{
								"phases" : multi_g[u][neighbor][edge_data["RG"]]["edge_phase"] + "N",
								"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
								"bustype" : "PQ",
								})
							RG_object_name = multi_g[u][neighbor][edge_data["RG"]]["edge_name"]
							self.objects[RG_object_name]["to"] = self.name(name_node_added,"node")
							if "OL" in edge_data.keys():
								line_object_name = multi_g[u][neighbor][edge_data["OL"]]["edge_name"]
							else:
								line_object_name = multi_g[u][neighbor][edge_data["UL"]]["edge_name"]
							self.objects[line_object_name]["from"] = self.name(name_node_added,"node")
						for key in edge_data.keys():
							if key != "RG" and key != "OL" and key != "UL":
								object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
								if object_name in self.objects.keys():
									self.delete(object_name)
					elif "TF" in edge_data.keys(): # one of the multi-edges is transformer
						if "OL" in edge_data.keys() or "UL" in edge_data.keys(): # add a node to handle both RG and OL/UL
							name_node_added = multi_g[u][neighbor][edge_data["TF"]]["edge_name"]
							TF_object_name = multi_g[u][neighbor][edge_data["TF"]]["edge_name"]
							TF_config_name = self.objects[TF_object_name]["configuration"]
							self.object("node",self.name(name_node_added,"node"),{
								"phases" : multi_g[u][neighbor][edge_data["TF"]]["edge_phase"] + "N",
								"nominal_voltage" : f"{self.objects[TF_config_name]['secondary_voltage']}",
								"bustype" : "PQ",
								})
							
							self.objects[TF_object_name]["to"] = self.name(name_node_added,"node")
							if "OL" in edge_data.keys():
								line_object_name = multi_g[u][neighbor][edge_data["OL"]]["edge_name"]
							else:
								line_object_name = multi_g[u][neighbor][edge_data["UL"]]["edge_name"]
							self.objects[line_object_name]["from"] = self.name(name_node_added,"node")
						for key in edge_data.keys():
							if key != "TF" and key != "OL" and key != "UL":
								object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
								if object_name in self.objects.keys():
									self.delete(object_name)
					elif "SW" in edge_data.keys():
						# one of the multi-edges is switch
						if "OL" in edge_data.keys() or "UL" in edge_data.keys(): # add a node to handle both RG and OL/UL
							name_node_added = multi_g[u][neighbor][edge_data["SW"]]["edge_name"]
							self.object("node",self.name(name_node_added,"node"),{
								"phases" : multi_g[u][neighbor][edge_data["SW"]]["edge_phase"] + "N",
								"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
								"bustype" : "PQ",
								})
							SW_object_name = multi_g[u][neighbor][edge_data["SW"]]["edge_name"]
							self.objects[SW_object_name]["to"] = self.name(name_node_added,"node")
							if "OL" in edge_data.keys():
								line_object_name = multi_g[u][neighbor][edge_data["OL"]]["edge_name"]
							else:
								line_object_name = multi_g[u][neighbor][edge_data["UL"]]["edge_name"]
							self.objects[line_object_name]["from"] = self.name(name_node_added,"node")
						for key in edge_data.keys():
							if key != "SW" and key != "OL" and key != "UL":
								object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
								if object_name in self.objects.keys():
									self.delete(object_name)
					elif "FS" in edge_data.keys():
						# one of the multi-edges is fuse
						if "OL" in edge_data.keys() or "UL" in edge_data.keys(): # add a node to handle both RG and OL/UL
							name_node_added = multi_g[u][neighbor][edge_data["FS"]]["edge_name"]
							self.object("node",self.name(name_node_added,"node"),{
								"phases" : multi_g[u][neighbor][edge_data["FS"]]["edge_phase"] + "N",
								"nominal_voltage" : "${GLM_NOMINAL_VOLTAGE}",
								"bustype" : "PQ",
								})
							FS_object_name = multi_g[u][neighbor][edge_data["FS"]]["edge_name"]
							self.objects[FS_object_name]["to"] = self.name(name_node_added,"node")
							if "OL" in edge_data.keys():
								line_object_name = multi_g[u][neighbor][edge_data["OL"]]["edge_name"]
							else:
								line_object_name = multi_g[u][neighbor][edge_data["UL"]]["edge_name"]
							self.objects[line_object_name]["from"] = self.name(name_node_added,"node")
						for key in edge_data.keys():
							if key != "FS" and key != "OL" and key != "UL":
								object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
								if object_name in self.objects.keys():
									self.delete(object_name)
					elif "OL" in edge_data.keys() or "UL" in edge_data.keys():
						for key in edge_data.keys():
							if "OL" in edge_data.keys() and key != "OL":
								object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
								if object_name in self.objects.keys():
									self.delete(object_name)
							elif "UL" in edge_data.keys() and key != "UL":
								object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
								if object_name in self.objects.keys():
									self.delete(object_name)
					else:
						for key in edge_data.keys():
							object_name = multi_g[u][neighbor][edge_data[key]]["edge_name"]
							print(self.objects[object_name])
						error(f"CYME model has unsupported duplicate connections between {u} and {neighbor}", 60)

	def phase_checks(self): # check phase dismatch
		check_done = False
		while not check_done:
			check_done = True
			for name in list(self.objects.keys()):
				data = self.objects[name]
				target_node_name = None
				target_device_name = None
				if "from" in data.keys() and "to" in data.keys():
					target_device_name = data["name"]
				elif "parent" in data.keys() and data["class"] != "solar":
					target_node_name = data["parent"]
				if target_device_name:
					if len(self.objects[data["from"]]["phases"]) < len(self.objects[data["to"]]["phases"]):
						for phase in self.objects[data["from"]]["phases"].replace("N",""):
							if phase not in self.objects[data["to"]]["phases"]:
								warning(f"{cyme_mdbname}@{network_id} phase dismatch: {data['from']} has {self.objects[data['from']]['phases']} \
									but {data['to']} has {self.objects[data['to']]['phases']}")
					else:
						for phase in self.objects[data["to"]]["phases"].replace("N",""):
							if phase not in self.objects[data["from"]]["phases"]:
								warning(f"{cyme_mdbname}@{network_id} phase dismatch: {data['from']} has {self.objects[data['from']]['phases']} \
									but {data['to']} has {self.objects[data['to']]['phases']}")
								break
					for phase in data["phases"].replace("N",""):
						if phase not in self.objects[data["from"]]["phases"] and phase not in self.objects[data["to"]]["phases"]:
							warning(f"{cyme_mdbname}@{network_id} phase dismatch: section {data['name']} has {data['phases']} \
								but {data['to']} has {self.objects[data['to']]['phases']} and {data['from']} has {self.objects[data['from']]['phases']}")
							break
				if target_node_name:
					target_node = self.objects[target_node_name]
					target_node_phases = target_node["phases"].replace("N","")
					for phase in data["phases"].replace("N",""):
						if phase not in target_node_phases:
							warning(f"{cyme_mdbname}@{network_id} phase dismatch: parent {target_node_name} has {target_node_phases} but child {name} has {data['phases']}")
							if phase_check_fix:
								self.objects[name]["phases"] = self.objects[name]["phases"].replace(phase,"")
								if len(self.objects[name]["phases"]) == 0:
									warning(f"{cyme_mdbname}@{network_id} phase problem: {name} phases is None")

	def voltage_checks(self, nominal_voltage): # Check transformer primary/secondary voltage
		check_done = False
		check_done_count = 0
		while not check_done:
			check_done = True
			check_done_count += 1
			if check_done_count > 1e8:
				error(f"the voltage_checks may suck in a dead loop.", 65)
			for name, data in self.objects.items():
				if "class" in data.keys() and (data["class"] == "transformer" or data["class"] == "single_transformer"):
					config_name = data["configuration"]
					config_data = self.objects[config_name]
					from_node_name = data["from"]
					to_node_name = data["to"]
					from_node_voltage = self.objects[from_node_name]["nominal_voltage"]
					to_node_voltage = self.objects[to_node_name]["nominal_voltage"]
					primary_voltage = config_data["primary_voltage"]
					secondary_voltage = config_data["secondary_voltage"]
					if from_node_voltage == "${GLM_NOMINAL_VOLTAGE}" and primary_voltage.replace("kV","") != nominal_voltage:
						warning(f"{cyme_mdbname}@{network_id} voltage mismatch: transformer {name} has primary voltage as {primary_voltage} but node {from_node_name} nominal voltage is {nominal_voltage}kV.")
						if voltage_check_fix:
							check_done = False
							self.objects[from_node_name]["nominal_voltage"] = primary_voltage
					elif from_node_voltage != "${GLM_NOMINAL_VOLTAGE}" and float(primary_voltage.replace("kV","")) != float(from_node_voltage.replace("kV","")):
						warning(f"{cyme_mdbname}@{network_id} voltage mismatch: transformer {name} has primary voltage as {primary_voltage} but node {from_node_name} nominal voltage is {from_node_voltage}.")
						if voltage_check_fix:
							check_done = False
							self.objects[from_node_name]["nominal_voltage"] = primary_voltage
					if to_node_voltage == "${GLM_NOMINAL_VOLTAGE}" and secondary_voltage.replace("kV","") != nominal_voltage:
						warning(f"{cyme_mdbname}@{network_id} voltage mismatch: transformer {name} has secondary voltage as {secondary_voltage} but node {to_node_name} nominal voltage is {nominal_voltage}kV.")
						if voltage_check_fix:
							check_done = False
							self.objects[to_node_name]["nominal_voltage"] = secondary_voltage
					elif to_node_voltage != "${GLM_NOMINAL_VOLTAGE}" and float(secondary_voltage.replace("kV","")) != float(to_node_voltage.replace("kV","")):
						warning(f"{cyme_mdbname}@{network_id} voltage mismatch: transformer {name} has secondary voltage as {secondary_voltage} but node {to_node_name} nominal voltage is {to_node_voltage}.")
						if voltage_check_fix:
							check_done = False
							self.objects[to_node_name]["nominal_voltage"] = secondary_voltage
				elif "from" in data.keys() and "to" in data.keys():
					from_node_name = data["from"]
					to_node_name = data["to"]
					from_node_voltage = self.objects[from_node_name]["nominal_voltage"]
					to_node_voltage = self.objects[to_node_name]["nominal_voltage"]
					if from_node_voltage != to_node_voltage:
						warning(f"{cyme_mdbname}@{network_id} voltage mismatch: node {from_node_name} is {from_node_voltage} but node {to_node_name} is {to_node_voltage}.")
						if voltage_check_fix:
							check_done = False
							self.objects[to_node_name]["nominal_voltage"] = self.objects[from_node_name]["nominal_voltage"]
				elif "parent" in data.keys() and data["class"] not in ["solar", "inverter"]:
					parent_name = data["parent"]
					parent_data = self.objects[parent_name]
					if data["nominal_voltage"] != parent_data["nominal_voltage"]:
						warning(f"{cyme_mdbname}@{network_id} voltage mismatch: parent {parent_name} has voltage as {parent_data['nominal_voltage']} but child {name} has {data['nominal_voltage']}.")
						if voltage_check_fix:
							check_done = False
							old_voltage = self.objects[name]["nominal_voltage"]
							self.objects[name]["nominal_voltage"] = parent_data["nominal_voltage"]
							if data["class"] == "load":
								warning(f"{cyme_mdbname}@{network_id}: load {fix_name(name)} nominal_voltage changes from {old_voltage} to {parent_data['nominal_voltage']}.")
								

	def object_checks(self): # Check conversion
		del_nom_volt_list = ['overhead_line', 'underground_line', 'regulator', 'transformer', 'switch', 'fuse', 'ZIPload', 'diesel_dg','triplex_line_conductor','recorder','inverter','solar','triplex_line']
		for name, data in self.objects.items():
			if not "name" in data.keys():
				warning("%s: object does not have a name, object data [%s]" % (self.filename,data))
			elif not "class" in data.keys():
				warning("%s: object '%s' does not have a class" % (self.filename,data["name"]))
			elif data["class"] in ["link","powerflow_object","line"]:
				print(self.objects[name])
				warning("%s: object '%s' uses abstract-only class '%s'" % (self.filename,data["name"],data["class"]))
			elif data["class"] in del_nom_volt_list and 'nominal_voltage' in data.keys():
				del self.objects[name]["nominal_voltage"]

	def name_check(self): # Check object name
		for name_compared, data_compared in self.objects.items():
			for name, data in self.objects.items():
				if name != name_compared:
					for key in data.keys():
						if data[key] == name_compared:
							data[key] = fix_name(name_compared)
			data_compared['name'] = fix_name(name_compared)

def cyme_extract_9(network_id,network,conversion_info):

	output_file_name = conversion_info['output_file_name']
	version = conversion_info['version']

	creation_time = int(network["CreationTime"])
	last_change = int(network["LastChange"])
	load_factor = float(network["LoadFactor"])
	if single_file:
		glmname = os.path.abspath(f"{output_folder}/{output_file_name}.glm")
	else:
		glmname = os.path.abspath(f"{output_folder}/{output_file_name}_{network_id}.glm")

	glm = GLM(glmname,"w")
	# glm.comment(f"Automatically generated by {git_project}/postproc/write_glm.py")

	# glm.blank()
	# glm.comment("","Application information","")
	# glm.define("APP_COMMAND",app_command)
	# glm.define("APP_VERSION",app_version)

	# glm.blank()
	# glm.comment("","Git information","")
	# glm.define("GIT_PROJECT",git_project)
	# glm.define("GIT_COMMIT",git_commit)
	# glm.define("GIT_BRANCH",git_branch)

	# glm.blank()
	# glm.comment("","GLM creation context","")
	# glm.define("GLM_PATHNAME",glmname)
	# glm.define("GLM_CREATED",dt.datetime.utcnow().isoformat())
	# glm.define("GLM_USER",os.getenv("USER"))
	# glm.define("GLM_WORKDIR",os.getenv("PWD"))
	# glm.define("GLM_LANG",os.getenv("LANG"))

	# # settings from model
	# glm.blank()
	# glm.comment("","CYME model information","")
	# glm.define("CYME_MDBNAME",cyme_mdbname)
	# glm.define("CYME_VERSION",version)
	# glm.define("CYME_CREATED",dt.datetime.fromtimestamp(creation_time).isoformat())
	# glm.define("CYME_MODIFIED",dt.datetime.fromtimestamp(last_change).isoformat())
	# glm.define("CYME_LOADFACTOR",load_factor)
	# glm.define("CYME_NETWORKID",network_id)

	# settings from config.csv
	glm.blank()
	glm.comment("","Settings from 'config.csv'","")
	define = settings["GLM_DEFINE"].split("=")
	if type(define) is list and len(define) > 1:
		glm.define(define[0].strip(),"=".join(define[1:]).strip())
	feeder_kVLN = feeder_voltage_find(network_id,cyme_table)
	if feeder_kVLN is not None:
		glm.comment("GLM_NOMINAL_VOLTAGE found in network MDB is used")
		settings["GLM_NOMINAL_VOLTAGE"] = feeder_kVLN + ' kV'
		glm.define("GLM_NOMINAL_VOLTAGE",settings["GLM_NOMINAL_VOLTAGE"])
	elif settings["GLM_NOMINAL_VOLTAGE"]:
		glm.comment("GLM_NOMINAL_VOLTAGE found in config.csv is used")
		glm.define("GLM_NOMINAL_VOLTAGE",settings["GLM_NOMINAL_VOLTAGE"])
		feeder_kVLN = fix_unit(settings["GLM_NOMINAL_VOLTAGE"], "kV")
	else:
		if settings["GLM_INCLUDE"]: # cannot verify setting in GLM_INCLUDE until run in gridlabd
			glm.ifndef("GLM_NOMINAL_VOLTAGE",lambda:glm.error("GLM_NOMINAL_VOLTAGE must be defined in either 'config.csv' or the GLM_INCLUDE file"))
		else:
			error("GLM_NOMINAL_VOLTAGE must be defined in either 'config.csv' or the GLM_INCLUDE file")
	default_load_voltage = re.match("\d+[\.]?[\d+]*", settings["GLM_NOMINAL_VOLTAGE"]).group(0)
	if settings["GLM_INCLUDE"]:
		for include in settings["GLM_INCLUDE"].split():
			glm.include(include.strip())
	else:
		glm.blank()
		glm.comment("","default clock settings","")
		glm.clock({"timezone":"PST+8PDT", "starttime":"2020-01-01T00:00:00+08:00", "stoptime":"2020-01-01T00:05:00+08:00"})

	glm.blank()
	glm.comment("","Modules","")
	glm.module("powerflow",{"solver_method":"NR"})
	glm.module("generators")

	node_dict = {}
	device_dict = {}
	node_links = {}

	all_section_device = table_find(cyme_table["sectiondevice"],NetworkId=network_id)
	all_section = table_find(cyme_table["section"],NetworkId=network_id)
	all_load = table_find(cyme_table["load"],NetworkId=network_id)
	all_node = table_find(cyme_table["node"],NetworkId=network_id)

	# node graph data
	if "nodetag" in cyme_table.keys():
		for index, node in table_find(cyme_table["nodetag"],NetworkId=network_id).iterrows():
			node_id = node['NodeId']
			node_dict[node_id] = [] # node dictionary
		for node_id, node in table_find(cyme_table["node"],NetworkId=network_id).iterrows():
			node_id = node['NodeId']
			node_links[node_id] = [] # incident links
	else:
		for index, node in table_find(cyme_table["node"],NetworkId=network_id).iterrows():
			node_id = node['NodeId']
			node_links[node_id] = [] # incident links
			node_dict[node_id] = [] # node dictionary

	glm.blank()
	glm.comment("","Objects","")

	# links
	for index, section in table_find(cyme_table["section"],NetworkId=network_id).iterrows():
		section_id = section['SectionId']
		links = glm.add("link",section_id,section, version=5020, node_links=node_links)
		if links:
			device_dict.update(links)

	# cyme_table["node"]
	for node_id in node_dict.keys():
		# only network node and substantiation will be added
		if table_find(cyme_table["node"],NodeId=node_id).iloc[0]["ComponentMask"] != "0":
			node_dict[node_id] = glm.add_node(node_id, node_links, device_dict, version=5020, node_info={"all_node":all_node})

	# overhead lines
	try:
		for cyme_id, cyme_data in table_find(cyme_table["overheadbyphase"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("overhead_line_phase", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "overheadbyphase".')

	# unbalanced overhead lines
	try:
		for cyme_id, cyme_data in table_find(cyme_table["overheadlineunbalanced"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("overhead_line_unbalanced", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table \'overheadlineunbalanced\'.')

	# overhead lines
	try:
		for cyme_id, cyme_data in table_find(cyme_table["overheadline"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("overhead_line", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "overheadline".')

	# underground lines
	try:
		for cyme_id, cyme_data in table_find(cyme_table["undergroundline"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("underground_line", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "undergroundline".')
	
	# load
	try:
		for cyme_id, cyme_data in table_find(cyme_table["customerload"],NetworkId=network_id).iterrows():
			section_id = all_section_device[all_section_device["DeviceNumber"] == cyme_data['DeviceNumber']]["SectionId"].values
			load_section = all_section[all_section["SectionId"] == section_id[0]]
			connection_type = int(all_load[all_load["DeviceNumber"] == cyme_data['DeviceNumber']]['ConnectionConfiguration'])
			cyme_id = cyme_data['DeviceNumber']
			glm.add("load", cyme_id, cyme_data, version=5200, node_info={"Node_Links":node_links, "Device_Dicts": device_dict, "load_section": load_section, "connection_type": connection_type, "all_node": all_node,"load_voltage" : default_load_voltage})
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "customerload".')

	# transformer
	try:
		for cyme_id, cyme_data in table_find(cyme_table["transformer"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("transformer", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "transformer".')

	# transformerbyphase
	try:
		for cyme_id, cyme_data in table_find(cyme_table["transformerbyphase"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("single_transformer", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "transformerbyphase".')

	# regulator
	try:
		for cyme_id, cyme_data in table_find(cyme_table["regulator"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("regulator", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "regulator".')

	# capacitor
	try:
		for cyme_id, cyme_data in table_find(cyme_table["shuntcapacitor"],NetworkId=network_id).iterrows():
			section_id = all_section_device[all_section_device["DeviceNumber"] == cyme_data['DeviceNumber']]["SectionId"].values
			cap_section = all_section[all_section["SectionId"] == section_id[0]]
			cyme_id = cyme_data['DeviceNumber']
			glm.add("capacitor", cyme_id, cyme_data, version=5020,node_info={"Node_Links":node_links, "Device_Dicts": device_dict, "cap_section": cap_section})
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "shuntcapacitor".')

	# switches
	try:
		for cyme_id, cyme_data in table_find(cyme_table["switch"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("switch", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "switch".')

	# breaker
	try:
		for cyme_id, cyme_data in table_find(cyme_table["breaker"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("breaker", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "breaker".')

	# recloser
	try:
		for cyme_id, cyme_data in table_find(cyme_table["recloser"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("recloser", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "recloser".')

	# fuse
	try:
		for cyme_id, cyme_data in table_find(cyme_table["fuse"],NetworkId=network_id).iterrows():
			cyme_id = cyme_data['DeviceNumber']
			glm.add("fuse", cyme_id, cyme_data, version=5020)
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "fuse".')

	# photovoltaic
	try:
		for cyme_id, cyme_data in table_find(cyme_table["photovoltaic"],NetworkId=network_id).iterrows():
			section_id = all_section_device[all_section_device["DeviceNumber"] == cyme_data['DeviceNumber']]["SectionId"].values
			pv_section = all_section[all_section["SectionId"] == section_id[0]]
			cyme_id = cyme_data['DeviceNumber']
			glm.add("photovoltaic", cyme_id, cyme_data, version=5020, node_info={"Node_Links":node_links, "Device_Dicts": device_dict, "pv_section": pv_section, "all_node": all_node})
	except Exception as err:
		exception_type = type(err).__name__
		warning(f'{cyme_mdbname}@{network_id}: ({exception_type}: {err}) cannot add GLM objects from cyme_table "photovoltaic".')

	# network senity checks
	node_dict = glm.node_checks(node_dict,node_links,device_dict,version=5200,node_info={"all_node":all_node})
	glm.link_checks()
	glm.section_checks()
	glm.phase_checks()
	glm.object_checks()
	glm.voltage_checks(feeder_kVLN)
	glm.name_check()

	# generate coordinate file
	if geodata_file:
		df_node = pd.DataFrame.from_dict(node_geodata)
		df_node = df_node.T
		df_node.drop(df_node[df_node[:]["NotworkID"] != network_id].index,inplace=True)
		df_node = df_node.drop(["NotworkID"], axis=1)
		df_node.to_csv(f'{output_folder}/{geodata_file}', index = False, header=True)

	glm.close()


def convert(input_file: str ,output_file: str =None, options: dict[str, Any]={}):
	"""Convert a CYME MDB file to GLM

	Parameters:
		input_file (str)	input MDB file name
		output_file (str)	output GLM file name
		options (dict)		options to define as globals in model
	"""

	global data_folder
	global input_folder
	global output_folder
	global network_select
	global equipment_file
	global config_file
	global modify_file
	global settings
	global cyme_table
	global cyme_equipment_table
	global default_load_voltage
	global network_id
	global cyme_mdbname
	global geodata_file
	global single_file
	global WARNING
	global DEBUG
	global QUIET
	global VERBOSE
	global voltage_check_fix
	global phase_check_fix
	global error_count
	global GLM_error_file
	global GLM_warning_file
	global GLM_output_file
	global node_geodata


	# Configure options and paths

	if options:
		for opt, arg in options.items():
			if opt in ("c","config"):
				if arg and arg != True:
					config_file = arg.strip()
				else:
					print(config)
			elif opt in ("t","cyme-tables"):
				print(" ".join(cyme_tables_required))
				sys.exit(0)
			elif opt in ("d", "data-dir"):
				data_folder = arg.strip()
			elif opt in ("m", "modify"):
				modify_file = arg.strip()
			elif opt in ("n", "network-ID"):
				# only extract the selected network
				network_select = arg.split(" ")
			elif opt in ("e", "equipment-mdb"):
				equipment_file = arg.strip()
			elif opt in ("C", "coordinateCSV"):
				geodata_file = arg.strip()
			else:
				error(f"{opt}={arg} is not a valid option", 5);
	input_folder = os.path.dirname(os.path.abspath(input_file))
	input_file_name = os.path.basename(input_file).split('.')[0]
	if output_file:
		output_folder = os.path.dirname(os.path.abspath(output_file))
		output_file_name = os.path.basename(output_file).split('.')[0]
	else:
		output_file_name = input_file_name
		output_folder = input_folder
		output_file = os.path.join(output_folder,output_file_name+'.glm')
	if not network_select:
		single_file = True

	# converter input MBD to CSV tables
	if not data_folder:
		data_folder = f"/tmp/gridlabd/mdb-cyme2glm/{input_file_name}"
	cyme_table = mdb2csv(input_file,data_folder,cyme_tables_required,'non-empty')
	cyme_mdbname = data_folder.split("/")[-1]
	if equipment_file != None: # if equipment MDB is provided
		equipment_data_folder = f'{data_folder}/cyme_equipment_tables'
		cyme_equipment_table = mdb2csv(equipment_file,equipment_data_folder,cyme_tables_required,'non-empty')
	else:
		cyme_equipment_table = {}

	# Load user configuration
	if config_file:
		settings = pd.read_csv(config_file, dtype=str,
			names=["name","value"],
			comment = "#",
			).set_index("name")
	else:
		default_config_data = {
			"name" : ["GLM_NOMINAL_VOLTAGE", "GLM_MODIFY", "GLM_WARNINGS"],
			"value" : ["2.40178 kV", "modify.csv", "stdout"]
		}
		settings = pd.DataFrame(data=default_config_data).set_index("name")
		warning(f"No configuration file, will use default configurations")
	for name, values in settings.iterrows():
		if name in config.index:
			config["value"][name] = values[0]
	settings = config["value"]

	GLM_output_file = open(settings["GLM_OUTPUT"],"w")
	GLM_error_file = open(settings["ERROR_OUTPUT"],"a")
	GLM_warning_file = open(settings["WARNING_OUTPUT"],"a")
	default_load_voltage = None
	voltage_check_fix = True if settings["GLM_VOLTAGE_FIX"].lower() == "true" else False
	phase_check_fix = True if settings["GLM_PHASE_FIX"].lower() == "true" else False
	WARNING = True if settings["WARNING"].lower() == "true" else False
	DEBUG = True if settings["DEBUG"].lower() == "true" else False
	QUIET = True if settings["QUIET"].lower() == "true" else False
	VERBOSE = True if settings["VERBOSE"].lower() == "true" else False


	# Prepare Cyme converter settings
	cyme_extracter = {
		"90000" : cyme_extract_9, # CYME version 9 database
		"82000" : cyme_extract_9, # CYME version 8 database
		"50401" : cyme_extract_9, # CYME version 5 database
	}
	cyme_extracter["default"] = cyme_extracter["90000"]

	version = cyme_table["schemaversion"].loc[0]['Version']
	network_count = 0
	conversion_info = {
		"input_folder" : input_folder,
		"output_file_name" : output_file_name,
		'version' : version,
	}

	# 
	for index, network in cyme_table["network"].iterrows():
		network_id = network['NetworkId']
		if not re.match(settings["GLM_NETWORK_MATCHES"],network_id):
			continue
		else:
			network_count += 1
		if network_select != None and network_id not in network_select:
			pass
		else:
			found = False
			for key, extractor in cyme_extracter.items():
				if key[0] == version[0]:
					extractor(network_id,network,conversion_info)
					found = True
			if not found:
				raise Exception(f"CYME model version {version} is not supported")



if __name__ == '__main__':
	testfile = 'autotest/IEEE-123-cyme.mdb'
	if os.path.exists(testfile):
		config.annotated_model = True
		convert(testfile)
