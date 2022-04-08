"""Fire Incident Report Tool
"""

import sys, os
import pandas
import requests

class FireIncidentReportFailed(Exception):
	pass

class FireInvalidCommandOption(Exception):
	pass

class FireMissingCommandOption(Exception):
	pass

CACHE = ".cache"
URLINFO = {
	"CA" : {
		"PGE" : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-and-enforcement-division/reports/fire-incidents/{YEAR}_pge-fire-incident-data-collection-report.xlsx",
		"SCE" : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-and-enforcement-division/reports/fire-incidents/{YEAR}_sce-fire-incident-data-collection-report.xlsx",
		"SDGE" : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-and-enforcement-division/reports/fire-incidents/sdge-cpuc-reportable-ignitions-for-{YEAR}.xlsx",
	},
}

YEAR = None
STATE = None
UTILITY = None
OUTPUT = None

def get_data(state,utility,year,engine="openpyxl",skiprows=1,**kwargs):
	"""Get fire incident report data

	ARGUMENTS

	state (str): state from which data is retrieved (see URLINFO.keys() for a list)

	utility (str): utility from which data is retrieve (see URLINFO["STATE"].keys() for a list)

	year (int or str): year for which data is retrieve (varies by utility)

	RETURNS

	dataframe: raw data
	"""
	url = URLINFO[state][utility].format(YEAR=year)
	filename = f"{CACHE}/{os.path.basename(url)}"
	if not os.path.exists(filename):
		response = requests.get(url, stream=True)
		if not 200 <= response.status_code <= 299:
			raise FireIncidentReportFailed(response.text)
		with open(filename,"wb") as fh:
			for chunk in response.iter_content(chunk_size=1024):
				fh.write(chunk)
	return pandas.read_excel(filename,engine=engine,skiprows=skiprows,**kwargs).drop("Unnamed: 0",axis=1)

def main(args):

	global STATE
	global YEAR
	global UTILITY
	global OUTPUT
	for arg in args:
		spec = arg.split("=")
		if len(spec) == 1:
			tag = arg
			value = True
		elif len(spec) == 2:
			tag = spec[0]
			value = spec[1]
		else:
			tag = spec[0]
			value = spec[1:]
		if tag in ["-y","--year"]:
			YEAR = value
		elif tag in ["-u","--utility"]:
			UTILITY = value
		elif tag in ["-s","--state"]:
			STATE = value
		elif tag in ["-o","--output"]:
			OUTPUT = open(value,"w")
		else:
			raise FireInvalidCommandOption(arg)	

	if not YEAR:
		raise FireMissingCommandOption("year")

	if not STATE:
		raise FireMissingCommandOption("state")

	if not UTILITY:
		raise FireMissingCommandOption("utility")

	os.makedirs(CACHE,exist_ok=True)

	csvname = f"{CACHE}/{STATE}_{UTILITY}_{YEAR}.csv"

	if not os.path.exists(csvname):

		data = get_data(STATE,UTILITY,YEAR)
		columns = []
		for column in data.columns:
			columns.append(column.lower().replace(" ","_"))
		data.columns = columns

		data.to_csv(csvname,index=False)

	data = pandas.read_csv(csvname,index_col=0,parse_dates=[[0,1]])
	data.index.name = "datetime"

	data.to_csv(OUTPUT)

if __name__ == "__main__":

	main(sys.argv[1:])
