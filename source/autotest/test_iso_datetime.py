import json 
with open('test_clock.json') as json_file:
	data = json.load(json_file)
	for obj in data['globals'] :
		if obj == "starttime" or obj == "stoptime": 
			if "T" == data["globals"]["starttime"]["value"][10] : 
				exit(0)
			else : 
				print("ISO format was not detected in the JSON")
				exit(1)
