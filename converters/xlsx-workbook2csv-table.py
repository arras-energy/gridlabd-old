"""Convert XLS worksheets to CSV tables

SYNOPSIS

	GLM:
		#convert WORKBOOK.xls TABLE.csv -f workbook -t table [-s|--save PATTERN]

DESCRIPTION

	This converter extracts worksheets in an XLS workbook to CSV tables. If more
	than one worksheet if found and more than one worksheet matches PATTERN, then
	the name TABLE is used as a root and the worksheet name is appended to the
	CSV filename when saving each worksheet.

OPTIONS:

	-s|--save PATTERN   save only worksheets with matching names using regex
	
	table.fixnames <bool>
	
	glm.class <str> (default None)

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

	write options (see pandas dataframe to_csv options):

		write.header <bool> (default True)
		write.index <bool> (default False)
"""
import os, sys
import pandas as pd 
import warnings
import re

class Xlsx2csvConverter(Exception):
	pass

default_options = {
	"table" : {
		"fixnames" : False,
	},
	"glm" : {
		"class" : None,
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
		if len(spec) == 1:
			spec.append(True)
		if not spec[1] in default_options[spec[0]].keys():
			raise Xlsx2csvConverter(f"option '{key}={value}' is not valid")
		if "," in value:
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
			book.columns = [re.sub(re.compile(r'[^a-zA-Z0-9_]'),'',x) for x in book.columns]
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
		for name,sheet in book.items():
			file = output_file.replace(".csv","_"+name+".csv")
			writecsv(sheet,file,**default_options)
	else:
		name = list(book.keys())[0]
		writecsv(book[name],output_file,**default_options)

if __name__ == "__main__":
	convert("/tmp/test.xlsx","/tmp/test.csv")
