"""Fize danger forecast tool

SYNTAX

	$ gridlabd fire_danger -f|--forecast=DAYAHEAD -d|--date=YYYY-MM-DD -t|--type=TYPE

DESCRIPTION

Downloads the USGS fire danger DAYAHEAD forecast map for the date YYYY-MM-DD.
The map is stored in the CACHEDIR (by default `$GLD_ETC/usgs/firedanger/`)
using the name `forecast_DAYAHEAD_DATE.tif`.

The output is the full pathname where the data is stored.

Valid TYPE values are:

fpi - Fire Potential Index
lfp - Large Fire Probability
fsp - Fire Spread Probability
"""

import sys, os
import requests
from io import BytesIO
import zipfile
import datetime

CACHEDIR = "/usr/local/opt/gridlabd/current/share/gridlabd/usgs/firedanger"
USGSURL = "https://edcintl.cr.usgs.gov/downloads/sciweb1/shared/firedanger/download-tool/source_rasters/w{TYPE}-forecast-{DAYAHEAD}/emodis-w{TYPE}-forecast-{DAYAHEAD}_data_{DATE}_{DATE}.zip"

class FireDangerBadRequest(Exception):
	pass

class FireDangerInvalidOption(Exception):
	pass

class FireDangerMissingOption(Exception):
	pass

def get_data(dayahead,date,maptype,url=USGSURL,cachedir=CACHEDIR):

	filename = f"{cachedir}/{maptype}_{date}_{dayahead}.tif"
	if not os.path.exists(filename):
		usgsurl = url.format(DAYAHEAD=dayahead,DATE=date,TYPE=maptype)
		reply = requests.get(usgsurl,stream=True)
		if 200 <= reply.status_code < 300:

			archive = zipfile.ZipFile(BytesIO(reply.content),mode="r")

			os.makedirs(cachedir,exist_ok=True)
			cachename = f"emodis-w{maptype}_data_{date}_{date}.tiff"
			archive.extract(cachename,f"{cachedir}")
			os.rename(f"{cachedir}/{cachename}",filename)
		else:		
			raise FireDangerBadRequest(f"{usgsurl} (code {reply.status_code})")
	return filename

def main(args):

	if not args:
		print("Syntax: gridlabd fire_danger -f|--forecast=DAYAHEAD -d|--date=YYYY-MM-DD -t|--type=TYPE",file=sys.stderr)
		return

	DAYAHEAD = None
	DATE = None
	TYPE = None
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
			value = "=".join(spec[1:])
		if tag in ["-d","--date"]:
			DATE = value
		elif tag in ["-f","--forecast"]:
			DAYAHEAD = value
		elif tag in ["-t","--type"]:
			TYPE = value
		elif tag in ["-h","--help","help"]:
			print(__doc__,file=sys.stdout)
		else:
			raise FireDangerInvalidOption(arg)

	if not DAYAHEAD:
		raise FireDangerMissingOption("-f|--forecast=DAYAHEAD")
	
	if not DATE:
		raise FireDangerMissingOption("-d|--date=YYYY-MM-DD")

	if not TYPE:
		raise FireDangerMissingOption("-t|--type=TYPE")

	for day in DAYAHEAD.split(","):
		for date in DATE.split(","):
			for mtype in TYPE.split(","):
				filename = get_data(day,datetime.datetime.strptime(date,"%Y-%m-%d").strftime("%Y%m%d"),mtype)
				print(filename,file=sys.stdout)

if __name__ == "__main__":

	BASENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
	try:
		main(sys.argv[1:])
	except Exception as err:
		etype,evalue,etrace = sys.exc_info()
		print(f"ERROR [{BASENAME}]: {etype.__name__} {evalue}",file=sys.stderr)
