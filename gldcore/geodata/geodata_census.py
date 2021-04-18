import os
import geopandas
import urllib.request
import pickle

urladdr = "https://www2.census.gov/geo/tiger/TIGER2017"
cachedir = "/usr/local/share/gridlabd/geodata/census"
os.makedirs(cachedir,exist_ok=True)

states_filename = "tl_2017_us_state.zip"
states_url = f"{urladdr}/STATE/{states_filename}"
states_file = f"{cachedir}/states"

zipcode_filename = "tl_2017_us_zcta510.zip"
zipcode_url = f"{urladdr}/ZCTA5/{zipcode_filename}"
zipcode_file = f"{cachedir}/zipcode"

def verbose(msg):
    print(f"VERBOSE [census]: {msg}",flush=True)

for data_file, url in zip([states_file, zipcode_file], [states_url, zipcode_url]):
    if not os.path.exists(data_file+".zip"):
        verbose(f"Downloading from {url}")
        with urllib.request.urlopen(url) as resp, open(data_file+".zip", "wb") as f:
            verbose(f"Saving to {data_file}")
            try:
                f.write(resp.read())
            except:
                verbose(f"Cleaning up {data_file}")
                os.remove(data_file)

if not os.path.exists(f"{zipcode_file}.gdf"):
    verbose(f"Caching zipcode data")
    zipcodes = geopandas.read_file(f"zip://{zipcode_file}.zip")
    with open(f"{zipcode_file}.gdf","wb") as f: pickle.dump(zipcodes,f)
else:
    verbose("Loading zipcode data")
    with open(f"{zipcode_file}.gdf","rb") as f: zipcodes = pickle.load(f)

if not os.path.exists(f"{states_file}.gdf"):
    verbose(f"Caching states data")
    states = geopandas.read_file(f"zip://{states_file}.zip")
    with open(f"{states_file}.gdf","wb") as f: pickle.dump(states,f)
else:
    verbose("Loading states data")
    with open(f"{states_file}.gdf","rb") as f: states = pickle.load(f)

print(zipcodes.columns)
print(states.columns)
