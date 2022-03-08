"""NOAA LCD to GridLAB-D weather converter

This converter reads a NOAA LCD file (with extension CSV) and writes a GridLAB-D weather 
object (with extension GLM).  The converter includes a solar irradiance estimate based 
on the Zhang-Huang model (see Zhang Q, J Huang, and L Siwei, "Development of typical year 
weather data for Chinese locations", ASHRAE Transactions 108:2, 2002).  The model is used
as follows

	I[t] = max(0,(I0*sin(h[t])*(c0+c1*CC[t]+c2*CC[t]**2+c3*(T[t]-T[t-3])+c4*RH[t]+c5*V[t])+d)/k)

where

	I   estimated solar irradiance in J/m^2.h
	I0  solar constant, i.e., 1355 W/m^2
	h   solar altitude angle
	CC  cloud cover in 1/10 pu
	RH  relative humidity in %
	T   dry-bulb temperature in Celcius
	V   wind speed in m/s

and the regression coefficients are

	c0 = 0.5598
	c1 = 0.4982
	c2 = -0.6762
	c3 = 0.02842
	c4 = -0.00317
	c5 = 0.014
	d  = -17.853
	k  = 0.843

The total global solar radiation is separated into direct (DH) and diffuse (SH) components as follows:

	KT = I/(I0*sin(h[t]))
	KTc = 0.4268 + 0.1934*sin(h[t])
	K = KT < KTc ? (2.996-3.862*sin(h[t])+1.540*sin(h[t])**2)*KT**3 : (KT-(1.107+0.03569*sin(h[t])+1.681*sin(h[t])**2)*(1-KT)**2) 
	DH = I0*sin(h[t])*K*(1-KT)/(1-K)
	SH = I0*sin(h[t])*(KT-K)/(1-K)

"""
import sys, os
from math import sin, pi
import urllib.request as urllib
import pandas as pd
import datetime as dt
import pysolar.solar as solar

timezone="+00:00"

# TODO: these values are based on estimates from TMY file for a location
#       a list of known values is available from a database that can obtained from Claudio
I0 = 1355.0
c0 = 0.5598
c1 = 0.4982
c2 = -0.6762
c3 = 0.02842
c4 = -0.00317
c5 = 0.014
d  = -17.853
k  = 0.843

def to_datetime(x):
	return dt.datetime.fromisoformat(x+timezone)

last_temperature = 59.0
def to_temperature(x):
	global last_temperature
	try: 
		last_temperature = float(x.rstrip('s'))
	except:
		pass
	return last_temperature

last_humidity = 50.0
def to_humidity(x):
	global last_humidity
	try:
		last_humidity = float(x.rstrip('s'))
	except:
		pass
	return last_humidity

last_windspeed = 0.0	
def to_windspeed(x):
	global last_windspeed
	try:
		last_windspeed = float(x.rstrip('s'))
	except:
		pass
	return last_windspeed
	
last_winddirection = 0.0
def to_winddirection(x):
	global last_winddirection
	try:
		last_winddirection = float(x.rstrip('s'))
	except:
		pass
	return last_winddirection

last_rainfall = 0.0
def to_rainfall(x):
	global last_rainfall
	try:
		last_rainfall = float(x.rstrip('s').replace('T','0').replace('M','0'))
	except:
		pass
	return last_rainfall

last_pressure = 29.92	
def to_pressure(x):
	global last_pressure
	try:
		last_pressure = float(x.rstrip('s'))
	except:
		pass
	return last_pressure

last_cover = 0.0
def to_cover(x):
	global last_cover
	if not x:
		return last_cover
	try:
		sky = x.split(' ')
		if len(sky) < 2:
			last_cover = 0.0
		else:
			last_cover = float(sky[-2].split(':')[1])/10
	except:
		pass
	# print(x,sky,' --> ',last_cover,flush=True)
	return last_cover

