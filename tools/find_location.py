# gridlabd find_location [OPTIONS] KEY
"""Find_location -- find the latitude and longitude of a location

SYNOPSIS
--------

Shell:

  $ gridlabd find_location [OPTIONS] KEY

GLM:

  #define LOCATION=${shell $(bindir)/python3 -m find_location PDX}

Python:

  >>> import sys, os
  >>> sys.path.append(os.getenv("GLD_ETC"))
  >>> import find_location
  >>> find_location.OUTPUT=None
  >>> find_location.main("PDX")
        latitude  longitude
  IATA                     
  PDX     45.589   -122.597

OPTIONS
-------

-d|--debug      enable debug output on error
-h|--help|help  display this help
-k|--key        change the lookup key (default is "IATA")
-q|--quiet      silence error output messages
--show_fields   show additional location fields (default None)
-v|--verbose    enable verbose output
-w|--warning    disable warning output
--with_header   output CSV with header row
--with_index    output CSV with index column

DESCRIPTION
-----------

The `find_location` tool find the latitude and longitude of a location. The
locations are based on airports. If you need to find a location for an address
use the "address" geodata package.

Value keys are `IATA`, `ICAO`, `NAME`, `CITY`, and `COUNTRY`. Valid fields are
`ICAO`, `IATA`, `NAME`, `CITY`, `COUNTRY`, `LATDEG`, `LATMIN`, `LATSEC`, `LATDIR`,
`LONDEG`, `LONMIN`, `LONSEC`, `LONDIR`, and `ELEVATION`.

EXAMPLE
-------

$ gridlabd find_location ORD,PDX --with_index --with_header --show_fields=City,Country
IATA,latitude,longitude,City,Country
ORD,41.979,-87.904,CHICAGO,USA
PDX,45.589,-122.597,PORTLAND,USA

SEE ALSO
--------

* Geodata address
"""
import os, sys
import requests
import pandas
import zipfile
import io

CACHEDIR = os.getenv("GLD_ETC")
if not CACHEDIR: 
	CACHEDIR = "/tmp"

VERBOSE = False
QUIET = False
DEBUG = False
WARNING = False
WITH_HEADER = False
WITH_INDEX = False
SHOW_FIELDS = ['latitude','longitude']
KEY_NAME = "IATA"

URL_SOURCE = "https://www.partow.net/downloads/GlobalAirportDatabase.zip"
OUTPUT = sys.stdout
ERROR = sys.stderr
DATABASE = None

class FindLocationError(Exception):
	pass

def error(msg):
	if not QUIET:
		print(f"ERROR [find_location]: {msg}",flush=True,file=ERROR)

def verbose(msg):
	if VERBOSE:
		print(f"VERBOSE [find_location]: {msg}",flush=True,file=ERROR)

def get_data():
	"""Load airport database"""
	global CACHEDIR
	global URL_SOURCE
	if not os.path.exists(CACHEDIR+"/airports.zip"):
		verbose(f"downloading data from {URL_SOURCE}")
		content = requests.get(URL_SOURCE).content
		with open(CACHEDIR+"/airports.zip","wb") as fh:
			fh.write(content)

	if not os.path.exists(CACHEDIR+"/GlobalAirportDatabase.txt"):
		verbose(f"extracting data from database")
		file = zipfile.ZipFile(CACHEDIR+"/airports.zip")
		file.extract("GlobalAirportDatabase.txt",CACHEDIR)

	data = pandas.read_csv(CACHEDIR+"/GlobalAirportDatabase.txt",
		sep=':',
		header=None,
		names=['ICAO','IATA','NAME','CITY','COUNTRY','LATDEG','LATMIN','LATSEC','LATDIR','LONDEG','LONMIN','LONSEC','LONDIR','ELEVATION','latitude','longitude'])

	return data

def get_location(key,value,data=None):
	"""Get location information

Arguments

	key - the index name to search on (see KEY_NAME)

	value - the value to search for in the index

	data - the database to search (default is airport database)

Return Value

	DataFrame - location data (latitude, longitude, and any fields listed in SHOW_FIELDS)
"""
	global DATABASE
	if not data:
		data = DATABASE
	if type(value) is list:
		return data[data[key].str.lower().isin([x.lower() for x in value])][[key,*SHOW_FIELDS]].round(5).set_index(key)
	else:
		return data[data[key].str.lower()==value.lower()][[key,*SHOW_FIELDS]].round(5).set_index(key)

def main(*argv):
	"""main function

Arguments

	argv - argument list

Return Value

	DataFrame - Result if OUTPUT is None

	None - if OUTPUT is 
"""
	if not argv:
		argv = sys.argv[1:]
	global DATABASE
	global TARGET
	global KEY_NAME
	global WITH_INDEX
	global WITH_HEADER
	global OUTPUT
	TARGET = []
	for arg in argv:
		specs = arg.split("=")
		if len(specs) == 1:
			tag = specs[0]
			value = None
		elif len(specs) == 2:
			tag = specs[0]
			value = specs[1]
		else:
			tag = specs[0]
			value = specs[1:]
		if tag in ["-h","--help","help"]:
			print(__doc__)
			return
		elif tag in ["-v","--verbose"]:
			global VERBOSE
			VERBOSE = True
		elif tag in ["-d","--debug"]:
			global DEBUG
			DEBUG = True
		elif tag in ["-q","--quiet"]:
			global QUIET
			QUIET = True
		elif tag in ["-w","--warning"]:
			global WARNING
			WARNING = True
		elif tag in ["--with_header"]:
			WITH_HEADER = True
		elif tag in ["--with_index"]:
			WITH_INDEX = True
		elif tag in ["--show_fields"]:
			global SHOW_FIELDS
			SHOW_FIELDS.extend(value.split(","))
		elif tag in ["-k","--key"]:
			KEY_NAME = value
		elif tag in ["-o","--output"]:
			if value:
				OUTPUT = open(value,"wt")
			else:
				OUTPUT = None
		elif tag.startswith("-"):
			raise FindLocationError(f"option '{arg}' is invalid")
		else:
			TARGET.extend(arg.split(","))
	
	if not TARGET:
		raise FindLocationError("city/airport name not specified")
	else:
		global DATABASE
		DATABASE = get_data()
		return get_location(KEY_NAME,TARGET)

if __name__ == "__main__":

	try:

		if len(sys.argv) == 1:
			print("Syntax: find_location NAME",file=ERROR)
		else:
			result = main()
			if result:
				result.to_csv(OUTPUT,header=WITH_HEADER,index=WITH_INDEX)

	except Exception as err:

		e_type, e_value, e_trace = sys.exc_info()
		e_file = os.path.basename(e_trace.tb_frame.f_code.co_filename)
		e_line = e_trace.tb_lineno
		error(f"{err} ({e_type.__name__} {e_value}@{e_file}:{e_line}) ")
		if DEBUG:
			raise
