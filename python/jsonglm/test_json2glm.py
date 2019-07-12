# gridlabd -c test_json.glm -o test_1.json
# python3 json2glm.py
# gridlabd -c test_json2glm.glm -o test_2.json
file_1 = '/Users/alyona_slac/gridlabd/grip/components/grip_sim_runner/simulation_models/virtual_islanding/m.json'
file_2 = '/Users/alyona_slac/gridlabd/grip/components/grip_sim_runner/simulation_models/virtual_islanding/m2.json'
import json
with open(file_1, 'r') as f1 : 
	data1 = json.load(f1)
with open(file_2, 'r') as f2 : 
	data2 = json.load(f2)

def comp_dict(d1, d2) :
	d1_new = {}
	d2_new = {}
	values_ignored = 0
	ignore_values = ['randomseed', 'randomstate', 'guid', 'modelname', 'savefile', 'command_line', 'rank', 'topological_parent', 'rng_state']
	if isinstance(d1,dict) and isinstance(d2, dict) :
		for f_id, f_info in d1.items() : # walking through the dictionary keys and values
			print(f_id)
			if f_id in d2 and f_id not in ignore_values: # check that the key exists in both files
				if isinstance(f_info, dict) : # check whether dict is nested
					len_dict = len(f_info) # nested 
					# for p_id, p_info in d1.items() : 
					d1_new = d1[f_id]
					d2_new = d2[f_id]
					comp_dict(d1_new, d2_new)
				else : 
					if f_info != d2[f_id] : # compare values
						print('ERROR: Values do not match : ', f_id)
						print(f_info, d2[f_id])
			elif f_id in ignore_values : 
				values_ignored = values_ignored + 1
			else : 
				print('ERROR: Files have mis-matched keys : ', f_id)
				# return False 
	else : 
		if d1 != d2 : 
			print('ERROR: Values do not match : ', d1, d2)
			print(f_info, d2[f_id])
			# return False # values do not match 

comp_dict(data1, data2)

f1.close()
f2.close()

