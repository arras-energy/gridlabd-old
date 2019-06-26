def glm_ptime(t_in) :
	from datetime import datetime
	if "T" and "+" in t_in : 
		t_temp = t_in[20:25]
		t_temp = t_temp.replace(":","")
		print(t_in[0:19]+t_temp)
		dt = datetime.strptime(t_in[0:19]+"+"+t_temp,"%Y-%m-%dT%H:%M:%S%z")
	else :
		import os 
		import re
		tz_file_name = "tzinfo.txt"
		if "Z" in t_in[-1] : 
			t_tz = "UTC"
			t_in=t_in[:-1]
			t_in=t_in.replace("T", " ") + t_tz
			print(t_in)
		else : 
			t_tz=t_in[20:24]
		t_tz_list = []

		#FIND LOCATION OF tzinfo.txt file
		for root, dirs, files in os.walk(r'/usr'):
			for name in files:
				if name == tz_file_name :
					tz_file = os.path.abspath(os.path.join(root, name))
					break 
		if t_tz not in open(tz_file).read() : 
			print("ERROR: No timezone found in tzinfo.txt")
		else : 
			if "S" or "UTC" in t_tz[1] : 
				regexp = re.compile(t_tz+r'.*?([0-9]+)')
			elif "D" in t_tz[1] :
				regexp = re.compile(r'.*?([0-9]+)'+t_tz)
			with open(tz_file) as f:
				for line in f:
					match = regexp.match(line)
					if match:
						t_tz_list.append(match.group(1))
		t_tz_offset = t_tz_list.pop()
		if "S" or "UTC" in t_tz[1] : 
			t=t_in[0:-4] + " +" + "0" + t_tz_offset + "00"
		elif "D" in t_tz[1] : 
			t=t_in[0:-4] + " +" + "0" + str(int(t_tz_offset)-1) + "00"
		
		if "T" in t_in[10] : 
			dt = datetime.strptime(t,"%Y-%m-%dT%H:%M:%S %z")
		elif " " in t_in[10] : 
			dt = datetime.strptime(t,"%Y-%m-%d %H:%M:%S %z")
		else : 
			print("ERROR: Check timestamp synthax")
	return dt

