def check(context,model):
	classes = []
	for name, values in model["classes"].items():
		if "module" in values.keys() and values["module"] == __name__.replace("check_",""):
			classes.append(name)
	return