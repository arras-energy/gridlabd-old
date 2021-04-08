import os, subprocess
import pandas as pd
import math
from PIL import Image

cache_folder = "/tmp"

def get_data(csv_file,resolution=0.13): # default resolution is 1 ft
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
        result = subprocess.run(["eio",
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
    return {
        "filename" : tif_file,
        "left" : left,
        "bottom" : bottom,
        "right" : right,
        "top" : top,
        }

if __name__ == '__main__':
    result = get_data("module/resilience/docs/230kV_example.csv")
    print(result)
    if "error" in result.keys():
        print(result["stderr"])
    else:
        img = Image.open(result["filename"])
        img.show()
