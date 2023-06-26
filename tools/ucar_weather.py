"""UCAR weather access module"""
import requests
import re
from metar import Metar

debug = False

def get(station):
	"""Get current conditions at a station

	PARAMETERS

		station    ICAO station id (see `stations()` for details)

	RETURNS

		dict       Observation data
	"""
	query = f"http://weather.rap.ucar.edu/surface/index.php?metarIds={station}&hoursStr=most+recent+only&std_trans=standard&num_metars=number&submit_metars=Retrieve"
	r = requests.get(query)
	if r.status_code != 200:
		if debug:
			raise Exception(f"request error (code={r.status_code}), query='{query}'")
		else:
			raise Exception("request error (code={r.status_code})")
	match = re.search(f"{station} (.[^<]*)",r.text)
	if not match:
		if debug:
			raise Exception(f"request error (no match for station '{station}'), query='{query}', response='{r.text}'")
		else:
			raise Exception("request error (no match)")
	data = match.group(0)[:-1]
	obs = Metar.Metar(f'METAR {data}')
	if not obs:
		if debug:
			raise Exception("request error (no observation)")
		else:
			raise Exception(r"request error (no observation),, query='{query}',' response='{r.text}'")
	result = {
		"station" : station,
		"temperature" : f"{obs.temp.value()} degC",
		"dew_point" : f"{obs.dewpt.value()} degC",
		"wind_speed" : f"{obs.wind_speed.value()} knot",
		"wind_dir" : f"{obs.wind_dir.value()} deg",
		"visibility" : f"{obs.vis.value()} mile",
		"pressure" : f"{obs.press.value()} inH2O",
		"clouds" : obs.sky_conditions(),
	}
	result["metar"] = data.replace("\n"," ")
	return result

def stations():
	"""Get list of stations

	RETURNS

		dict   Station data by ICAO station id
	"""
	query = "https://www.aviationweather.gov/docs/metar/stations.txt"
	r = requests.get(query)
	if r.status_code != 200:
		raise Exception(f"request error (code={r.status_code})")
	result = {}
	linenum = 0
	section = None
	for line in r.text.split("\n"):
		linenum += 1
		if not line:
			section = None
		elif line[0] != '!':
			if not section:
				section = line
			elif line[3] != ' ':
				try:
					region = line[0:2].strip()
					if not region:
						region = None
					station = line[3:19].strip()
					icao = line[20:24].strip()
					if not icao:
						continue
					iata = line[26:29].strip()
					try:
						synop = int(line[32:37])
					except:
						synop = None
					lat = round(float(line[39:41])+float(line[42:44])/60,2)
					if line[44] == 'S': lat = -lat
					elif line[44] != 'N': raise Exception(f"latitude invalid")
					lon = round(float(line[47:50]) + float(line[51:53])/60,2)
					if line[53] == 'W': lon = -lon
					elif line[53] != 'E': raise Exception(f"longitude invalid")
					elev = float(line[55:59])
					country = line[80:83].strip()
				except Exception as err:
					print(f"stations.txt({linenum}): '{line}': {err}")
					raise
				if country:
					result[icao] = dict(
						country = country,
						region = region,
						station = station,
						latitude = lat,
						longitude = lon,
						# linenum = linenum,
						elevation = elev,
					)

	return result
