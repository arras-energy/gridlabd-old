import json 
import os 
import sys, getopt
import datetime

config = {"input":"json","output":"glm","type":[]}

def help():
	return """json2glm.py -i <modelinputfile> -o <outputfile> [options...]
    -c|--config              output converter configuration data
    -h|--help                output this help
    -i|--ifile <filename>    [REQUIRED] JSON input file
    -o|--ofile <filename>    [OPTIONAL] GLM output file name
"""

def main():
	filename_json = ''
	filename_glm = ''
	try : 
		opts, args = getopt.getopt(sys.argv[1:],"chi:o:",["config","help","ifile=","ofile="])
	except getopt.GetoptError:
		sys.exit(2)
	if not opts : 
		print('Missing command arguments')
		sys.exit(2)
	for opt, arg in opts:
		if opt in ("-c","--config"):
			print(config)
			sys.exit()
		elif opt in ("-h","--help"):
			print(help())
			sys.exit()
		elif opt in ("-i", "--ifile"):
			filename_json = arg
		elif opt in ("-o", "--ofile"):
			filename_glm = arg

	convert(ifile=filename_json,ofile=filename_glm)

def convert(ifile,ofile) :

	if os.path.exists(ofile):
		os.remove(ofile)
	data = {}
	objects_ignore = ["id", "class", "rank", "clock", "flags"]
	globals_ignore = ['clock', 'timezone_locale', 'starttime', 'stoptime','glm_save_options'] # REMOVE glm_save_options when bug is fixed
	classkeys_ignore = ['object_size', 'trl', 'profiler.numobjs', 'profiler.clocks', 'profiler.count', 'parent']

	with open(ifile,'r') as fr :
		data = json.load(fr)
		assert(data['application']=='gridlabd')
		assert(data['version'] >= '4.0.0')
		# print(data['classes'].keys())
		

	with open(ofile, "a") as fw : 
		fw.write(f"// JSON to GLM Converter Output\n")
		fw.write(f"// InputFile '{ifile}'\n")
		fw.write(f"// CreateDate '{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S %Z')}'\n")

		# clock
		header_str = '\n' + 'clock {'
		tmzone_str = '\n' + '\t' + 'timezone ' + data['globals']['timezone_locale']['value']+';'
		start_str = '\n' + '\t' + 'starttime'+ ' \"' + data['globals']['starttime']['value']+'\";'
		stop_str = '\n' + '\t' + 'stoptime'+' \"'+ data['globals']['stoptime']['value']+'\";'
		end_str = '\n' + '}'
		fw.write('\n\n// CLOCK')	
		fw.write(header_str)
		fw.write(tmzone_str)
		fw.write(start_str)
		fw.write(stop_str)
		fw.write(end_str)

		# modules
		fw.write('\n\n// MODULES') 
		for p_id, p_info in data['modules'].items() : 
			tmp_str = '\n' + 'module ' + p_id + '\n{'
			fw.write(tmp_str)
			for f_id, f_info in data['globals'].items() : 
				if p_id in f_id and '::' in f_id and f_info['access'] == "PUBLIC" and f_info['value']: 
					mod_var = f_id.split('::')
					if mod_var[0] == p_id:
						val_str = '\n\t' + mod_var[1] +' '+ f_info['value'] + ';'
						fw.write(val_str)
			fw.write('\n}')

		# globals
		fw.write('\n\n// GLOBALS')
		for p_id, p_info in data['globals'].items() : 
			if p_info['access'] == "PUBLIC" and p_info['value'] and 'infourl' not in p_id and "::" not in p_id: 
				ifndef_str = '\n' + '#ifndef ' + p_id 
				if 'int' in p_info['type'] or 'double' in p_info['type'] or 'bool' in p_info['type'] or \
				'enumeration' in p_info['type'] or p_info['value']=='NONE' or 'set' in p_info['type'] \
				or 'complex' in p_info['type'] :
					tmp_str = '\n' + 'global ' + p_info['type'] +' '+ p_id +' '+ p_info['value'] +';'
					set_str = '\n' + '#set ' + p_id + '=' + p_info['value']
				else : 
					if p_info['type']=='char1024' :

						tmp_str = '\n' + '#define' +' '+ p_id +'='+ p_info['value']
					else :
						tmp_str = '\n' + 'global ' + p_info['type'] +' '+ p_id +' \"'+ p_info['value'] +'\";'
					set_str = '\n' + '#set ' + p_id + '=' + p_info['value']
				else_str = '\n' + '#else'
				endif_str = '\n' + '#endif //' + p_id
				fw.write(ifndef_str)
				fw.write(tmp_str+else_str+set_str)
				fw.write(endif_str)
			else :
				val_str = '\n' + '// ' + p_id + ' is set to ' + p_info['value']
				fw.write(val_str)	

		# classes
		fw.write('\n\n// CLASSES')
		for p_id, p_info in data['classes'].items() : 
			header_str = ''
			val_str = ''
			for v_id, v_info in data['classes'][p_id].items() :
				if 'flags' in v_info and 'EXTENDED' in v_info['flags']:
					header_str = '\n' + 'class ' + p_id + '\n{'
					val_str = val_str + "\n" + "\t" + v_info['type'] + " " + v_id + ';'
			if header_str : 
				fw.write(header_str)
				fw.write(val_str)	
				fw.write("\n}")

		# schedules
		fw.write('\n\n// SCHEDULES')
		for p_id, p_info in data['schedules'].items() : 
			header_str = '\n' + 'schedule ' + p_id + '\n{'
			fw.write(header_str)
			fw.write(p_info)
			fw.write('\n}' )

		# objects
		fw.write('\n\n// OBJECTS')
		obj_id = []
		if data['objects'] :
			for p_id, p_info in data['objects'].items() : 
				obj_id.append([int(p_info['id']),p_id])
				obj_id_sorted = sorted(obj_id, key=lambda tup: tup[0])
				id_list,ordered_obj_list= zip(*obj_id_sorted)
			for obj_id_sorted in ordered_obj_list : 
				classname = data['objects'][obj_id_sorted]['class']
				classdata = data['classes'][classname]
				# print("CLASSNAME",classname,classdata)
				fw.write(f"\nobject {classname}")
				fw.write("\n{")
				fw.write(f"\n\tname \"{obj_id_sorted.replace(':','_')}\";")
				for v_id, v_info in data['objects'][obj_id_sorted].items() : 
					if v_id not in objects_ignore and v_info:
						# print(v_id)
						if v_id in classdata and type(classdata[v_id]) is dict and classdata[v_id]['type'] == 'object':
							v_str = v_info.replace(':','_')
						else:
							v_str = v_info.replace('"', '\\\"')
						if '\n' in v_info :
							var_str = f"\n\t{v_id} \"\"\"{v_str}\"\"\";"
						else : 
							val_str = f"\n\t{v_id} \"{v_str}\";"
						fw.write(val_str)
				fw.write("\n}")
		fw.write('\n')

if __name__ == '__main__':
	main()

