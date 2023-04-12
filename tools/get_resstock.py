import os, sys
import requests
import pandas
from datetime import datetime

ROOT = "/tmp"
URL = "https://oedi-data-lake.s3.amazonaws.com/nrel-pds-building-stock/end-use-load-profiles-for-us-building-stock"
TZ = -5 # NREL data is always EST

def get_loaddata(dataset,year,upgrade,state,building_type,group=None):
	path = os.path.join(ROOT,dataset,str(year),state)
	file = os.path.join(path,f"{building_type}-{upgrade:02d}.csv")
	print(file)
	if not os.path.exists(file):
		url = os.path.join(URL,str(year),dataset,f"timeseries_aggregates/by_state/upgrade={upgrade}",f"state={state.upper()}",f"up{upgrade:02d}-{state.lower()}-{building_type}.csv")
		print(url)
		reply = requests.get(url)
		if reply.status_code == 200:
			os.makedirs(path,exist_ok=True)
			with open(file,"w") as fh:
				fh.write(reply.text)
		else:
			print(f"ERROR: requests.get('{url}') failed with code {reply.status_code}",file=sys.stderr)
	data = pandas.read_csv(file,
		index_col = ['timestamp'],
		parse_dates = ['timestamp'],
		usecols = ['timestamp',
			'units_represented',
			'out.electricity.total.energy_consumption.kwh',
			'out.natural_gas.total.energy_consumption.kwh'],
		)
	data.columns = ['houses','electric','gas']
	data['electric'] = data['electric'] / data['houses']
	data['gas'] = data['gas'] / data['houses']
	data.drop('houses',axis=1,inplace=True)
	data.index = data.index + pandas.Timedelta(hours=TZ)
	if group:
		return data.groupby(pandas.Grouper(freq=group)).sum()
	else:
		return data

data = get_loaddata(dataset = "resstock_amy2018_release_1",
	year = 2022,
	upgrade = 0,
	state = "CA",
	building_type = "single-family_detached")
print(data)