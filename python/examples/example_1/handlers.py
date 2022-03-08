""" python/examples/example_1/handlers.py

This module illustrates how to implement handlers for a gridlabd model using a
'on_init' and 'commit' handlers to share collected data with the  main python
module. 
"""
import datetime as dt
import pandas as pd
import numpy as np 

def on_term(t):
    data = pd.read_csv("x.csv",names=["datetime","x"],index_col="datetime")
    index = np.array(list(map(lambda t:dt.datetime.strptime(t[0:-4],'%Y-%m-%d %H:%M:%S').timestamp(),data.index)))
    data.index = (index-index[0])/3600
    plot = data.plot(grid=True,xticks=range(0,25,5))
    plot.get_figure().savefig("example.png")
