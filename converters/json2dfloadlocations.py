import json 
from datetime import datetime 
import pandas as pd
import os

def convert(input_file,output_file=None, options={}):

	if output_file == '':
		if input_file[-5:] == ".json":
			output_file = input_file[:-5] + ".csv" 
			output_file2= input_file[:-5] + "2.csv" 
		else: 
			output_file = input_file + ".csv"

	with open(input_file,"r") as f :
		data = json.load(f)
		assert(data['application']=='gridlabd')
		assert(data['version'] >= '4.2.0')
	
	def find(objects,property,value):
		result = []
		for name,values in objects.items():
			if property in values.keys() and values[property] == value:
				result.append(name)
		return result

	def get_string(values,key):
		return values[key]

	def get_complex(values,prop):
		return complex(get_string(values,prop).split(" ")[0].replace('i','j'))

	def get_real(values,prop):
		return get_complex(values,prop).real

	def get_load_vals(values):
		classname = values["class"]
		bt = values["bustype"]
		bf = values["busflags"]

		if "groupid" in values.keys():
			gid = values["groupid"]
		else:
			gid=""

		if "latitude" in values.keys():
			lat = values["latitude"]
			long = values["longitude"]
		else:
			lat,long= "",""
		
		if values['class'] == "triplex_meter":
			parent = values['parent']
			mre=get_real(values,"measured_real_energy")
		else:
			parent, mre = "",""

		return classname,bt,bf,gid,parent,mre,lat,long

	def get_line_vals(values):
		linktype = values["class"]
		linklen = get_real(values,"length")
		lat = ""
		long= ""
		if "latitude" in values.keys():
			lat = values["latitude"]
			long = values["longitude"]
		fn = values['from']
		tn = values['to']
		return linktype,linklen,fn,tn,lat,long

	def get_pole_mount(values):
		linktype = values["class"]
		equipment = values["equipment"]
		height = values['height']
		offset = values['offset']
		polespacing = values['pole_spacing']
		windSpeed = values['wind_speed']
		windDir = values['wind_direction']
		return linktype,equipment, height, offset, polespacing, windSpeed, windDir

	def gatherData(df,objects,root):
		fromdata = objects[root]
		class_name,bustype,busflags,groupid,parent,mre,latitude,longitude = get_load_vals(fromdata)
		df.loc[len(df)]= [root,class_name,bustype,busflags,groupid,parent,"","",mre,latitude,longitude,"",""]
		
		for meter in find(objects,"groupid",root):
			meterdata = objects[meter]
			class_name,bustype,busflags,groupid,parent,mre,latitude,longitude = get_load_vals(meterdata)
			df.loc[len(df)]= [meter,class_name,bustype,busflags,groupid,parent,"","",mre,latitude,longitude,"",""]

		for link in find(objects,"from",root):
			linkdata = objects[link]
			if "line" in get_string(linkdata,"class"):
				linktype,linklen,fn,tn,lat,long= get_line_vals(linkdata)
				df.loc[len(df)]= [link,linktype,"","","","",fn,tn,"",lat,long,linklen,""]
				#since it is a line, then need to go to the "pole mount to get additional info"
				for equipment in find(objects, "equipment", link):
					equipmentData = objects[equipment]
					linktype,equipment, height, offset, polespacing, windSpeed, windDir = get_pole_mount(equipmentData)
					df.loc[len(df)]= [link,linktype,"","","",equipment,"","","","","","",height]
					
			else:
				linktype = "--o"
			if "to" in linkdata.keys():
				to = linkdata["to"]
				gatherData(df,objects,to)
		return df

	df=pd.DataFrame(columns=["node","class","bustype","busflags","group_id","parent","from","to","load","lat","long","length","height"])
	for obj in find(objects=data["objects"],property="bustype",value="SWING"):
		df2= gatherData(df,objects=data["objects"],root=obj)
		df.append(df2)
	df.to_csv(output_file,index=False)
	return df


# ### Load and Convert JSON for IEEE123_pole.json
# path= os.path.join(os.getcwd(),'example/IEEE123_pole.json')
# # path= os.path.join(os.getcwd(),'tools/example/IEEE123_pole.json') # uncomment in debug mode
# datapole = convert(input_file=path,output_file='')
