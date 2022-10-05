def check(context,model,options={}):
	"""Verify a powerflow network model"""
	count = 0
	classes = []
	for name, values in model["classes"].items():
		if "module" in values and values["module"] == __name__.replace("check_",""):
			classes.append(name)
	objects = {}
	for name, values in model["objects"].items():
		if "class" in values and values["class"] in classes:
			objects[name] = values
	for method in globals():
		if method.startswith("check_"):
			count += globals()[method](context,objects)
	return count

def check_swing(context,objects):
	"""Verify that one and only one swing bus is present"""
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
		return len(swing)
	return 0

def check_parallel(context,objects):
	"""Verify that no two nodes are connected by more than one link"""
	links = {}
	count = 0
	for name,values in objects.items():
		if "from" in values and "to" in values:
			n = values["from"]
			m = values["to"]
			if n < m:
				link = f"{n}--{m}"
			elif m < n:
				link = f"{m}--{n}"
			else:
				context.warning(f"link '{name}' is circular")
				count += 1
			if link in links:
				context.warning(f"links '{name}' and '{links[link]}' are parallel")
				count += 1
			else:
				links[link] = name
	return count
