"""Test vegetation analysis on 230kV powerline example"""

import sys
from PIL import Image
import IPython.display as dsp
import numpy as np
import pandas as pd
from scipy.interpolate import interp1d

if len(sys.argv) < 2:
	print("Syntax: vegetation -i|--input PATHFILE -m|--mapfile MAPFILE -o|--output DATAFILE [-s|--show] [-r|--resolution METERS]")
	exit(0)

PATHFILE = None
MAPFILE = None
DATAFILE = None
SHOWPLOT = False
res = 10 # TIFF resolution in meters

n = 1
while n < len(sys.argv):
	if sys.argv[n] in ["-i","--input"]:
		n += 1
		PATHFILE = sys.argv[n]
	elif sys.argv[n] in ["-m","--mapfile"]:
		n += 1
		MAPFILE = sys.argv[n]
	elif sys.argv[n] in ["-o","--output"]:
		n += 1
		DATAFILE = sys.argv[n]
	elif sys.argv[n] in ["-s","--show"]:
		SHOWPLOT = True
	elif sys.argv[n] in ["-r","--resolution"]:
 		n += 1
 		res = int(sys.argv[n]) 
	else:
		print(f"ERROR [vegetation]: '{sys.argv[n]}' is an invalid option")
		exit(1)
	n += 1

if not PATHFILE or not MAPFILE or not DATAFILE:
	print(f"ERROR [vegetation]: missing required argument")

def vegetation_data(PATHFILE,MAPFILE,DATAFILE,SHOWPLOT,res):
	image_data = {
		"MarinCounty" : {
			"p0" : {"lat":38.131435, "lon":-122.740464},
			"p1" : {"lat":38.110846, "lon":-122.724984},
		},
		"SanMateoCounty" : {
			"p0" : {"lat":"37:47:29", "lon":"-122:31:31"},
			"p1" : {"lat":"37:06:24", "lon":"-122:06:57"},	
		},
	}

	# convert dms coordinates to degrees
	for county, specs in image_data.items():
		for spec, coords in specs.items():
			for name, coord in coords.items():
				if type(coord) is str:
					dms = list(map(lambda n: int(n),coord.split(":")))
					image_data[county][spec][name] = np.sign(dms[0])*(abs(dms[0]) + (dms[1] + dms[2]/60)/60)
	p0 = image_data[county]["p0"]
	p1 = image_data[county]["p1"]
	box = {
		"N":max(p0["lat"],p1["lat"]), 
		"S":min(p0["lat"],p1["lat"]), 
		"E":max(p0["lon"],p1["lon"]), 
		"W":min(p0["lon"],p1["lon"]),
		}

	def get_values(specs,path):

		dd = res*39/12 # image resolution in feet
		img = Image.open(specs)
		if SHOWPLOT:
			img.show()
		data = np.array(img)
		result = []
		dt = 0
		for line in path:
			p0 = line["p0"]
			p1 = line["p1"]
			x0 = int((p0["lon"]-box["W"])/(box["E"]-box["W"])*data.shape[0])
			y0 = int((p0["lat"]-box["N"])/(box["S"]-box["N"])*data.shape[1])
			x1 = int((p1["lon"]-box["W"])/(box["E"]-box["W"])*data.shape[0])
			y1 = int((p1["lat"]-box["N"])/(box["S"]-box["N"])*data.shape[1])
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
				"z": np.array(z),
			})
			dt += d
		return result

	line_data = pd.read_csv(PATHFILE)
	lats = line_data["latitude"]
	lons = line_data["longitude"]
	start = {"lat" : lats[0], "lon" : lons[0]}
	path = []
	for line in range(1,len(lats)):
		stop = {"lat" : lats[line], "lon" : lons[line]}
		path.append({"p0":start,"p1":stop})
		start = stop
	results = get_values(MAPFILE,path)

	data = []
	for segment in results:
		data.append(pd.DataFrame({"t":segment["t"],"z":segment["z"]}).set_index("t"))
	pd.concat(data).to_csv("230kV_example_canopycover.csv")

	if SHOWPLOT:
		import matplotlib.pyplot as plt
		plt.figure(1,figsize=(15,5))
		for result in results:
			plt.plot(result["t"],result["z"]*100,"-k")
		plt.xlabel('Line location (ft)')
		plt.ylabel('Canopy cover (%)')
		plt.ylim([0,100])
		plt.grid()
		start = [round(results[0]["from"]["lat"],4),round(results[0]["from"]["lon"],4)]
		stop = [round(results[-1]["to"]["lat"],4),round(results[-1]["to"]["lon"],4)]
		plt.title(f"{MAPFILE} from {start} to {stop}")
		plt.show()

if __name__ == '__main__':
	vegetation_data(PATHFILE,MAPFILE,DATAFILE,SHOWPLOT,res)
