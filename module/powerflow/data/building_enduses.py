"""Generate the enduses PNG validation image"""
import pandas
import matplotlib.pyplot as plt

data = pandas.read_csv("building_enduses.csv",index_col=[0,1])

nrows = len(data.index.get_level_values(0).unique())
ncols = 3

fig = plt.figure(figsize=(15,5*nrows/ncols))
nplt = 1
for building_type in data.index.get_level_values(0).unique():

	ax = plt.subplot(int(nrows/ncols),3,nplt)
	
	data.loc[building_type].sum(axis=1).plot(kind='bar')
	ax.grid()
	ax.set_title(building_type)
	ax.set_ylabel('W/sf')

	nplt += 1

plt.tight_layout()
plt.savefig("building_enduses.png")
