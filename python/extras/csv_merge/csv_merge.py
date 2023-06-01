import glob
import pandas
output_file = "multi_recorder_full.csv" #output file name
filelist = glob.glob("output/*.csv") #dataframe list
print(filelist)


dfList = []
for i,filename in enumerate(filelist) : 
	if i!=0 : 
		df = pandas.read_csv(filename,skiprows=7)
		df.drop(df.columns[0],axis=1,inplace=True)
		print(df.columns[0])
	else :
		df = pandas.read_csv(filename,skiprows=7)
	dfList.append(df)

concatDf=pandas.concat(dfList,axis=1,sort=False)
concatDf.to_csv(output_file, index=None)
