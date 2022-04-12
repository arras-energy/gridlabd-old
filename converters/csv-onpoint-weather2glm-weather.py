import sys, os
import urllib.request as urllib
import pandas as pd
import datetime as dt

def convert(input,output=None,options={}):

	if output == None:
		output = os.path.basename(input).replace('.csv','.glm')
	csvname = output.replace('.glm','.csv')

	if 'country' not in options.keys():
		raise Exception("country not specified in options")
	if 'postal_code' not in options.keys():
		raise Exception("country not specified in options")
	rows = {'country':options['country'], 'postal_code':options['postal_code']}

	if 'columns' not in options.keys():
		options['columns'] = {
				'time_valid_utc':'#datetime',
				'temperature_air_2m_f':'temperature',
				'humidity_relative_2m_pct':'humidity',
				'radiation_solar_total_wpm2':'solar_total',
				'wind_speed_10m_mph':'wind_speed',
				'wind_direction_10m_deg':'wind_direction',
			}

	# load and reformat weather data
	try:
		if not os.path.exists(csvname) or ( 
				'refresh' in options.keys() and options['refresh'] in [True,'TRUE','always','true'] ):
			with open(csvname,"w") as csv:
				with urllib.urlopen(input) as fh:
					data = fh.read()
					csv.write(data.decode('utf-8'))

			data = pd.read_csv(csvname,dtype=str)
			for key,value in rows.items():
				data = data[data[key] == str(value)]
			if 'index' in options.keys():
				data.set_index(options['index'],inplace=True)
			data = data.filter(list(options['columns'].keys())).rename(mapper=options['columns'],axis='columns')
			if not 'index' in options.keys():
				data.set_index(data.columns[0],inplace=True)
			data.dropna().to_csv(csvname)
	except:
		os.remove(csvname)
		raise

	# write the GLM results
	name = csvname.replace('.csv','')
	try:
		with open(output,"w") as glm:
			glm.write(f'// converted from {input} to {output} on {dt.datetime.now()}\n')
			glm.write("""
module tape;
class weather {
	char32 country;
	char32 postal_code;
	double temperature[degF];
	double humidity[pu];
	double solar_total[W/m^2];
	double wind_speed[mph];
	double wind_direction[degF];
}	
""")
			glm.write(f'object weather\n')
			glm.write('{\n')
			glm.write(f'\tname "{name}";\n')
			glm.write(f'\tcountry "{options["country"]}";\n')
			glm.write(f'\tpostal_code "{options["postal_code"]}";\n')
			glm.write(f'\tobject player\n')
			glm.write('\t{\n')
			glm.write(f'\t\tfile "{csvname}";\n')		
			glm.write(f'\t\tproperty "{",".join(list(options["columns"].values())[1:])}";\n')		
			glm.write('\t};\n')
			glm.write('}\n')
	except:
		os.remove(output)
		raise

if __name__ == '__main__':
	convert('https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/onpoint_data/onpoint_history_postal-code_hour_201801010000-201812312359.csv',
		options={'refresh':True,'country':'US','postal_code':'36101'})
