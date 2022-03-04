"""Convert Schneider ION meter data to GLM players
"""
import os, sys
import pandas as pd

default_options = dict(
    csv_index = "Timestamp UTC",
    csv_source = "Measurement",
    csv_value = "Value",

    csv_real_power = "Real Power",
    csv_reactive_power = "Reactive Power",

    player_index = "timestamp",
    player_real_power = "real_power",
    player_reactive_power = "reactive_power",
)

def convert(inputfile,outputfile,options=default_options):

    pathname = os.path.splitext(outputfile)[0]
    basename = os.path.basename(pathname)

    data = pd.read_csv(inputfile,parse_dates=[csv_index]).pivot(csv_index,csv_source,csv_value)
    data.index.name = player_index
    data = data.loc[:,[csv_real_power,csv_reactive_power]]
    data.columns = [player_real_power,player_reactive_power]

    data.to_csv(pathname+".csv",header=False,index=player_index)



