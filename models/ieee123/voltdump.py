import csv
import pandas as pd
import os 

#def commit_voltdump(obj,t):
time_stamp = []
column_names = []
value_voltA_real = []
value_voltB_real = []
value_voltC_real = []
value_voltA_imag = []
value_voltB_imag = []
value_voltC_imag = []
column_names.append("Time")

#voltdump is a file with appended timstamps to the end of the file
with open('output/volt_dump.csv', 'r') as textfile:
	for row, val in enumerate(list(csv.reader(textfile))): 
		if (str(val)).startswith("['#") :
			time_stamp.append((str(val))[32:55])
		elif row == 1 : 
			continue 
		else : 
			column_names.append(val[0])
print(time_stamp)

