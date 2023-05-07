"""Generate the occupancy PNG validation image"""
import pandas
import matplotlib.pyplot as plt

data = pandas.read_csv("building_occupancy.csv",index_col=[0,1])

nrows = len(data.index.get_level_values(0).unique())
ncols = len(data.index.get_level_values(1).unique())

fig = plt.figure(figsize=(7.5*ncols,5*nrows))

nplt = 0
for building_type in data.index.get_level_values(0).unique():
	hours = {}
	for daytype in data.loc[building_type].index.get_level_values(0).unique():
		spec = data.loc[building_type,daytype]
		hours[daytype] = 24*[spec.unoccupied]
		for hr in range(int(spec.start),int(spec.stop)):
			hours[daytype][hr] = spec.occupied

		plt.subplot(nrows,ncols,nplt+1)
		plt.plot(hours[daytype])
		plt.title(f"{building_type} {daytype}")
		plt.grid()
		plt.xticks(list(range(0,25,3)))
		plt.xlim([0,24])
		plt.ylim([0,1.1])

		nplt += 1

plt.tight_layout()
plt.savefig("building_occupancy.png")