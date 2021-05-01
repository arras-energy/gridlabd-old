def check(context,model,options={}):
	count = 0
	classes = []
	for name, values in model["classes"].items():
		if "module" in values.keys() and values["module"] == __name__.replace("check_",""):
			classes.append(name)
	objects = {}
	for name, values in model["objects"].items():
		if "class" in values.keys() and values["class"] in classes:
			objects[name] = values
	count += check_swing(context,objects)
	return count

def check_swing(context,objects):
	swing = []
	for name,values in objects.items():
		oclass = values["class"]
		if oclass == "node":
			if not "bustype" in values.keys():
				context.warning(f"object '{name}' missing bustype")
			elif values["bustype"] == "SWING":
				swing.append(name)
	if not swing:
		context.warning("powerflow network missing swing bus")
		return 1
	elif len(swing) > 1:
		context.warning(f"powerflow network has {len(swing)} swing busses ({', '.join(swing)})")
		return 1
	return 0
