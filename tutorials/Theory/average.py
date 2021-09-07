
object_list = []
y = 0.0
n = 0

def on_init(t):
	object_list = gridlabd.get("objects")
	gridlabd.output(f"{len(object_list)} objects found")
	return True

def sync(obj,t):
	global y
	global n
	y += float(gridlabd.get_value(obj,"x"))
	n += 1
	return gridlabd.NEVER

def on_term(t):
	if n > 0:
		gridlabd.output(f"average = {y/n}")
	gridlabd.output(f"n = {n}")
