"""Fire Incident Report Tool
"""

import sys, os
import pandas
import requests
import re

class FireIncidentReportFailed(Exception):
	pass

class FireInvalidCommandOption(Exception):
	pass

class FireMissingCommandOption(Exception):
	pass

class FireMissingData(Exception):
	pass

ETC = os.getenv("GLD_ETC")
if not ETC:
	ETC = "."
if not ETC.endswith("/"):
	ETC += "/"
CACHE = f"{ETC}/fire_report/"
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
OUTPUT = sys.stdout

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
	data = pandas.read_excel(filename,engine=engine,skiprows=skiprows,**kwargs)
	try:
		columns = list(data.columns)
		datecol = columns.index("Date")
		timecol = columns.index("Time")
		latcol = columns.index("Latitude")
		loncol = columns.index("Longitude")
	except Exception as err:
		raise FireMissingData(err) from Exception
	data.drop(data.columns[0:min([datecol,timecol,latcol,loncol])],axis=1,inplace=True)
	return data

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
		data.columns = [re.sub(r'[^a-z0-9]+','_',col.lower()) for col in list(data.columns)]
		data.to_csv(csvname,index=False)

	data = pandas.read_csv(csvname,index_col=0,parse_dates=[[0,1]])
	data.index.name = "datetime"
	data.dropna(how="all",inplace=True)

	data.to_csv(OUTPUT)

if __name__ == "__main__":

	main(sys.argv[1:])
