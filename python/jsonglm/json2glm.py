import json 

filename_json = 'test_1.json'
filename_glm = 'test_json2glm.glm'
data = {}
objects_ignore = ["id", "class", "rank", "clock", "schedule_skew", \
"rng_state", "heartbeat", "guid", "flags"]
globals_ignore = ['clock', 'timezone_locale', 'starttime', 'stoptime']

with open(filename_json,'r') as fr :
	data = json.load(fr)
	assert(data['application']=='gridlabd')
	assert(data['version'] >= '4.0.0')
	# print(data['classes'].keys())
	

with open(filename_glm, "a") as fw : 
	fw.write("// JSON to GLM Converter Output")

def clock_glm() : 
	global data 
	global fw 
	header_str = '\n' + 'clock {'
	tmzone_str = '\n' + '\t' + 'timezone ' + data['globals']['timezone_locale']['value']+';'
	start_str = '\n' + '\t' + 'starttime'+ ' \"' + data['globals']['starttime']['value']+'\";'
	stop_str = '\n' + '\t' + 'stoptime'+' \"'+ data['globals']['stoptime']['value']+'\";'
	end_str = '\n' + '}'
	with open(filename_glm, "a") as fw :
		fw.write('\n // CLOCK')	
		fw.write(header_str)
		fw.write(tmzone_str)
		fw.write(start_str)
		fw.write(stop_str)
		fw.write(end_str)
	return True

def classes_glm() : 
	global data 
	global fw 
	with open(filename_glm, "a") as fw :
		fw.write('\n // CLASSES')
		for p_id, p_info in data['classes'].items() : 
			header_str = '\n' + 'class ' + p_id + ' {'
			fw.write(header_str)
			# for v_id, v_info in data['classes'][p_id].items() :
			# 	val_str = "\n" + "\t" + v_id + " " + v_info + ';'
			# 	fw.write(val_str)
			fw.write("\n }")

	return True 

def globals_glm() : 
	global data 
	global fw
	with open(filename_glm, "a") as fw :
		fw.write('\n // GLOBALS')
		for p_id, p_info in data['globals'].items() : 
			if p_info['value'] and p_id not in globals_ignore: 
				tmp_str = '\n' + '#define ' + p_id + " = " + p_info['value'] 
				fw.write(tmp_str)
	return True

def modules_glm() : 
	global data
	global fw
	with open(filename_glm, "a") as fw :
		fw.write('\n // MODULES') 
		for p_id, p_info in data['modules'].items() :  
			tmp_str = '\n' + 'module ' + p_id + ';'
			fw.write(tmp_str)

# def classes_glm P: 
# 	return True 
def objects_glm() : 
	global data 
	global fw 
	with open(filename_glm, "a") as fw :
		fw.write('\n // OBJECTS')
		for p_id, p_info in data['objects'].items() : 
			header_str = '\n' + 'object ' + p_id + '{'
			fw.write(header_str)
			for v_id, v_info in data['objects'][p_id].items() : 
				if v_id not in objects_ignore and v_info:  
					val_str = "\n"+ "\t" + v_id + " " + v_info + ";"
					fw.write(val_str)
			fw.write('\n }' )
	return True

# classes_glm() 
clock_glm()
# globals_glm()
modules_glm()
objects_glm()

fw.close()