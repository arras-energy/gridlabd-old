import json 

filename_json = 'model.json'
filename_glm = 'test_json.glm'
data = {}

with open(filename_json,'r') as fr :
	data = json.load(fr)
	assert(data['application']=='gridlabd')
	assert(data['version'] >= '4.0.0')
	print(data['modules'])
	

with open(filename_glm, "w") as fw : 
	fw.write("// JSON to GLM Converter Output")

def classes_glm() : 
	global data 
	global fw 
	with open(filename_glm, "w") as fw :
		fw.write('// Classes')
		for p_id, p_info in data['classes'].items() : 
			header_str = '\n' + 'class' + p_id + ' {'
			fw.write(header_str)
			# for v_id, v_info in data['classes'][p_id].items() :
			# 	val_str = "\n" + "\t" + v_id + " " + v_info + ';'
			# 	fw.write(val_str)
			fw.write("\n }")

	return True 

def globals_glm() : 
	global data 
	global fw
	with open(filename_glm, "w") as fw :
		fw.write('// GLOBALS')
		for p_id, p_info in data['globals'].items() : 
			if p_info['value'] : 
				tmp_str = '\n' + '#define ' + p_id + " = " + p_info['value'] 
				fw.write(tmp_str)
	return True

# def classes_glm P: 
# 	return True 
def objects_glm() : 
	global data 
	global fw 
	with open(filename_glm, "w") as fw :
		fw.write('// GLOBALS')
		for p_id, p_info in data['objects'].items() : 
			header_str = '\n' + 'object ' + p_id + '{'
			fw.write(header_str)
			for v_id, v_info in data['objects'][p_id].items() : 
				if v_id != "class" : 
					if  v_id != "clock" :
						val_str = "\n"+ "\t" + v_id + " " + v_info + ";"
						fw.write(val_str)
			fw.write('\n }' )
	return True

classes_glm() 
globals_glm()
objects_glm()

fw.close()