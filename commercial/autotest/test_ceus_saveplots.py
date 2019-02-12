import glob
files = glob.glob("test_ceus_*.csv")
for csvname in files:
	pngname = csvname.replace('.csv','.png')
	with open(csvname,'r') as fh:
		
		import csv
		reader = csv.reader(fh)
		P = []
		Q = []
		for row in reader:
			P.append(float(row[4])/1e6)
			Q.append(float(row[5])/1e6)

		import matplotlib.pyplot as plt
		plt.figure()
		plt.plot(P,label='Real power (MW)')
		plt.plot(Q,label='Reactive power (MVAr)')
		plt.title(csvname)
		plt.xlabel('Hour')
		plt.ylabel('Power')
		plt.legend()
		plt.grid()
		plt.savefig(pngname)
