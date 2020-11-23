def check(context,model,options={}):
	classes = []
	for name, values in model["classes"].items():
		if "module" in values.keys() and values["module"] == __name__.replace("check_",""):
			classes.append(name)
	objects = {}
	for name, values in model["objects"].items():
		if "class" in values.keys() and values["class"] in classes:
			objects[name] = values
