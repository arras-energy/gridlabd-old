my_variable = None

def on_init(t) :
	global my_variable
	my_variable = gridlabd.get_global("my_test")
	print("timestamp,x")
	return True

def commit(obj,t) :
	global my_variable
	if my_variable == "yes" :
		my_variable = "no"
	else :
		my_variable = "yes"
	print("%s.commit(obj='%s',t=%d) ok, my_variable = %s" % (gridlabd.__name__,obj,t,my_variable))
	return gridlabd.NEVER
