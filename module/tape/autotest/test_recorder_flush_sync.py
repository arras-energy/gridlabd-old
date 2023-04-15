import os, sys
import pandas

n = 0
def test_commit(obj,t):
	try:
		srcdir = os.getenv('DIR')
		if not srcdir:
			srcdir = "."
		data = pandas.read_csv(f"{srcdir}/test_recorder_flush_0.csv",index_col='timestamp')
		global n
		f = int(gridlabd.get_value("recorder","flush"))
		if not f:
			f = 1
		n += 1
		assert(int(n/f)==int(len(data)/f))
		return t+1
	except:
		return None