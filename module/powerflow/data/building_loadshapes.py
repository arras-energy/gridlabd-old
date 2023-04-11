"""Generate the loadshapes PNG validation image"""
import pandas
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec

data = pandas.read_csv("building_loadshapes.csv",index_col=[0,1,2,3,4]).sort_index()

building_types = data.index.get_level_values(0).unique()
seasons = data.index.get_level_values(1).unique()
fuels = data.index.get_level_values(2).unique()
daytypes = data.index.get_level_values(3).unique()

nrows = len(building_types)
ncols = len(seasons)

fig,axes = plt.subplots(figsize=(5*ncols,3*nrows),nrows=nrows,ncols=ncols)

column = {"WINTER":0,"SPRING":1,"SUMMER":2,"FALL":3}
style = {"GAS":"r","ELECTRIC":"b"}
color = {"WEEKDAY":"-","WEEKEND":":"}

for row,building_type in enumerate(building_types):

	for season in data.loc[building_type].index.get_level_values(0).unique():

		ax = axes[row,column[season]]
		leg = []
		for fuel in data.loc[(building_type,season)].index.get_level_values(0).unique():
			for daytype in data.loc[(building_type,season,fuel)].index.get_level_values(0).unique():
				pl = (data.loc[(building_type,season,fuel,daytype)]).plot(ax = ax,
					style = style[fuel]+color[daytype],
					legend = (row==0 and column[season]==0))
				leg.append(f"{daytype} {fuel}")
		if row==0 and column[season]==0:
			ax.legend(leg)
		ax.grid()
		ax.set_title(f"{building_type} {season}")
		ax.set_ylabel('W/sf')
		ax.set_xlabel(None)
		ax.set_xlim([0,24])
		ax.set_xticks(range(0,25,3))

plt.tight_layout()
plt.savefig("building_loadshapes.png")
