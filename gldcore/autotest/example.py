import gridlabd

prop = None

def on_init(t):
	global prop
	prop = gridlabd.property('my_example','my_real')
	return True

def on_sync(t):
	prop.get_value()
	# # The above usage is 5-10 times faster than the usage below
	# gridlabd.get_value('my_example','my_real')
	return t+10
