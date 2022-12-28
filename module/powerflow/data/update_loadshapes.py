import pandas
from datetime import datetime
import matplotlib.pyplot as plt

# load CEUS data
data = pandas.read_csv("ceus_loadshapes.csv",parse_dates=['datetime'])
data.drop('month',axis=1,inplace=True)
data.drop('day',axis=1,inplace=True)
data.drop('hour',axis=1,inplace=True)
data.set_index(['segment','enduse','fuel','datetime'],inplace=True)

# cleanup CEUS data
load = pandas.DataFrame(data.groupby(['segment','fuel','datetime']).sum())
load['season'] = load.index.get_level_values(2).quarter
load['weekday'] = (load.index.get_level_values(2).dayofweek < 5)
load['hour'] = load.index.get_level_values(2).hour
load.reset_index(inplace=True)
load.set_index(['segment','season','fuel','weekday','hour'],inplace=True)
load.drop('datetime',axis=1,inplace=True)
load['load'] = load['load'].round(4)

# plot loads
for segment in load.index.get_level_values(0).unique():
    plt.figure(figsize=(18,24))
    n = 0
    for season in load.loc[segment].index.get_level_values(0).unique():
        for fuel in load.loc[segment,season].index.get_level_values(0).unique():
            title = ' '.join([fuel,("WINTER","SPRING","SUMMER","FALL")[season-1]])
            n += 1
            plt.subplot(4,2,n)
            for weekday in load.loc[segment,season,fuel].index.get_level_values(0).unique():
                weekday_name = "Week" + ("day" if weekday else "end")
                shape = load.loc[(segment,season,fuel,weekday)].groupby(['hour']).mean()
                plt.plot(shape,label=weekday_name)
            plt.legend()
            plt.xlabel('Hour of day')
            plt.ylabel('Load (W/sf)')
            plt.grid()
            plt.title(title[0].upper()+title[1:].lower())
    plt.suptitle(segment[0].upper()+segment[1:].lower())
    plt.savefig(segment.lower()+".png")

# create loadshape table
loadshapes = pandas.DataFrame(load.groupby(['segment','season','fuel','weekday','hour']).mean()).round(4)
loadshapes.reset_index(inplace=True)
loadshapes['weekday'] = ["WEEKDAY" if x else "WEEKEND" for x in loadshapes['weekday']]
loadshapes['season'] = [["WINTER","SPRING","SUMMER","FALL"][x-1] for x in loadshapes['season']]
loadshapes.columns = ["building_type","season","fuel","daytype","hour","load"]
loadshapes.set_index(["building_type","season","fuel","daytype","hour"],inplace=True)
loadshapes.to_csv("../building_loadshapes.csv",index=True,header=True)
