import sys, os
import urllib.request as urllib
import pandas as pd
import datetime as dt

def convert(input,output=None,options={}):

	if output == None:
		output = os.path.basename(input).replace('.csv','.glm')
	csvname = output.replace('.glm','.csv')

	if 'station_id' not in options.keys():
		raise Exception("station_id not specified in options")
	rows = {'STATION':options['station_id']}

	if 'columns' not in options.keys():
		options['columns'] = {
				'DATE':'#datetime',
				'HourlyDryBulbTemperature':'temperature',
				'HourlyRelativeHumidity':'humidity',
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
			data['temperature'] = pd.Series(data['temperature']).str.rstrip('s')
			data.dropna().to_csv(csvname)
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
				glm.write("\t}\n")
				glm.write(f'object weather\n')
				glm.write('{\n')
				glm.write(f'\tname "{name}";\n')
				glm.write(f'\tstation_id "{options["station_id"]}";\n')
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
