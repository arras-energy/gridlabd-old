import pandas

COUNTY = "CA_San_Mateo"

building_data = pandas.read_csv("CA_San_Mateo.csv",index_col=["building_type","building_id"])
for column in building_data.columns:
    if column in ["UA","CA","UI","CM","UM","QE","QG"]: #TODO: QO,QV,SA,TS,PZM,PPM,QPM,PZE,PIE,PPE,QZE,QIE,QPE,PPH,QPH
        building_data[column] = building_data[column] / building_data["floor_area"]

building_defaults = building_data.groupby('building_type').mean().round(4)
building_defaults.to_csv("building_defaults.csv",index=True,header=True,float_format='%.4g')
