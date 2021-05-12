"""Assemble the zipcode-level census data from the various sources"""
import os
import csv
import pickle
import pandas
import geopandas

try:
	# load the GDF file if found
	print(f"Loading zipcodes.gdf...",end='',flush=True)
	with open(f"zipcodes.gdf","rb") as f: zipcodes = pickle.load(f)
	print("ok",flush=True)
except:
	# GDF load failed
	print("error",flush=True)

	# download original data from source if not found
	if not os.path.exists("zipcodes.zip"):
		print("Downloading zipcodes.zip",end='',flush=True)
		zipcode_url = f"https://www2.census.gov/geo/tiger/TIGER2020/ZCTA5/tl_2020_us_zcta510.zip"
		with urllib.request.urlopen(zipcode_url) as resp, open("zipcodes.zip", "wb") as f:
			try:
				f.write(resp.read())
			except:
				os.remove(zipcode_file)
		print("ok",flush=True)

	# load the original data
	print(f"Loading zipcodes.zip...",end='',flush=True)
	zipcodes = geopandas.read_file("zip://zipcodes.zip")
	print("ok",flush=True)

	# save the GDF file
	print(f"Saving zipcodes.gdf...",end='',flush=True)
	with open("zipcodes.gdf","wb") as f: pickle.dump(zipcodes,f)
	print("ok",flush=True)


# remove unneeded columns in GDF file
print(f"Cleaning zipcode data...",end='',flush=True)
zipcodes.drop(columns=["GEOID10","CLASSFP10","MTFCC10","FUNCSTAT10"],inplace=True)
zipcodes.columns=["zipcode","land_area","water_area","center_latitude","center_longitude","geometry"]
zipcodes.set_index("zipcode")
print("ok",flush=True)

# TODO: add the population data
print(f"Adding population data...",end='',flush=True)
zipcodes["population"] = float('nan')
print("ok",flush=True)

# load the income data
print(f"Loading income data...",end='',flush=True)
income = pandas.read_csv("income_CA.csv",index_col="zipcode")
print("ok",flush=True)

# add the income data
# TODO: add other states
print(f"Adding income data...",end='',flush=True)
zipcodes = zipcodes.join(income,how="inner",on="zipcode",sort=True)
print("ok",flush=True)

# save the top-level zipcode files
# TODO: handle all zipcode
for digit in "9": #"0123456789":
	print(f"Writing zipcode{digit}.csv...",end='',flush=True)
	zipcodes[zipcodes["zipcode"].str.startswith(digit)].to_csv(f"zipcodes{digit}.csv",index=False)
	print("ok",flush=True)
