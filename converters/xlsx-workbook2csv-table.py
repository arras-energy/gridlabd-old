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
"""
import os, sys
import pandas as pd 
import warnings

class Xlsx2csvConverter(Exception):
	pass

default_options = {
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

	# read input
	with warnings.catch_warnings(record=True):
		warnings.simplefilter("always")
		book = pd.read_excel(input_file,**default_options["read"])

	# write output
	if len(book.keys()) > 1:
		for name,sheet in book.items():
			file = output_file.replace(".csv","_"+name+".csv")
			sheet.to_csv(file,**default_options["write"])
	else:
		name = list(book.keys())[0]
		book[name].to_csv(output_file,**default_options["write"])


if __name__ == "__main__":

	convert("/tmp/test.xlsx","/tmp/test.csv")
