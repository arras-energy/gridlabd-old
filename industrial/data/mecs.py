from openpyxl import load_workbook
from pandas import DataFrame, concat

table5 = load_workbook(filename="MECS2014_Table_5-1.xlsx")["Table 5.1"]
table9 = load_workbook(filename="MECS2014_Table_9-1.xlsx")["Table 9.1"]
table11 = load_workbook(filename="MECS2014_Table_11-1.xlsx")["Table 11.1"]

columns = [
	"NAICS", "DESCRIPTION", "REGION", # index
	"FLOORAREA","SITES","FLOORAREA_PER_SITE","BUILDINGS","BUILDINGS_PER_SITE", # Table 9
	"PURCHASES","TRANSFERS","GENERATION","SALES","NET_DEMAND", # Table 11
	]

#
# Table 9 - Buildings
#
data = []
for values in table9.iter_rows(min_row=18,min_col=1,max_row=98,max_col=7,values_only=True):
	row = [int(values[0])]
	row.extend(values[1:])
	data.append(row)
mecs9 = DataFrame.from_records(data,index=["NAICS"],
			columns = ["NAICS","DESCRIPTION","FLOORAREA[Msf]","SITES[unit]","FLOORAREA_PER_SITE[sf/unit]","BUILDINGS[unit]","BUILDINGS_PER_SITE[unit/unit]"])

valid_codes = mecs9.index

#
# Table 11 - Regional electricity use
#
regions = {
	"NERC":(13,93),
	"NORTHEAST":(97,177),
	"MIDWEST":(181,261),
	"SOUTH":(265,345),
	"WEST":(349,429),
	}
data = []
for region, rows in regions.items():
		for values in table11.iter_rows(min_row=rows[0],min_col=1,max_row=rows[1],max_col=7,values_only=True):
			try:
				index = int(values[0])
				if index in valid_codes:
					row = [index,region]
					row.extend(values[2:])
					data.append(row)
			except:
				pass
mecs11 = DataFrame.from_records(data,index=["NAICS"],
			columns=["NAICS","REGION","PURCHASES[GWh]","TRANSFERS[GWh]","GENERATION[GWh]","SALES[GWh]","NET_DEMAND[GWh]"])

#
# Table 5 - End-use decomposition
#
enduses = {
	"Conventional Boiler Use" : {"B":0.1,"Z":0.9}, # heaters and pumps
	"CHP and/or Cogeneration Process" : {"B":0.5,"C":0.5}, # pumps and fans
	"Process Heating" : {"B":0.1,"Z":0.9}, # heaters and pumps
	"Process Cooling and Refrigeration" : {"A":0.8,"B":0.1,"C":0.1}, # compressors, pumps, and fans
	"Machine Drive" : {"B":1.0}, # machine drives
	"Electro-Chemical Processes" : {"A":0.4,"B":0.05,"C":0.05,"Z":0.5}, # compressors, pumps, fans, and heaters
	"Other Process Use" : {"B":1.0},
	"Facility HVAC (g)" : {"A":0.4,"B":0.05,"C":0.05,"Z":0.5},
	"Facility Lighting" : {"E":0.8,"I":0.2},
	"Other Facility Support" : {"E":1.0},
	"Onsite Transportation" : {"B":1.0},
	"Other Nonprocess Use" : {"D":0.1,"Z":0.8,"I":0.1},
	"End Use Not Reported" : {"D":0.1,"Z":0.8,"I":0.1},
}
data = []
index = None
electrification = [None,None,None,None]
for values in table5.iter_rows(min_row=17,min_col=1,max_row=1819,max_col=4,values_only=True):
	if values[0]:
		try:
			if index:
				row = [index]
				row.extend(electrification)
				for enduse in ["A","B","C","D","E","I","Z"]:
					row.append(enduse_composition[enduse]/enduse_composition["TOTAL"])
				data.append(row)
			index = int(values[0])
			if not index in valid_codes:
				index = None
		except:
			continue
		enduse_composition = {"A":0.0,"B":0.0,"C":0.0,"D":0.0,"E":0.0,"I":0.0,"Z":0.0,"TOTAL":0.0}
	elif not values[1]:
		continue
	tag = values[1].strip()
	if index and values[2] and values[3]:
		try:
			electric = float(values[3])
			if tag == "TOTAL FUEL CONSUMPTION":
				total = float(values[2])
				electrification[0] = electric
				electrification[1] = total/electric*3.412
			elif tag == "End Use Not Reported":
				total = float(values[2])
				electrification[2] = electric
				electrification[3] = total/electric*3.412
				components = enduses[tag]
			elif tag in enduses.keys():
				components = enduses[tag]
			if components:
				for enduse, value in components.items():
					enduse_composition[enduse] += value
					enduse_composition["TOTAL"] += value
				components = None				
		except:
			pass
if index:
	row = [index]
	row.extend(electrification)
	data.append(row)
mecs5 = DataFrame.from_records(data,index=["NAICS"],
			columns=["NAICS","REPORTED_ELECTRIC[GWh]","REPORTED_ELECTRIFICATION[pu]","UNREPORTED_ELECTRIC[GWh]","UNREPORTED_ELECTRIFICATION[pu]",
			"A","B","C","D","E","I","Z"])


mecs = mecs9.join(mecs11).join(mecs5)

mecs.index.name = "NAICS"
mecs.reset_index(inplace=True)
mecs.set_index(["REGION","NAICS"],inplace=True)

print(mecs)

mecs.to_csv("../mecs_data.csv")