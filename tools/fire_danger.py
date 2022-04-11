"""Fize danger forecast tool
"""

import sys, os
import requests
from io import BytesIO
import zipfile
import datetime

CACHEDIR = "/usr/local/opt/gridlabd/current/share/gridlabd/usgs/firedanger/"
USGSURL = "https://edcintl.cr.usgs.gov/downloads/sciweb1/shared/firedanger/download-tool/source_rasters/wfpi-forecast-{DAYAHEAD}/emodis-wfpi-forecast-{DAYAHEAD}_data_{DATE}_{DATE}.zip"

class FireDangerBadRequest(Exception):
	pass

class FireDangerInvalidOption(Exception):
	pass

class FireDangerMissingOption(Exception):
	pass

def get_data(dayahead,date,url=USGSURL,cachedir=CACHEDIR):

	usgsurl = url.format(DAYAHEAD=dayahead,DATE=date)
	reply = requests.get(usgsurl,stream=True)
	if 200 <= reply.status_code < 300:

		archive = zipfile.ZipFile(BytesIO(reply.content),mode="r")

		os.makedirs(cachedir,exist_ok=True)
		archive.extract(f"emodis-wfpi_data_{date}_{date}.tiff",f"{cachedir}")
		os.rename(f"{cachedir}/emodis-wfpi_data_{date}_{date}.tiff",f"{cachedir}/forecast_{date}_{dayahead}.tif")
	
	else:
	
		raise FireDangerBadRequest(f"{usgsurl} (code {reply.status_code}")

def main(args):

	DAYAHEAD = None
	DATE = None
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
			DATE = datetime.datetime.strptime(value,"%Y-%m-%d")
		elif tag in ["-f","--forecast"]:
			DAYAHEAD = int(value)
		else:
			raise FireDangerInvalidOption(arg)

	if not DAYAHEAD:
		raise FireDangerMissingOption("-f|--forecast=DAYAHEAD")
	
	if not DATE:
		raise FireDangerMissingOption("-d|--date=YYYY-MM-DD")

	get_data(str(DAYAHEAD),DATE.strftime("%Y%m%d"))


if __name__ == "__main__":

	BASENAME = sys.argv[0]
	main(sys.argv[1:])