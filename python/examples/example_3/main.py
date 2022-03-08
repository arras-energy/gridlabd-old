""" python/examples/example_3/main.py

This is an example of a main Python module that runs a simulation and plots
the collected data to a PNG file.  This process is completed in two parts.
"""

import sys
assert(sys.version_info.major>2)
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import gridlabd
import datetime as dt

gridlabd.command('model.glm')
gridlabd.start('wait')

data = pd.read_csv("x.csv",names=["datetime","x"],index_col="datetime")
index = np.array(list(map(lambda t:dt.datetime.strptime(t[0:-4],'%Y-%m-%d %H:%M:%S').timestamp(),data.index)))
data.index = (index-index[0])/3600
plot = data.plot(grid=True,xticks=range(0,25,5))
plot.get_figure().savefig("example.png")

