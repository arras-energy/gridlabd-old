import gridlabd

def my_callback(data):
	print("my_callback(data) -> 1")
	return 1

gridlabd.add_callback("my_callback",my_callback)
