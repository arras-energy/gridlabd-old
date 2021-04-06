import pandas as pd
import math

line_data = pd.read_csv("230kV_example.csv")

left = math.floor(line_data["latitude"].min())
right = math.ceil(line_data["latitude"].max())
top = math.ceil(line_data["longitude"].max())
bottom = math.floor(line_data["longitude"].min())
print(left,bottom,right,top)

import elevation

elevation.clip(bounds=(left,bottom,right,top),output="elevation.tif")
elevation.clean()

from PIL import Image
img = Image.open("elevation.tif")
img.show()
