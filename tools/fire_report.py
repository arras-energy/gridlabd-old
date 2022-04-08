"""Fire Incident Report Tool

SYNOPSIS

Shell:

	$ gridlabd fire_report -s|--state=STATE -y|--year=YEARS -u|--utility=UTILITY [-o|--output=CSVFILE] [--refresh] [--help]

GLM:

	#fire_report -s|--state=STATE -y|--year=YEARS -u|--utility=UTILITY [-o|--output=CSVFILE] [--refresh] [--help]

Python:

	>>> import fire_report
	>>> fire_report.main(state=STATE,year=YEARS,utility=UTILITY,output=CSVFILE,refresh=BOOLEAN,help=BOOLEAN)

DESCRIPTION

The fire_report tool retrieve annual fire report data delivered to state
regulators.  The current only support California Public Utility Commission
reporting (state=CA) for Pacific Gas and Electric (utility=PGE), Southern
California Edison (utility=SCE), and San Diego Gas and Electric (utility=SDGE).

The data is available reported years, starting in 2014. The YEARS parameters may
be specified as a sequence of comma-delimited years or range of years, e.g.,
2014,2015,2018,2019,2020,2021 or 2014,2015,2018-2021.

EXAMPLE

The following downloads the complete fire report record for PG&E from 2014 through 2021:

$ gridlabd fire_report -y=2014-2021 -u=PGE -s=CA
datetime,latitude,longitude,material_at_origin,land_use_at_origin,size,suppressed_by,suppressing_agency,facility_identification,other_companies,voltage_volts_,equipment_involved_with_ignition,type,was_there_an_outage,date_1,time_1,suspected_initiating_event,equipment_facility_failure,contact_from_object,facility_contacted,contributing_factor,unnamed_23,voltage,material_at_origin_comments
2014-06-10 15:14.00,40.20599,-122.25333,Vegetation,Rural,< 0.25 Acres,Fire Agency,Red Bluff FD,101545144,,12000,Conductor,Overhead,Yes,2014-06-10 15:14:00,2014-06-10 15:14:00,Contact From Object,N.A.,Other,Electric Facility,Human Error,Conveyor belt contact,,
2014-06-12 06:30.00,38.04374999,-122.74623999999999,Vegetation,Rural,10 - 99 Acres,Fire Agency,Cal Fire et al,102230371,AT&T,12000,Conductor,Overhead,Yes,2014-06-12 06:30:00,2014-06-12 06:30:00,Equipment/ Facility Failure,Crossarm,N.A.,N.A.,Unknown,,,
2014-06-13 09:19.00,36.93347999,-121.388549,Vegetation,Rural,< 0.25 Acres,Fire Agency,Hollister FD,101703071,AT&T,21000,Conductor,Overhead,Yes,2014-06-13 09:19:00,2014-06-13 09:19:00,Contact From Object,N.A.,Vehicle,Pole,Human Error,,,
...
2021-12-24 03:20:00,37.148326700000005,-121.99329170000001,Vegetation,Conifer Forest,<3 meters of linear travel,Utility - PG&E,,100554483,N,,Conductor - Primary,Overhead,Y,2021-12-24,03:17:00,Contact from Object,,Vegetation,Conductor - Primary,,,12000,
2021-10-14 07:00.00,40.070370000000004,-122.2334,Other,Unknown,<0.25,Unknown,,,N,,Switch,Overhead,N,,,Equipment - PG&E,Unknown,,,,,60000,Pole
2021-06-28 07:18:00,38.9999673839,-122.75278890690001,Vegetation,Unknown,<0.25,Cal Fire,,102156370,Y,,Conductor - Open Wire Secondary,Overhead,Y,2021-06-28,07:18:00,Contact from Object,,Vegetation,Conductor - Open Wire Secondary,,,0-750,

SEE ALSO

* https://www.cpuc.ca.gov/industries-and-topics/wildfires
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
		"PGE" : {
			2014 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/pgefire-incident-data-collection2014-report20150401.xlsx",
			2015 :"https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/pgefire-incident-data-collection2015-report20160401.xlsx",
			2016 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/pgefire-incident-data-collection2016-report20170401.xlsx",
			2017 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/pgefire-incident-data-collection2017-report20180401.xlsx",
			2018 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/pgefire-incident-data-collection2018-report20190329.xlsx",
			2019 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/pgefire-incident-data-collection2019-report20200401.xlsx",
			2020 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/2020_pge-fire-incident-data-collection-report.xlsx",
			2021 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-and-enforcement-division/reports/fire-incidents/2021_pge-fire-incident-data-collection-report.xlsx",
		},
		"SCE" : {
			2014 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/scefire-incident-data-collection2014-report20150401.xlsx",
			2015 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/scefire-incident-data-collection2015-report20160401.xlsx",
			2016 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/scefire-incident-data-collection2016-report20170401.xlsx",
			2017 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/scefire-incident-data-collection2017-report20180401.xlsx",
			2018 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/scefire-incident-data-collection2018-report20190401.xlsx",
			2019 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/scefire-incident-data-collection2019-report20200401.xlsx",
			2020 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sce-fire-reporting-for-year-2020_final.xlsx",
			2021 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-and-enforcement-division/reports/fire-incidents/2021_sce-fire-incident-data-collection-report.xlsx",
		},
		"SDGE" : {
			2014 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdgefire-incident-data-collection2014-report20150401.xlsx",
			2015 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdgefire-incident-data-collection2015-report20160401.xlsx",
			2016 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdgefire-incident-data-collection2016-report20170401.xlsx",
			2017 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdgefire-incident-data-collection2017-report20180401.xlsx",
			2018 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdgefire-incident-data-collection2018-report20190401.xlsx",
			2019 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdgefire-incident-data-collection2019-report20200401.xlsx",
			2020 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-policy-division/reports/sdge-cpuc-reportable-ignitions-for-2020.xlsx",
			2021 : "https://www.cpuc.ca.gov/-/media/cpuc-website/divisions/safety-and-enforcement-division/reports/fire-incidents/sdge-cpuc-reportable-ignitions-for-2021.xlsx",
		},
	},
}

YEAR = None
STATE = None
UTILITY = None
REFRESH = False
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
	url = URLINFO[state][utility][int(year)]
	filename = f"{CACHE}/{os.path.basename(url)}"
	if not os.path.exists(filename) or REFRESH:
		response = requests.get(url, stream=True)
		if not 200 <= response.status_code <= 299:
			raise FireIncidentReportFailed(response.text)
		with open(filename,"wb") as fh:
			for chunk in response.iter_content(chunk_size=1024):
				fh.write(chunk)

	def from_xldate(d):
		try:
			return f"{d.year:04d}-{d.month:02d}-{d.day:02d}"
		except:
			return d
		
	def from_xltime(t):
		try:
			return f"{t.hour:02d}:{t.minute:02d}.{t.second:02d}"
		except:
			return t
	
	data = pandas.read_excel(filename,
		engine=engine,
		skiprows=skiprows,
		usecols=list(range(24)),
		converters = {
			"Date" : from_xldate,
			"Time" : from_xltime,
		},
		**kwargs)
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

def syntax():
	print("Syntax: gridlabd fire_report -s|--state=CA -y|--year=YEAR -u|--utility=UTILITY [-o|--output=CSVFILE] [--refresh] [--help]",file=sys.stderr)

def main(args=[],**kwargs):

	for key,value in kwargs.items():
		args.append(f"--{key}={value}")

	global STATE
	global YEAR
	global UTILITY
	global OUTPUT
	global REFRESH

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
		elif tag in ["-r","--refresh"]:
			REFRESH = True
		elif tag in ["-h","--help","help"]:
			print(__doc__,file=sys.stdout)
		else:
			raise FireInvalidCommandOption(arg)	

	if not YEAR:
		raise FireMissingCommandOption("year")

	if not STATE:
		raise FireMissingCommandOption("state")

	if not UTILITY:
		raise FireMissingCommandOption("utility")

	os.makedirs(CACHE,exist_ok=True)

	result = []
	for years in YEAR.split(","):
		startstop = years.split("-")
		if len(startstop) == 1:
			startstop = [int(startstop[0])]
		elif len(startstop) == 2:
			startstop = list(range(int(startstop[0]),int(startstop[1])+1))
		else:
			raise FireInvalidCommandOption(years)
		for year in startstop:
			csvname = f"{CACHE}/{STATE}_{UTILITY}_{year}.csv"
			if not os.path.exists(csvname) or REFRESH:
				data = get_data(STATE,UTILITY,year)
				data.columns = [re.sub(r'[^a-z0-9]+','_',col.lower()) for col in list(data.columns)]
				data.to_csv(csvname,index=False)
			data = pandas.read_csv(csvname,index_col=0,parse_dates=[[0,1]])
			data.index.name = "datetime"
			data.dropna(how="all",inplace=True)
			result.append(data)
	pandas.concat(result).to_csv(OUTPUT)

if __name__ == "__main__":

	if len(sys.argv) == 1:
		syntax()
	else:
		main(sys.argv[1:])
