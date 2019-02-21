def on_init(t) :
	print("timestamp,x")
	return True

def on_sync(t) :
	print("%s,%s"%(gridlabd.get_global("clock"),gridlabd.get_object("my_test")["x"]))
	return gridlabd.NEVER

def commit(obj,t) :
	print("%s.commit(obj='%s',t=%d) ok" % (gridlabd.__name__,obj,t))
	return gridlabd.NEVER
