"""Extract ResStock building load data

See https://oedi-data-lake.s3.amazonaws.com/nrel-pds-building-stock/end-use-load-profiles-for-us-building-stock/README.md for documentation

See https://data.openei.org/s3_viewer?bucket=oedi-data-lake&prefix=nrel-pds-building-stock%2Fend-use-load-profiles-for-us-building-stock%2F2022%2Fresstock_amy2018_release_1.1%2Ftimeseries_aggregates%2Fby_state%2Fupgrade%3D10%2Fstate%3DCA%2F for data
"""

import os, sys
from datetime import datetime, tzinfo, timedelta
import pandas
import re

pandas.options.display.max_columns = None
pandas.options.display.width = None

# settings for California
REGION = "ca" # state for which loadshape is generated
UPGRADE = "00" # building upgrade (0 for none, 10 for maximum)
YEAR = 2018 # year for which loadshape is generated
TZOFFSET = -5 # timezone for which loadshape is generated

FLOORAREA = {
	"APARTMENT" : 1000.0,
	"CONDO" : 1200.0,
	"HOUSE" : 1700.0,
	"TOWNHOUSE" : 1500.0,
}

INPUTFOLDER = 'ResStock'
INPUTFILES = {
	f"up{UPDATE}-{UPGRADE}-multi-family_with_5plus_units.csv" : "APARTMENT",
	f"up{UPDATE}-{UPGRADE}-multi-family_with_2_-_4_units.csv" : "CONDO",
	f"up{UPDATE}-{UPGRADE}-single-family_detached.csv" : "HOUSE",
	f"up{UPDATE}-{UPGRADE}-single-family_attached.csv" : "TOWNHOUSE",
	}
COLUMNSPEC = [
	'timestamp',
	'units_represented',
	'out.electricity.*.kwh',
	'out.natural_gas.*.energy_consumption.kwh',
	]
FUELSPEC = {
	"electricity" : "ELECTRIC", 
	"natural_gas" : "GAS"}

result = []
for file, segment in INPUTFILES.items():

	print(f"Processing {file}...",flush=True)
	data = pandas.read_csv(os.path.join(INPUTFOLDER,file),parse_dates=['timestamp'])

	for column in data.columns:

		ok = False
		for pattern in COLUMNSPEC:
			if re.match(f"^{pattern}$",column):
				ok = True
		if not ok or 'total' in column:
			data.drop(column,axis=1,inplace=True)

	data.set_index('timestamp',inplace=True)
	units = data['units_represented'].mean()
	if data['units_represented'].std() > 1e-3:
		print("WARNING: 'units_represented' varies unexpectedly")
	data.drop('units_represented',axis=1,inplace=True)
	data = data.stack().reset_index()

	# create required data columns
	data.columns = ['datetime','loadtype','load']
	data['load'] = data['load'] / units / FLOORAREA[segment] * 1000

	# segment,enduse,fuel,month,day,hour,load,datetime
	data['segment'] = segment
	data['enduse'] = [x.split('.')[2].upper() for x in data['loadtype']]
	data['fuel'] = [FUELSPEC[x.split('.')[1]] for x in data['loadtype']]
	data['month'] = [int(x.month) for x in data['datetime']]
	data['day'] = [int(x.day) for x in data['datetime']]
	data['hour'] = [int(x.hour) for x in data['datetime']]
	data.drop('loadtype',axis=1,inplace=True)
	data.set_index(['segment','enduse','fuel','month','day','hour'],inplace=True)

	data = data.groupby(['segment','enduse','fuel','month','day','hour']).sum().round(4)
	data['datetime'] = [datetime(2018,x['month'],x['day'],x['hour'])+timedelta(hours=TZOFFSET) for n,x in data.reset_index().iterrows()]

	data.sort_index(inplace=True)

	result.append(data)


pandas.concat(result).to_csv("resstock_loadshapes.csv")
