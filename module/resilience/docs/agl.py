import pandas as pd

line_data = pd.read_csv("230kV_example.csv")

left = line_data["latitude"].min()
right = line_data["latitude"].max()
top = line_data["longitude"].max() 
bottom = line_data["longitude"].min()
print(left,bottom,right,top)

import elevation

elevation.clip(bounds=(left,bottom,right,top),output="230kV_elevation.tif")
elevation.clean()

