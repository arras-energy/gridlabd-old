"""Convert XLS worksheets to CSV tables

SYNOPSIS

	Shell:
		$ gridlabd convert WORKBOOK.xls TABLE.csv -f xlsx-workbook -t csv-table [OPTIONS ...]

	GLM:
		#convert WORKBOOK.xls TABLE.csv -f xlsx-workbook -t csv-table [OPTIONS ...]

DESCRIPTION

	This converter extracts worksheets in an XLS workbook to CSV tables. If more
	than one worksheet is found and more than one worksheet matches table.pattern
	option, then the name TABLE is used as a root and the worksheet name is appended 
	to the CSV filename when saving each worksheet.

	The read.converters option may be used to specify converters for individual columns
	of worksheets.  The converters are specified in a JSON file as a dict where the 
	original column name is specified and a gridlabd data type is associated, i.e.,
	"gridlabd.double", "gridlabd.complex", "gridlabd.int64", "gridlabd.int32", 
	"gridlabd.int16", or "gridlabd.timestamp". You may also specify python or numpy
	data types, e.g., "float" or "numpy.float64", etc.  Failsafe converters are provided
	by "failsafe.float" by "failsafe.int". These return invalid.float and invalid.int
	respectively, if the data conversion fails.

OPTIONS:

	table.fixnames {<regex>,<bool>} (default False)

		Removes characters matching the pattern. If True is used the pattern
		[^A-Za-z0-9] is used.

	table.pattern <regex> (default None)

		Saves only worksheets when names that match the pattern. None saves 
		all worksheets found. Only used when more than one worksheet is found.

	invalid.float <str> (default 'nan')

	glm.class <str> (default None)

		Class to use when outputing the GLM file.  If None is used, no GLM
		file is output.

	read options (see pandas read_excel options):

		read.sheet_name <str> (default None)
		read.header <int> (default 0)
		read.names <str>[,...] (default None)
		read.index_col <int>[,...] (default None)
		read.usecols <int>[,...] (default None)
		read.skiprows <int> (default 0)
		read.nrows <int> (default None)
		read.parse_dates <bool> (default False)
		read.engine <str> (default openpyxl)
		read.converters <file> (default None)

	write options (see pandas dataframe to_csv options):

		write.header <bool> (default True)
		write.index <bool> (default False)
"""
import os, sys
import pandas as pd 
import warnings
import re
import json
import numpy

class Xlsx2csvConverter(Exception):
	pass

class failsafe:
	def float(x):
		try: 
			return float(x)
		except: 
			return float(default_options["invalid"]["float"])
	def int(x):
		try: 
			return int(x)
		except: 
			return int(default_options["invalid"]["int"])

default_options = {
	"table" : {
		"fixnames" : False,
		"pattern" : None,
	},
	"glm" : {
		"class" : None,
	},
	"datetime" : {
		"read" : "%Y-%m-%d %H:%M:%S %Z",
	},
	"invalid" : {
		"float" : 'nan',
		"int" : '-1',
	},
	"read" : {
		"sheet_name" : None,
		"header" : 0,
		"names" : None,
		"index_col" : None,
		"usecols" : None,
		"skiprows" : 0,
		"nrows" : None,
		"parse_dates" : False,
		"engine" : "openpyxl",
		"converters" : None, # TODO
	},
	"write" : {
		"header" : True,
		"index" : False,
	}
}

excel_types = {
	"float64" : "double",
	"int64" : "int64",
	"datetime64[ns]" : "timestamp",
}

def convert(input_file, output_file, options={}):

	# check input
	if not input_file.endswith(".xlsx"):
		raise Xlsx2csvConverter(f"input file '{input_file}' must have xlsx extension")

	# check output
	if not output_file.endswith(".csv"):
		raise Xlsx2csvConverter(f"output file '{output_file}' must have csv extension")

	# check options
	for key,value in options.items():
		spec = key.split(".")
		if len(spec) != 2 or not spec[1] in default_options[spec[0]].keys():
			raise Xlsx2csvConverter(f"option '{key}={value}' is not valid")
		if value == None:
			value = ["True"]
		elif not type(value) is str:
			value = [str(value)]
		elif "," in value:
			value = value.split(",")
		else:
			value = [value]

		# retype data
		def autotype(x):
			if x.lower() == "true":
				return True
			if x.lower() == "false":
				return False
			if x.lower() == "none":
				return None
			try:
				return int(x)
			except:
				pass
			try:
				return float(x)
			except:
				pass
			return x
		default_options[spec[0]][spec[1]] = [autotype(x) for x in value]

		# collapse non-str lists of len 1
		if len(default_options[spec[0]][spec[1]]) == 1 and not type(default_options[spec[0]][spec[1]]) is str:
			default_options[spec[0]][spec[1]] = default_options[spec[0]][spec[1]][0]

	# setup converters if needed
	if default_options["read"]["converters"]:
		with open(default_options["read"]["converters"],"r") as fh:
			import gridlabd
			default_options["read"]["converters"] = {}
			for key,value in json.load(fh).items():
				default_options["read"]["converters"][key] = eval(value)

	# read input
	with warnings.catch_warnings(record=True):
		warnings.simplefilter("always")
		book = pd.read_excel(input_file,**default_options["read"])

	# fix names
	def typeof(df,col):
		try:
			return excel_types[str(df.dtypes[col])]
		except:
			return "string"

	def writecsv(book,file,**options):
		fixed = []
		if options["table"]["fixnames"]:
			if type(options["table"]["fixnames"]) is str:
				fixpattern = options["table"]["fixnames"]
			else:
				fixpattern = r'[^a-zA-Z0-9_]'
			book.columns = [re.sub(re.compile(fixpattern),'',x) for x in book.columns]
		book.to_csv(file,**options["write"])
		if options["glm"]["class"]:
			glmname = file[:-4]+".glm"
			with open(glmname,"w") as glm:
				glm.write(f"class {options['glm']['class']} {{\n")
				for name in book.columns:
					glm.write(f"\t{typeof(book,name)} {name};\n")
				glm.write("}\n")

	# write output
	if len(book.keys()) > 1:
		if options["table"]["pattern"]:
			pattern = re.complex(options["table"]["pattern"])
		else:
			pattern = None
		for name,sheet in book.items():
			if pattern == None or re.match(pattern,name):
				file = output_file.replace(".csv","_"+name+".csv")
				writecsv(sheet,file,**default_options)
	else:
		name = list(book.keys())[0]
		writecsv(book[name],output_file,**default_options)
