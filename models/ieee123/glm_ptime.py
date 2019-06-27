import os 
import re
from datetime import datetime

TZ_dict = {}

def init_glmptype() : 
	t_tz_list = []
	t_tz_list_ds = []
	tz_offset = []
	tz_file_name = "tzinfo.txt"
	#FIND LOCATION OF tzinfo.txt file
	for root, dirs, files in os.walk(r'/usr/local/share/'):
		for name in files:
			if name == tz_file_name :
				tz_file = os.path.abspath(os.path.join(root, name))
				break 
	with open(tz_file) as f:
		# Search for timestamp format, types available: PST+8PDT, UTC0, GMT0BST, 
		# catch GMT0GMT as faulty
		# Flip the sign for UTC+# 
		# Make sure the digits are correct for 9+ offset values 
		# Catch the decimal point offsets
		regexp = re.compile(r'([A-Z]+)'+r'([+\-]+)?'+r'([0-9.0-9]+)'+r'([A-Z]+)?')
		for line in f:
			match = regexp.match(line)
			if match:
				t_tz_list.append(match.group(1))
				# if match.group(2) == "+" or match.group(2) == "-" :
					#Check for decimal and 9+ values 
				if "." in match.group(3) : 
					hour_offset = match.group(3).split('.')[0]
					min_offset = str(int(6*int(match.group(3).split(".")[1])))
					if float(min_offset) < 10 : 
						min_offset = "0" + min_offset 
				else : 
					hour_offset = match.group(3)
					min_offset = "00"
				if int(hour_offset) < 10 :
					hour_offset = "0" + hour_offset 
				if int(hour_offset) == 0 : 
					hour_offset = "00"
				if match.group(2) != None : # no sign in timestamp, assume + 
					tz_offset.append(match.group(2) + hour_offset + min_offset)
				else :
					tz_offset.append("+" + hour_offset + min_offset)

				t_tz_list_ds.append(match.group(4)) # Daylight savings column 
		# BUILD the dictionary TZ_dict with all timezones within (includes DS)
		# FIRST add the DS timezones 
		for i,item in enumerate(t_tz_list_ds) : 
			if t_tz_list_ds[i] != None : # Discard all the empty DS values 
				offset_adj = (int(tz_offset[i])-100)*(-1)
				TZ_dict[t_tz_list_ds[i]] = format(offset_adj,'05') if offset_adj<0 else "+"+format(offset_adj,'04')
		del TZ_dict['GMT'] # Removing GMT from GMT0GMT - "suspicious rule"
		# Then add the rest 
		for i,item in enumerate(t_tz_list) :
			if item=="EST" and tz_offset[i]=="-1000" : 
				# Using US EST+5 and discrading Australian EST-10 standard	
				continue
			if t_tz_list[i] not in TZ_dict : 
				offset_adj = (int(tz_offset[i]))*(-1)
				TZ_dict[t_tz_list[i]] = format(offset_adj,'05') if offset_adj<0 else "+"+format(offset_adj,'04')	
init_glmptype()
def glm_ptime(t_in) :
	# GLOBALS
	global TZ_dict
	# YYYY-MM-DDTHH:mm:SSZ
	if "Z" in t_in[-1] : 
		t_tz = "UTC"
		t_in=t_in[:-1]
		t_in=t_in.replace("T", " ") + " " + t_tz
	# YYYY-MM-DD HH:mm:SS PST
	# Finds the last space and populated the time zone with the timestamp letters
	else : 
		t_space_index = t_in.rfind(" ")
		t_tz=t_in[t_space_index+1:]
	# YYYY-MM-DDTHH:mm:SS+00:00
	if "T" in t_in[10] :
		if "-" in t_in[19] or "+" in t_in[19] : 
			dt = datetime.strptime(t_in[0:19]+t_in[19]+t_in[20:25].replace(":",""),"%Y-%m-%dT%H:%M:%S%z")
	else :
		# FIND THE SPECIFIED TIMESTAMP IN THE TZ_dict or TZ_DS_dict dictionaries
		t_time = t_in[0:19]
		t = t_time +" "+ TZ_dict[t_tz]
		if "T" in t_in[10] : 
			dt = datetime.strptime(t,"%Y-%m-%dT%H:%M:%S %z")
		elif " " in t_in[10] : 
			dt = datetime.strptime(t,"%Y-%m-%d %H:%M:%S %z")
		else : 
			print("ERROR: Check timestamp synthax")
	return dt
