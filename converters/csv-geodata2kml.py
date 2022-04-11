"""Convert CSV geodata to KML
"""
import sys, os, getopt
import pandas
import simplekml

EXENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
LATITUDE = "latitude"
LONGITUDE = "longitude"
NAME = "name"
CLASS = "class"

def error(msg):
    print(f'ERROR [{EXENAME}]: {msg}',file=sys.stderr)
    sys.exit(1)

def warning(msg):
    print(f'WARNING [{EXENAME}]: {msg}',file=sys.stderr)

def to_label(field):
	return field.replace("_"," ").title()

def to_data(value):
	result = str(value)
	if result == 'nan':
		return "-"
	else:
		return result

def convert (input_name, output_name=None, options={} ) : 

	if not output_name:
		output_name = os.path.basename(input_name) + ".kml"

	data = pandas.read_csv(input_name)

	if LATITUDE not in data.columns or LONGITUDE not in data.columns:
		error("CSV does not include latitude and longitude fields")

	kml = simplekml.Kml()

	hidden = [LATITUDE,LONGITUDE,NAME,CLASS]

	for n, row in data.iterrows():
		lat = row[LATITUDE]
		lon = row[LONGITUDE]
		if -90 <= lat <= 90 and -180 <= lon <= 180:
			try:
				name = row[NAME]
			except:
				name = n
			desc = "<TABLE>"
			for field in data.columns:
				desc += f"<TR><TH ALIGN=LEFT>{to_label(field)}</TH><TD>{to_data(row[field])}</TD></TR>"
			desc += "</TABLE>"
			pt = kml.newpoint(name=str(name),coords=[(lon,lat)],description=desc)
	kml.save(output_name)
