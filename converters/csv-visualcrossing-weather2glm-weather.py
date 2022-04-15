import sys, os
import urllib.request as urllib
import pandas as pd
import datetime as dt

def convert(input,output=None,options={}):

	if output == None:
		output = os.path.basename(input).replace('.csv','.glm')
	csvname = output.replace('.glm','.csv')

	if 'location' not in options.keys():
		raise Exception("location not specified in options")
	rows = {'Location':options['location']}

	if 'columns' not in options.keys():
		options['columns'] = {
				'Date time':'#datetime',
				'Temperature':'temperature',
				'Relative Humidity':'humidity',
				'Wind Speed':'wind_speed',
				'Cloud Cover':'opq_sky_cov',
			}

	# load and reformat the weather data
	if not os.path.exists(csvname) or ( 
			'refresh' in options.keys() and options['refresh'] in [True,'TRUE','always','true'] ):
		with open(csvname,"w") as csv:
			with urllib.urlopen(input) as fh:
				data = fh.read()
				csv.write(data.decode('utf-8'))
		def us_dates(s):
			return dt.datetime.strptime(s,'%m/%d/%Y %H:%M:%S')
		data = pd.read_csv(csvname,dtype=str,parse_dates=['Date time'], date_parser=us_dates).fillna(value=0.0)
		for key,value in rows.items():
			data = data[data[key] == str(value)]
		if 'index' in options.keys():
			data.set_index(options['index'],inplace=True)
		data = data.filter(list(options['columns'].keys())).rename(mapper=options['columns'],axis='columns')
		if not 'index' in options.keys():
			data.set_index(data.columns[0],inplace=True)
		data.dropna().to_csv(csvname)

	# write the GLM results
	name = csvname.replace('.csv','')
	with open(output,"w") as glm:
		glm.write(f'// converted from {input} to {output} on {dt.datetime.now()}\n')
		glm.write("""
module tape;
class weather {
	char32 location;
	double temperature[degF];
	double humidity[%];
	double wind_speed[mph];
	double opq_sky_cov[%];
}	
""")
		glm.write(f'object weather\n')
		glm.write('{\n')
		glm.write(f'\tname "{name}";\n')
		glm.write(f'\tlocation "{options["location"]}";\n')
		glm.write(f'\tobject player\n')
		glm.write('\t{\n')
		glm.write(f'\t\tfile "{csvname}";\n')		
		glm.write(f'\t\tproperty "{",".join(list(options["columns"].values())[1:])}";\n')		
		glm.write('\t};\n')
		glm.write('}\n')



if __name__ == '__main__':
	convert('https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/visual_crossing/test_data.csv',
		options={'refresh':True,'location':'Menlo Park, CA'})
