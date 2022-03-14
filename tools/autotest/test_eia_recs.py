"""Plot histogram of floor areas in json file

Syntax: python -m test_eia_recs input.json output.png
"""

import sys, json
import matplotlib.pyplot as plt

if len(sys.argv) < 3:
    raise Exception("missing input and output file names")

with open(sys.argv[1],"r") as f:
    glm = json.load(f)
    if glm["application"] != "gridlabd":
        raise Exception("input is not a GridLAB-D model")
    area = [float(obj["floor_area"].split()[0]) for obj in glm["objects"].values()]
    plt.hist(area,20,range=(0,10000))
    plt.xlabel("Floor area (sf)")
    plt.ylabel("Number of homes")
    plt.title(f"Floor areas (N={len(area)})")
    plt.grid()
    plt.savefig(sys.argv[2])