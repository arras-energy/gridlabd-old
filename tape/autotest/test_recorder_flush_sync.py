import csv
import time 
import datetime
import os
import json

json_exists = os.path.isfile('gridlabd.json')

#csv_exists = os.path.isfile('test_recorder_flush.csv')
if json_exists : 
	with open("gridlabd.json", "r") as jsonfile : 
		data = json.load(jsonfile)
		clock = data["globals"]["clock"]
		global_clock = datetime.datetime.strptime(clock["value"][:-3].rstrip(), "%Y-%m-%d %H:%M:%S")
	#	print('global clock time : ', global_clock)
		flush_duration = int(int(data["objects"]["recorder:1"]["flush"])/60) #converted to min
		if flush_duration == 0 : 
			flush_duration = 1

	with open('test_recorder_flush.csv', 'r') as textfile:
		temp_row = 0
		first_row_flag = True
		delta = 0
		if os.stat('test_recorder_flush.csv').st_size == 0 : #empty file
			exit(1)
		else : 
			time_csv = list(csv.reader(textfile))[-1][0]
	#		print('csv recorded time :  ',time_csv)
			time_col = datetime.datetime.strptime(time_csv[:-3].rstrip(), "%Y-%m-%d %H:%M:%S")
			delta = abs(global_clock - time_col)
			if delta <= datetime.timedelta(minutes=flush_duration) : 
				#print('success' , delta )
				exit(0)
			else : 
				#print('fail', delta)
				exit(1)
			



