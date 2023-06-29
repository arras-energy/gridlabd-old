"""ISO New England data access module

Syntax: gridlabd isone DATASET[=SUBSET] [STARTDATE [STOPDATE]]

Currently supported dataset is only `smd`.  Startdate alone can be given as a
year. Otherwise start and stop days must provided.
"""

import sys, os
import datetime
import pandas

STARTDATE = None
STOPDATE = None
CACHEDIR = "/usr/local/opt/gridlabd/current/share/gridlabd/isone/" if not "GLD_ETC" in os.environ else f"{os.environ['GLD_ETC']}/isone"
DATETIMEFORMAT = "%Y-%m-%d"
AREA = "ISO NE CA"

E_OK = 0
E_SYNTAX = 1
E_INVALID = 2
E_FAILED = 3

def error(msg,code=None):
	print(f"ERROR: {msg}",flush=True,file=sys.stderr)
	if not code is None:
		exit(code)

def get_year(year=None,area=None):
	if year == None:
		year = datetime.datetime.now().year
	if area == None:
		area = AREA
	csvfile = os.path.join(CACHEDIR,f"smd_{year}_{area.replace(' ','_')}.csv")
	if os.path.exists(csvfile):
		return pandas.read_csv(csvfile,index_col=[0,1],parse_dates=[0])
	else:
		os.makedirs(CACHEDIR,exist_ok=True)
		try:
			url = f"https://www.iso-ne.com/static-assets/documents/{year}/02/{year}_smd_hourly.xlsx"
			data = pandas.read_excel(url,sheet_name=area,index_col=[0,1])
		except Exception as err:
			error(f"unable to get data from '{url}' ({err})",E_FAILED)
		data.to_csv(csvfile,index=True,header=True)
		return data

def get_data(startdate=None,stopdate=None):
	if startdate == None:
		startdate = datetime.datetime(year=datetime.datetime.now().year)
	else:
		startdate = datetime.datetime.strptime(startdate,DATETIMEFORMAT)
	if startdate == None:
		stopdate = datetime.datetime.now()
	else:
		stopdate = datetime.datetime.strptime(stopdate,DATETIMEFORMAT)
	if startdate > stopdate:
		error("startdate is after stopdate",E_INVALID)
	data = []
	for year in range(startdate.year,stopdate.year+1):
		data.append(get_year(year))
	data = pandas.concat(data)
	return data.loc[pandas.date_range(startdate,stopdate)]

if len(sys.argv) == 1:
	for line in __doc__.split("\n"):
		if line.startswith("Syntax: "):
			print(line,file=sys.stderr)
			exit(E_SYNTAX)
elif sys.argv[1] in ["-h","--help","help"]:
	print(__doc__)
	exit(E_OK)
elif not sys.argv[1].startswith("smd"):
	error(f"'{sys.argv[1]}'' is not a valid dataset",E_INVALID)
elif "=" in sys.argv[1]:
		AREA = sys.argv[1].split("=")[1]

if len(sys.argv) == 2:
	STARTDATE = None
elif len(sys.argv) == 3:
	try:
		STARTDATE = int(sys.argv[2])
	except:
		error(f"startdate '{sys.argv[2]}' is not a valid year",E_INVALID)
elif len(sys.argv) == 4:
	STARTDATE = sys.argv[2]
	STOPDATE = sys.argv[3]
else:
	error("too many arguments",E_SYNTAX)

if STARTDATE and STOPDATE:
	print(get_data(STARTDATE,STOPDATE).to_csv(float_format="%.2f"))
else:
	print(get_year(STARTDATE).to_csv(float_format="%.2f"))
