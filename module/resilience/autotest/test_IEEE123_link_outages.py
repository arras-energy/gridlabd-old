# test_IEEE123_link_outages.py

import sys

linklist = None
def on_init(t):
	global linklist
	objlist = gridlabd.get("objects")
	linklist = []
	for obj in objlist:
		data = gridlabd.get_object(obj)
		if "from" in data.keys() and "to" in data.keys():
			linklist.append(obj)
	print(linklist,file=sys.stderr)
	return True

current_object = None
t_next = 0
def on_precommit(t0):
	global t_next
	t_next = (int(t0/3600)+1)*3600
	global current_object
	if current_object == None and len(linklist) > 0:
		current_object = linklist[0]
		del linklist[0]
		gridlabd.set_value(current_object,"status","OPEN")
		print(f"on_precommit(t={t0}): opening object {current_object} --> {t_next}",file=sys.stderr)
	elif current_object != None:
		gridlabd.set_value(current_object,"status","CLOSED")
		current_object = None
		print(f"on_precommit(t={t0}): closing object {current_object} --> {t_next}",file=sys.stderr)
	else:
		t1 = gridlabd.NEVER
		print(f"on_precommit(t={t}): no objects left --> {t_next}",file=sys.stderr)
	return t_next

def on_sync(t0):
	global t_next
	return t_next