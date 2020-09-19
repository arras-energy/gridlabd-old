import gridlabd

def on_init(t):
	for name in ['my_list','my_value','my_string','my_real']:
		prop = gridlabd.property('my_example',name)
		print(f"{repr(prop)}={str(prop)}")
	return True
