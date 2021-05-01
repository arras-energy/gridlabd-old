import sys
with open('out.csv') as f:
	if '2021-02-01 11:50:00 PST,+500' not in f.read():
		raise TypeError("Modify did not execute")
