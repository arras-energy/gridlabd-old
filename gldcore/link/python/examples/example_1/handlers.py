""" gldcore/link/python/examples/example_1/handlers.py

This module illustrates how to implement handlers for a gridlabd model using a
'on_init' and 'commit' handlers to share collected data with the  main python
module. """
import datetime
import pandas as pd
import numpy as np 
import matplotlib.pyplot as plt

def on_term(t):

	pd.read_csv("x.csv")
	pd.plot()