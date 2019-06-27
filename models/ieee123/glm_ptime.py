def glm_ptime(t_in) :
	from datetime import datetime
	if "T" and "+" in t_in : 
		t_temp = t_in[20:25]
		t_temp = t_temp.replace(":","")
		dt = datetime.strptime(t_in[0:19]+"+"+t_temp,"%Y-%m-%dT%H:%M:%S%z")
	elif "T" and "-" in t_in[19] : 
		t_temp = t_in[20:25]
		t_temp = t_temp.replace(":","")
		dt = datetime.strptime(t_in[0:19]+"-"+t_temp,"%Y-%m-%dT%H:%M:%S%z")
	else :
		import os 
		import re
		TZ_dict = {}
		TZ_DS_dict = {}
		t_tz_list = []
		t_tz_list_ds = []
		t_tz_list_ds_temp = []
		t_dir = []
		t_dir_ds = []
		tz_offset = []
		tz_offset_ds = []
		tz_file_name = "tzinfo.txt"
		DS_offset = 0
		if "Z" in t_in[-1] : 
			t_tz = "UTC"
			t_tz_list.append(str('0'))
			t_in=t_in[:-1]
			t_in=t_in.replace("T", " ") + t_tz
		else : 
			t_space_index = t_in.rfind(" ")
			t_tz=t_in[t_space_index+1:]
		

		#FIND LOCATION OF tzinfo.txt file
		for root, dirs, files in os.walk(r'/usr/local/share/gridlabd'):
			for name in files:
				if name == tz_file_name :
					tz_file = os.path.abspath(os.path.join(root, name))
					break 
		if t_tz not in open(tz_file).read() : 
			print("ERROR: No timezone found in tzinfo.txt")
		else : 			
			with open(tz_file) as f:
				regexp = re.compile(r'([A-Z]+)'+r'([+\-]+)?'+r'([0-9.0-9]+)'+r'([A-Z]+)?')
				for line in f:
					match = regexp.match(line)
					if match:
						#TZ_dict = dict(zip(match.group(1), [match.group(2), match.group(3)]))
						t_tz_list.append(match.group(1))
						t_dir.append(match.group(2))
						tz_offset.append(match.group(3))
						t_tz_list_ds_temp.append(match.group(4))
			for i,item in enumerate(t_tz_list_ds_temp) : 
				if t_tz_list_ds_temp[i] != None : 
					t_tz_list_ds.append(t_tz_list_ds_temp[i]) 
					if t_dir[i] == '-' : 
						tz_offset_ds.append(str(int(tz_offset[i])+1))
						t_dir_ds.append(t_dir[i])
					elif t_dir[i] == '+' : 
						tz_offset_ds.append(str(int(tz_offset[i])-1))
						t_dir_ds.append(t_dir[i])
					elif t_dir[i] == None and int(tz_offset[i]) > 0:
						tz_offset_ds.append(str(int(tz_offset[i])-1))
						t_dir_ds.append(t_dir[i])	
					elif t_dir[i] == None and int(tz_offset[i]) == 0:
						t_dir_ds.append("-")
						tz_offset_ds.append("1")	
					elif t_dir[i] == None and int(tz_offset[i]) < 0:
						t_dir_ds.append("-")
						tz_offset_ds.append(str(int(tz_offset[i])+1))
					else : 
						print("ERROR: Check offset direction value of the timestamp")
			t_tz_list_ds = [x for x in t_tz_list_ds_temp if x is not None]
			TZ_dict = {i:[j, k] for i, j, k in zip(t_tz_list, t_dir, tz_offset)}
			TZ_DS_dict = {i:[j, k] for i, j, k in zip(t_tz_list_ds, t_dir_ds, tz_offset_ds)}
			del TZ_DS_dict['GMT'] # Removing GMT from GMT0GMT - "suspicious rule"

			# FIND THE SPECIFIED TIMESTAMP IN THE TZ_dict or TZ_DS_dict dictionaries
			t_time = t_in[0:19]
			if t_tz in TZ_dict :
				if TZ_dict.get(t_tz)[0] == None : 
					t_dir_local = "+"
				else :
					t_dir_local = TZ_dict.get(t_tz)[0]
				if "." in TZ_dict.get(t_tz)[1] : 
					t_hour_offset = TZ_dict.get(t_tz)[1].split(".")[0]
					t_min_offset = str(int(60*int(TZ_dict.get(t_tz)[1].split(".")[1])/10))
				else : 
					t_hour_offset = TZ_dict.get(t_tz)[1]  
					t_min_offset = "00"
			elif t_tz in  TZ_DS_dict : 
				t_dir_local = TZ_DS_dict.get(t_tz)[0]
				if "." in TZ_DS_dict.get(t_tz)[1] : 
					t_hour_offset = TZ_DS_dict.get(t_tz)[1].split(".")[0]
					t_min_offset = str(int(60*int(TZ_DS_dict.get(t_tz)[1].split(".")[1])/10))
				else : 
					t_hour_offset = TZ_DS_dict.get(t_tz)[1]  
					t_min_offset = "00"
			else : 
				print("ERROR : Unknown timestamp.")
			if int(t_hour_offset) > 9 :
				t = t_time + " " + t_dir_local + t_hour_offset + t_min_offset
			else : 
				t = t_time + " " + t_dir_local + "0" + t_hour_offset + t_min_offset
		if "T" in t_in[10] : 
			dt = datetime.strptime(t,"%Y-%m-%dT%H:%M:%S %z")
		elif " " in t_in[10] : 
			dt = datetime.strptime(t,"%Y-%m-%d %H:%M:%S %z")
		else : 
			print("ERROR: Check timestamp synthax")
	return dt