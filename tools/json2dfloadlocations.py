import json 
from datetime import datetime 
import pandas as pd

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
		lat = values["latitude"]
		long = values["longitude"]
		fn = values['from']
		tn = values['to']
		return linktype,linklen,fn,tn,lat,long

	def gatherData(df,objects,root):
		fromdata = objects[root]
		class_name,bustype,busflags,groupid,parent,mre,latitude,longitude = get_load_vals(fromdata)
		df.loc[len(df)]= [root,class_name,bustype,busflags,groupid,parent,"","",mre,latitude,longitude,""]
		
		for meter in find(objects,"groupid",root):
			meterdata = objects[meter]
			class_name,bustype,busflags,groupid,parent,mre,latitude,longitude = get_load_vals(meterdata)
			df.loc[len(df)]= [meter,class_name,bustype,busflags,groupid,parent,"","",mre,latitude,longitude,""]

		for link in find(objects,"from",root):
			linkdata = objects[link]
			if "line" in get_string(linkdata,"class"):
				linktype,linklen,fn,tn,lat,long= get_line_vals(linkdata)
				df.loc[len(df)]= [link,linktype,"","","","",fn,tn,"",lat,long,linklen]
			else:
				linktype = "--o"

			if "to" in linkdata.keys():
				to = linkdata["to"]
				gatherData(df,objects,to)
		return df

	df=pd.DataFrame(columns=["node","class","bustype","busflags","group_id","parent","from","to","load","lat","long","length"])
	for obj in find(objects=data["objects"],property="bustype",value="SWING"):
		df2= gatherData(df,objects=data["objects"],root=obj)
		df.append(df2)
	df.to_csv(output_file,index=False)
	return df
