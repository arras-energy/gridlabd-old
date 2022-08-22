#!/bin/bash
"exec" "`dirname $0`/python3" "$0" "$@"

import sys
sys.path.append(sys.argv[0].split()[0:-1])
import ucar_weather
import datetime

station_list = []
station_data = ucar_weather.stations()
syntax = "python3 -m metar2glm [--index|STATION ...]"

n = 1
while n < len(sys.argv):
	if sys.argv[n] in ["-h","--help","help"]:
		print(f"Syntax: {syntax}")
		exit(0)
	elif sys.argv[n] in ["--index"]:
		print("\n".join(station_data.keys()))
		exit(0)
	else:
		station_list.append(sys.argv[n])
	n += 1
if not station_list:
	print(f"Syntax: {syntax}")
	exit(1)

unit_conversion = {
	"C" : "degC",
	"KT" : "knot",
	"M" : "m",
	"mb" : "mbar",
	"deg" : "deg",
}
print(f"// generated by metar2glm.py on {datetime.datetime.utcnow().isoformat()}Z")
print("""
class weather
{
	char8 country;
	char8 region;
	char32 station;
	double elevation[ft];
	timestamp time;
	double temperature[degF];
	double dew_point[degF];
	double wind_speed[m/s];
	double wind_dir[deg];
	double visibility[mile];
	double pressure[mbar];
	char256 clouds;
	char1024 metar;
	on_init "python:metar_weather.weather_init";
}
""")
for station in station_list:
	if not station in station_data.keys():
		raise Exception(f"station '{station}' not found in metar index")
	weather = ucar_weather.get(station)
	print("object weather")
	print("{")
	for attr,data in station_data[station].items():
		print(f"    {attr} \"{data}\";");
	for attr,data in weather.items():
		if not data or attr == "type":
			continue
		if attr == "station":
			attr = "name"
		if type(data) is dict and "value" in data.keys():
			if "unit" in data.keys():
				print(f"    {attr} {data['value']} {unit_conversion[data['unit']]};")
			elif data['value']:
				print(f"    {attr} \"{data['value']}\";")
		else:
			print(f"    {attr} \"{data}\";")
	print("}")