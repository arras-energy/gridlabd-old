import gridlabd

test = {
	"shortint" : 21,
	"mediumint" : 31,
	"longint" : 41,
	"real" : 4.321,
	"realimag" : (4.321+5.432j),
	"shortstr" : "another short string",
	"mediumstr" : "another medium string",
	"longstr" : "another long string",
	"speed" : ["100 m/s", 100],
	# "random" : {'type':'normal','a':10, 'b':1,'low':9,'high':11,'update_rate':30, 'state':3376415067},
	"random" : {'type':'normal(10,1)','min':9,'max':11,'refresh':30, 'state':3376415067},
}

convert = {
	"random" : (lambda r: f"type:{r['type']}; min:{r['min']}; max:{r['max']}; refresh:{r['refresh']}; state:{r['state']}")
	}

units = {
	"speed" : [19685.039370078743,"ft/min"]
}

def on_term(t):
	last = None
	for obj in gridlabd.get("objects"):
		oclass = gridlabd.get_value(obj,"class")
		prop = gridlabd.property(obj,"py_object")
		if oclass == "test":
			prop = gridlabd.property(obj,"py_object")
		elif oclass == "check":
			prop = gridlabd.property(obj,obj)
		else:
			continue
		prop.rlock()
		value = prop.get_value()
		initv = prop.get_initial()
		text = str(prop)
		info = repr(prop)
		unit = prop.get_unit()
		prop.unlock()
		if obj in test.keys():
			value = test[obj]
			prop.wlock()
			if type(value) is dict:
				goal = value
				check = value
				init = True
			elif type(test[obj]) is list:
				goal = value[0]
				check = value[1]
				init = False
			else:
				goal = value
				check = value
				init = False
			prop.set_value(goal)
			if init:
				result = prop.get_initial()
			else:
				result = prop.get_value()
			if obj in convert.keys():
				check = convert[obj](check)
			if result != check:
				raise Exception(f"set failed ('{result}' != '{check}')")
			prop.unlock()

			for u,v in units.items():
				if prop.get_name() == u:
					x = prop.convert_unit(v[1])
					if x != v[0]:
						raise Exception(f"unit conversion failed ({x} != {u[0]}")
		if prop == last:
			raise Exception("property eq comparison failed unexpectedly")
		if prop != prop:
			raise Exception("property ne comparison failed unexpectedly")
		ok = None
		try:
			prop < last
			prop > last
			prop <= last
			prop >= last
			ok = False
		except:
			ok = True
			pass
		if ok == None:
			raise Exception("property lt/le/gt/ge test inconclusive")
		elif ok == False:
			raise Exception("property lt/le/gt/ge succeeded unexpectedly")
		last = prop
			