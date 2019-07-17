import json 
import os 
import sys, getopt

filename_json = ''
filename_glm = ''
try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["ifile=","ofile="])
except getopt.GetoptError:
	sys.exit(2)
if not opts : 
	print('Missing command arguments')
	print('json2glm.py -i <modelinputfile> -o <outputfile>')
	print('-i : [REQUIRED] Name of the model input file')
	print('-o : [OPTIONAL] JSON dump file name.')
for opt, arg in opts:
	if opt == '-h':
		sys.exit()
	elif opt in ("-i", "--ifile"):
		filename_json = arg
	elif opt in ("-o", "--ofile"):
		filename_glm = arg

if os.path.exists(filename_glm):
	os.remove(filename_glm)
data = {}
objects_ignore = ["id", "class", "rank", "clock", "schedule_skew", \
"rng_state", "heartbeat", "guid", "flags"]
globals_ignore = ['clock', 'timezone_locale', 'starttime', 'stoptime']
classkeys_ignore = ['object_size', 'trl', 'profiler.numobjs', 'profiler.clocks', 'profiler.count', 'parent']

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
			for v_id, v_info in data['classes'][p_id].items() :
				if v_id not in classkeys_ignore : 
					val_str = "\n" + "\t" + v_info['type'] + " " + v_id + ';'
					fw.write(val_str)
			fw.write("\n}")

	return True 

def globals_glm() : 
	global data 
	global fw
	with open(filename_glm, "a") as fw :
		fw.write('\n // GLOBALS')
		for p_id, p_info in data['globals'].items() : 
			if p_info['value'] and p_id not in globals_ignore: 
				tmp_str = '\n' + '#define ' + p_id + "="+ p_info['value'] 
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
			header_str = '\n' + 'object ' + p_info["class"] + '{'
			fw.write(header_str)
			if ':' in p_id : 
				new_name = p_info['class']+'_'+p_info['id']
			else :
				new_name = p_id 
			name_str = '\n' + '\t' + "name " + new_name + ';'
			fw.write(name_str)
			for v_id, v_info in data['objects'][p_id].items() : 
				if v_id not in objects_ignore and v_info:  
					if v_id == 'wh_shape' : # TEMP - GENERALIZE
						val_str = "\n"+ "\t" + v_id + " \"" + v_info + "\";"
					else :
						val_str = "\n"+ "\t" + v_id + " " + v_info + ";"
					fw.write(val_str)
			fw.write('\n}' )
	return True


clock_glm()
# classes_glm() 

modules_glm()
globals_glm()
objects_glm()

fw.close()