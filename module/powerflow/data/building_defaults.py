"""Generate the defaults PNG validation image"""
import pandas
import matplotlib.pyplot as plt

data = pandas.read_csv("building_defaults.csv",index_col=[0])

cols = {
	"THERMAL" : ['UA','CA','UI','CM','UM','DF','K','QE','QG','QO','QV','SA','TS'],
	"ELECTRICAL" : ['PZM','PPM','QPM','PZE','PIE','PPE','QZE','QIE','QPE','PPH','QPH'],
	}
nrows = len(data.index.get_level_values(0).unique())
ncols = len(cols)

fig = plt.figure(figsize=(7.5*ncols,5*nrows))
nplt = 1
for building_type in data.index.get_level_values(0).unique():
	for ftype,fields in cols.items():	
		ax = plt.subplot(nrows,2,nplt)
		plt.bar(x=fields,height=data[fields].loc[building_type])
		if ftype == "THERMAL":
			ax.set_yscale('log')
		plt.title(f"{building_type} {ftype}")
		plt.grid()

		nplt += 1

plt.tight_layout()
plt.savefig("building_defaults.png")
