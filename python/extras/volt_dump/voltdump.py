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
with open('volt_dump.csv', 'r') as textfile:
	for row, val in enumerate(list(csv.reader(textfile))): 
		if (str(val)).startswith("['#") :
			time_stamp.append((str(val))[31:54])
			# value_voltA_real.append(time_stamp)
			# value_voltB_real.append(time_stamp)
			# value_voltC_real.append(time_stamp)
			# value_voltA_imag.append(time_stamp)
			# value_voltB_imag.append(time_stamp)
			# value_voltC_imag.append(time_stamp)
		elif row == 1 : 
			continue 
		else : 
			column_names.append(val[0])
			# value_voltA_real.append(val[1])
			# value_voltB_real.append(val[3])
			# value_voltC_real.append(val[5])
			# value_voltA_imag.append(val[2])
			# value_voltB_imag.append(val[4])
			# value_voltC_imag.append(val[6])
print(time_stamp)

# try : # if file exists 
# 	df = pd.read_csv('voltA_real.csv')
# 	with open('voltA_real.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(value_voltA_real)
# except : # if file doesn't exist add column names 
# 	with open('voltA_real.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(column_names)
# 		filewriter.writerow(value_voltA_real)

# try : # if file exists 
# 	df = pd.read_csv('voltB_real.csv')
# 	with open('voltB_real.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(value_voltB_real)
# except : # if file doesn't exist add column names 
# 	with open('voltB_real.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(column_names)
# 		filewriter.writerow(value_voltB_real)

# try : # if file exists 
# 	df = pd.read_csv('voltC_real.csv')
# 	with open('voltC_real.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(value_voltC_real)
# except : # if file doesn't exist add column names 
# 	with open('voltC_real.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(column_names)
# 		filewriter.writerow(value_voltC_real)

# try : # if file exists 
# 	df = pd.read_csv('voltA_imag.csv')
# 	with open('voltA_imag.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(value_voltA_imag)
# except : # if file doesn't exist add column names 
# 	with open('voltA_imag.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(column_names)
# 		filewriter.writerow(value_voltA_imag)

# try : # if file exists 
# 	df = pd.read_csv('voltB_imag.csv')
# 	with open('voltB_imag.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(value_voltB_imag)
# except : # if file doesn't exist add column names 
# 	with open('voltB_imag.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(column_names)
# 		filewriter.writerow(value_voltB_imag)

# try : # if file exists 
# 	df = pd.read_csv('voltC_imag.csv')
# 	with open('voltC_imag.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(value_voltC_imag)
# except : # if file doesn't exist add column names 
# 	with open('voltC_imag.csv', 'a') as csvfile : 
# 		filewriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
# 		filewriter.writerow(column_names)
# 		filewriter.writerow(value_voltC_imag)
