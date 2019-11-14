import csv

column_names = []
meter_objects = []
meter_power_real = []
meter_power_reactive = []
with open('volt_dump.csv', 'r') as textfile:
	for row, val in enumerate(list(csv.reader(textfile))): 
		if (str(val)).startswith("['#") :
			if row == 0 :
				num_nodes = int((str(val))[58:61])
		if row >= 2 and row <= num_nodes+1 :
				column_names.append(val[0])

for val in column_names : 
	if "meter" in val: 
		meter_objects.append(val)

for val in meter_objects : 
	meter_power_real.append(val + ":measured_real_power")
	meter_power_reactive.append(val + ":measured_reactive_power")
print(meter_power_reactive)


with open('meter_power_real_names.txt', 'w') as textfile : 
	for item in meter_power_real : 
		textfile.write("%s, " %item)