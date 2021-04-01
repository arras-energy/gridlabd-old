import sys
with open('out.csv') as f:
	if '1999-12-31 16:00:00 PST,+2000' not in f.read():
		raise TypeError("Modify did not execute")