def convert(input,output=None,options={}):

	if output == None:
		output = os.path.basename(input).replace('.csv','.glm')
	csvname = output.replace('.glm','.csv')

	if 'station_id' not in options.keys():
		raise Exception("station_id not specified in options")

	if 'latitude' not in options.keys() or 'longitude' not in options.keys():
		raise Exception("latitude and longitude must be specified")
	else:
		latitude = float(options["latitude"])
		if not -90 < latitude < +90:
			raise "latitude must be between -90 and +90"
		longitude = float(options["longitude"])
		if not -180 < longitude < +180:
			raise "longitude must be between -180 and +180"

	if 'ground_albedo' in options.keys():
		albedo = float(options["ground_albedo"])
		if not 0.0 < albedo < 1.0:
			raise "ground_albedo must be between 0.0 and 1.0"
	else:
		albedo = None

	if 'timezone' not in options.keys():
		raise Exception("timezone not specified")
	else:
		global timezone
		timezone = options["timezone"]

	rows = {'STATION':options['station_id']}

	if 'columns' not in options.keys():
		options['columns'] = {
				'DATE':'#datetime',
				'HourlyDryBulbTemperature':'temperature[degF]',
				'HourlyRelativeHumidity':'humidity[%]',
				'HourlyWindSpeed':'wind_speed[mph]', # mph
				'HourlyWindDirection':'wind_dir[deg]', # degree
				'HourlyStationPressure':'pressure[inHg]', # inHg
				'HourlyPrecipitation':'rainfall[in/h]', # inches
				# used to generation sky cover and solar irradiance records
				'HourlySkyConditions':'opq_sky_cov[pu]', # TOK:okta alt [TOK:okta alt [TOK:okta alt]]
			}

	# load and reformat weather data
	try:
		if not os.path.exists(csvname) or ( 
				'refresh' in options.keys() and options['refresh'] in [True,'TRUE','always','true'] ):
			with open(csvname,"w") as csv:
				with urllib.urlopen(input) as fh:
					data = fh.read()
					csv.write(data.decode('utf-8'))

			data = pd.read_csv(csvname,low_memory=False,converters={
					"DATE" : to_datetime,
					"HourlyDryBulbTemperature" : to_temperature,
					"HourlyRelativeHumidity" : to_humidity,
					"HourlyWindSpeed" : to_windspeed,
					"HourlyWindDirection" : to_winddirection,
					"HourlyStationPressure" : to_pressure,
					"HourlyPrecipitation" : to_rainfall,
					"HourlySkyConditions" : to_cover,
				})
			for key,value in rows.items():
				data = data[data[key] == str(value)]
			if 'index' in options.keys():
				data.set_index(options['index'],inplace=True)
			data = data.filter(list(options['columns'].keys())).rename(mapper=options['columns'],axis='columns')
			if not 'index' in options.keys():
				data.set_index(data.columns[0],inplace=True)
			solar_total = []
			solar_dir = []
			solar_diff = []
			for t in range(len(data.index)):
				# print(type(date),dir(date))
				# breakpoint()
				h = solar.get_altitude(latitude,longitude,data.index[t].to_pydatetime())*pi/180
				if h < 0:
					I = 0.0
					DH = 0.0
					SH = 0.0
				else:
					CC = data['opq_sky_cov[pu]'][t]
					T0 = data['temperature[degF]'][t]
					T3 = data['temperature[degF]'][t-3]
					RH = data['humidity[%]'][t]
					V = data['wind_speed[mph]'][t]
					I = (I0*sin(h)*(c0+c1*CC+c2*CC**2+c3*(T0-T3)+c4*RH+c5*V)+d)/k
					if I > 0 :
						KT = I/(I0*sin(h))
						KTc = 0.4268 + 0.1934*sin(h)
						if KT < KTc:
							K = (2.996-3.862*sin(h)+1.540*sin(h)**2)*KT**3 
						else: 
							K = (KT-(1.107+0.03569*sin(h)+1.681*sin(h)**2)*(1-KT)**2) 
						DH = round(I0*sin(h)*K*(1-KT)/(1-K),1)
						SH = round(I0*sin(h)*(KT-K)/(1-K),1)
						I = round(I,1)
					else:
						I = 0.0
						DH = 0.0
						SH = 0.0
				solar_total.append(I)
				solar_dir.append(DH)
				solar_diff.append(SH)
			data['solar_global[W/sf]'] = solar_total
			data['solar_direct[W/sf]'] = solar_dir 
			data['solar_diffuse[W/sf]'] = solar_diff
			options['columns']['HourlySolarGlobal'] = 'solar_global[W/sf]'
			options['columns']['HourlySolarDirect'] = 'solar_direct[W/sf]'
			options['columns']['HourlySolarDiffuse'] = 'solar_diffuse[W/sf]'
			data.dropna().to_csv(csvname, date_format='%Y-%m-%d %H:%M:%S')
	except:
		os.remove(csvname)
		raise

	# write the GLM results
	name = csvname.replace('.csv','')
	try:
		with open(output,"w") as glm:
			glm.write(f'// converted from {input} to {output} on {dt.datetime.now()}\n')
			if not "--use-climate" in options:
				glm.write("module tape;\n")
				glm.write("class weather {\n")
				glm.write("\tchar32 station_id;\n")
				glm.write("\tdouble temperature[degF];\n")
				glm.write("\tdouble humidity[%];\n")
				glm.write("\tdouble wind_speed[mph];\n")
				glm.write("\tdouble wind_dir[deg];\n")
				glm.write("\tdouble pressure[mbar];\n")
				glm.write("\tdouble rainfall[in/h];\n")
				glm.write("\tdouble opq_sky_cov[pu];\n")
				glm.write("\tdouble tot_sky_cov[pu];\n")
				glm.write("\tdouble solar_direct[W/sf];\n")
				glm.write("\tdouble solar_diffuse[W/sf];\n")
				glm.write("\tdouble solar_global[W/sf];\n")
				glm.write("\tdouble ground_reflectivity[pu];\n")
				glm.write("\t}\n")
				glm.write(f'object weather\n')
				glm.write('{\n')
				glm.write(f'\tname "{name}";\n')
				glm.write(f'\tstation_id "{options["station_id"]}";\n')
				if albedo:
					glm.write(f'\tground_reflectivity {albedo} pu;\n')
				glm.write(f'\tobject player\n')
				glm.write('\t{\n')
				glm.write(f'\t\tfile "{csvname}";\n')		
				glm.write(f'\t\tproperty "{",".join(list(options["columns"].values())[1:])}";\n')		
				glm.write('\t};\n')
				glm.write('}\n')
			else:
				raise Exception("climate output not implemented yet")
	except:
		os.remove(output)
		raise

	if __name__ == '__main__':
		convert('https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/test_data/noaa.csv',
			options={'refresh':True,'station_id':'72594524283'})
