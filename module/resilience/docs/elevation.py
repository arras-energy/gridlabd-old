"""Global elevation data acquisition

REQUIREMENTS

Requires the elevation module (https://pypi.org/project/elevation/)
and the GDAL package (http://www.gdal.org).
"""
import os, subprocess
import pandas as pd
import numpy as np
import math
from PIL import Image
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

cache_folder = "/tmp"

def project(data,box,lat,lon):
	x = int((lon-box["W"])/(box["E"]-box["W"])*data.shape[0])
	y = int((lat-box["N"])/(box["S"]-box["N"])*data.shape[1])
	return x,y

def get_values(specs,path,res=10,box={}):

    dd = res*39/12 # image resolution in feet
    img = Image.open(specs)
    data = np.array(img)
    result = []
    dt = 0
    p = {"dt":[],"zz":[]}
    for line in path:
        p0 = line["p0"]
        p1 = line["p1"]
        x0,y0 = project(data,box,p0["lat"],p0["lon"])
        x1,y1 = project(data,box,p1["lat"],p1["lon"])
        dx = x1-x0
        dy = y1-y0
        s = 0.0
        n = 0
        zz = []
        tt = []
        if abs(dx) > abs(dy): # iterate over x
            y = y0
            r = dy/dx
            for x in range(x0,x1+1,np.sign(dx)):
                zz.append(data[int(x),int(y)])
                tt.append(np.sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y))*dd)
                y += r
        else: # iterate over y
            x = x0
            r = dx/dy
            for y in range(y0,y1+1,np.sign(dy)):
                zz.append(data[int(x),int(y)])
                tt.append(np.sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y))*dd)
                x += r
        d = round(np.sqrt(dx*dx+dy*dy)*dd) # distance in feet
        zz = np.array(zz)
        t = np.array(tt)
        z = (list(map(lambda x:float(x/res/res),list(map(interp1d(t,zz),np.arange(tt[0],tt[-1],1.0))))))
        result.append({
            "from" : p0,
            "to" :p1,
            "min":zz.min(),
            "max":zz.max(),
            "avg":np.round(zz.mean(),1),
            "std":np.round(zz.std(),1),
            "len":d,
            "t" : np.arange(tt[0],tt[-1],1.0) + dt,
            "z": np.array(z).round(3)*100,
        })
        p["dt"].append(dt)
        p["zz"].append(zz[0])
        dt += d
    return result, p

def get_data(csv_file): # default resolution is 1 ft
    """Get elevation data for region around path in CSV file.

    PARAMETERS

      csv_file: path along which elevation data is desired
                latitude,longitude columns must be labeled in first row
    """
    line_data = pd.read_csv(csv_file)

    bottom = math.floor(line_data["latitude"].min())
    top = math.ceil(line_data["latitude"].max())
    left = math.floor(line_data["longitude"].min())
    right = math.ceil(line_data["longitude"].max())

    tif_file = f"{cache_folder}/elevation_{left}_{bottom}_{right}_{top}_10m.tif"
    if not os.path.exists(tif_file):
        # unfortunately the python API call to elevation.clip does not work correctly
        result = subprocess.run(["/usr/local/bin/eio",
            "--product","SRTM1",
            "clip",
            "-o",tif_file,
            "--bounds",str(left),str(bottom),str(right),str(top)],
            capture_output=True)
        return {
            "error" : result.returncode,
            "stderr" : result.stderr,
            "stdout" : result.stdout,
        }

    lats = line_data["latitude"]
    lons = line_data["longitude"]
    start = {"lat" : lats[0], "lon" : lons[0]}
    path = []
    for line in range(1,len(lats)):
        stop = {"lat" : lats[line], "lon" : lons[line]}
        path.append({"p0":start,"p1":stop})
        start = stop
    results, poles = get_values(tif_file,path,10,{"N":top,"E":right,"W":left,"S":bottom})

    data = []
    for segment in results:
        data.append(pd.DataFrame({"t":segment["t"],"z":segment["z"]}).set_index("t"))
    pd.concat(data).to_csv(csv_file.replace(".csv","_elevation.csv"))

    return {
        "filename" : tif_file,
        "left" : left,
        "bottom" : bottom,
        "right" : right,
        "top" : top,
        "elevation" : results,
        "poles" : poles,
        }

if __name__ == '__main__':
    results = get_data("module/resilience/docs/230kV_example.csv")
    if "error" in results.keys():
        print(results["stderr"])
    else:
        img = Image.open(results["filename"])
        img.show()
    plt.figure(1,figsize=(15,5))
    elevation = results["elevation"]
    poles = results["poles"]

    t = []
    z = []
    for result in elevation:
        t.extend(result["t"])
        z.extend(result["z"])
    plt.plot(t,z,"-k")
    plt.plot(poles["dt"],poles["zz"],"or")
    plt.xlabel('Line location (ft)')
    plt.ylabel('Elevation (m)')
    plt.grid()
    start = [round(elevation[0]["from"]["lat"],4),round(elevation[0]["from"]["lon"],4)]
    stop = [round(elevation[-1]["to"]["lat"],4),round(elevation[-1]["to"]["lon"],4)]
    plt.title(f"{results['filename']} from {start} to {stop}")
    plt.show()
