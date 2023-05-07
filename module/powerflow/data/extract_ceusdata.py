"""Extract CEUS data

This script generates the CEUS loadshapes from the CEUS data provided in
the worksheets in the CEUS folder. The output is a CSV table containing 
load in Watts per square foot 
"""

import sys, os
import xlrd
import pandas
import json
from datetime import datetime

segment_code = {
    "CA_REST" : "RESTAURANT",
    "CA_RETL" : "RETAIL",
    "CA_HLTH" : "HEALTHCARE",
    "CA_SCHL" : "EDUCATION",
    "CA_LODG" : "LODGING",
    "CA_SOFF" : "SMALLOFFICE",
    "CA_LOFF" : "LARGEOFFICE",
    "CA_GROC" : "GROCERY",
}
enduse_code = {
    "Heating" : ["Heat","HEATING"],
    "Cooling" : ["Cool","COOLING"],
    "Ventilation" : ["Vent","VENTILATION"],
    "Water Heating" : ["HotWater","HOTWATER"],
    "Cooking" : ["Cook","COOKING"],
    "Refrigeration" : ["Refrig","REFRIGERATION"],
    "Exterior Lighting" : ["ExtLight","OUTDOORLIGHTING"],
    "Interior Lighting" : ["IntLight","LIGHTING"],
    "Office Equipment" : ["OffEquip","OFFICEEQUIPMENT"],
    "Miscellaneous" : ["Misc","MISCELLANEOUS"],
    "Process" : ["Process","PROCESS"],
    "Motors" : ["Motors","MOTORS"],
    "Air Compressors" : ["AirComp","AIRCOMPRESSION"],
}
enduse_ignore = ["Heat","Cool"]
fuel_code = {
    "Elec" : "ELECTRIC",
    "Gas" : "GAS"
}

floorarea = {}
data = []
for file in os.listdir("CEUS"):
    if file.endswith(".xls"):
        print(f"Processing {file}",end='...',flush=True)
        book = xlrd.open_workbook(f"CEUS/{file}")

        seginfo = book["ctrlSEGINFO"]
        segment = str(seginfo[0][1].value)
        year = int(seginfo[2][1].value)
        
        summary = book["Summary"]
        floorarea[segment] = {
            "Elec" : dict([(enduse_code[str(summary[row][1].value)][0],{"Floor area":float(summary[row][2].value)}) for row in range(5,18)]),
            "Gas" : dict([(enduse_code[summary[row][1].value][0],{"Floor area":float(summary[row][2].value)*3.412}) for row in range(26,32)]) # include conversion of kBTU to kWh
        }
        # json.dump(data,sys.stdout,indent=4)

        # load enduse data
        load = pandas.read_excel(f"CEUS/{file}",sheet_name="expEndUse8760")
        load.set_index(["SegID","EndUse","Fuel","Mth","Dy"],inplace=True)
        load.columns = [str(n) for n in range(24)]

        series = pandas.DataFrame(load.stack())
        series.columns = ["load"]
        series.index.names = ["segment","enduse","fuel","month","day","hour"]

        series.reset_index(inplace=True)
        series.set_index(["segment","enduse","fuel"],inplace=True)
        print("ok",flush=True)

        # print(segment,"data:",series.loc[segment])
        # print(segment,"enduse:",series.loc[segment].index.get_level_values(0).unique())

        for enduse in series.loc[segment].index.get_level_values(0).unique():
            # print(segment,enduse,"fuel:",series.loc[segment,enduse].index.get_level_values(0).unique())
            # print(segment,enduse,"data:",series.loc[segment,enduse])
            for fuel in series.loc[segment,enduse].index.get_level_values(0).unique():
                # print(series.loc[segment,enduse,fuel]['load'])
                if enduse in enduse_ignore:
                    series.loc[(segment,enduse,fuel),'load'] = 0.0
                elif fuel in floorarea[segment] and enduse in floorarea[segment][fuel]:
                    series.loc[(segment,enduse,fuel),'load'] = (series.loc[(segment,enduse,fuel),'load'].astype('float') / floorarea[segment][fuel][enduse]["Floor area"]).round(4)
                else:
                    print("WARNING: no floor area given for  {fuel} {segment} {enduse}",file=sys.stderr)

        series.reset_index(inplace=True)
        series.set_index(["segment","enduse","fuel","month","day","hour"],inplace=True)
        data.append(series)

# data.columns = [segment_code[x[1]] for x in data.columns]
print(f"Preparing output",end='...',flush=True)
data = pandas.concat(data)
data.reset_index(inplace=True)
data['segment'] = [segment_code[x] for x in data['segment']]
data['enduse'] = [dict(enduse_code.values())[x] for x in data['enduse']]
data['fuel'] = [fuel_code[x] for x in data['fuel']]
data['datetime'] = pandas.DatetimeIndex([f"2002-{x['month']}-{x['day']} {x['hour']}:00" for n,x in data.iterrows()])
print("ok")
data.to_csv("ceus_loadshapes.csv",index=False,header=True)

# json.dump(data,sys.stdout,indent=4)
