import csv 
import os 
import numpy as np

csv_exists = os.path.isfile("test_total_power.csv")

if csv_exists : 
	with open("test_total_power.csv", 'r') as csvfile : 
		#print(data)
		if os.stat("test_total_power.csv").st_size == 0 : 
			exit(1) #empty file 
		else : 
			data_file = list(csv.reader(csvfile))
			for data in data_file[9:]: 
				if  '# end of tape' in data :
					exit(0)
				else : 
					total_power = data[1].split()[0] #magnitude of total_power
					hvac_power = data[2].split()[0] #magnitude of hvac_power
					total_power = float(total_power.split("+")[1]) #magnitude of total_power
					hvac_power = float(hvac_power.split("+")[1]) #magnitude of hvac_power
					if total_power >= hvac_power : 
						continue
					else : 
						exit(1)


			