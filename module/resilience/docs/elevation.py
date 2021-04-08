import os
import pandas as pd
import math
from PIL import Image

def get_data(csv_file,resolution=0.13): # default resolution is 1 ft
    """Get elevation data for region around path in CSV file.

    PARAMETERS

      csv_file: path along which elevation data is desired
                latitude,longitude columns must be labeled in first row
    """
    line_data = pd.read_csv(csv_file)
    tif_file = csv_file.replace(".csv","_elevation.tif")

    bottom = math.floor(line_data["latitude"].min())
    top = math.ceil(line_data["latitude"].max())
    left = math.floor(line_data["longitude"].min())
    right = math.ceil(line_data["longitude"].max())

    output = "230kV_example.tif"

    # unfortunately the python API call to elevation.clip does not work correctl
    os.system(f"eio --product SRTM1 clip -o {tif_file} --bounds {left} {bottom} {right} {top}")
    return {
        "filename" : tif_file,
        "left" : left,
        "bottom" : bottom,
        "right" : right,
        "top" : top,
        }

if __name__ == '__main__':
    result = get_data("module/resilience/docs/230kV_example.csv")
    img = Image.open(result["filename"])
    img.show()
