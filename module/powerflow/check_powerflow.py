def check(context,model):
	classes = []
	swing = []
	for name, values in model["classes"].items():
		if "module" in values.keys() and values["module"] == __name__.replace("check_",""):
			classes.append(name)
	for name, values in model["objects"].items():
		if not "class" in values.keys():
			context.warning(f"object '{name}' missing class")
			continue
		oclass = values["class"]
		if not oclass in classes:
			continue
		if oclass == "node":
			if not "bustype" in values.keys():
				context.warning(f"object '{name}' missing bustype")
			elif values["bustype"] == "SWING":
				swing.append(name)
	if not swing:
		context.warning("powerflow network missing swing bus")
	elif len(swing) > 1:
		context.warning(f"powerflow network has {len(swing)} swing busses ({', '.join(swing)})")
	return