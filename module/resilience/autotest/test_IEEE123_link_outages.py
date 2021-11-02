# test_IEEE123_link_outages.py

import sys, datetime

linklist = []
def on_init(t):
	global linklist
	objlist = gridlabd.get("objects")
	for obj in objlist:
		data = gridlabd.get_object(obj)
		if "from" in data.keys() and "to" in data.keys():
			linklist.append(obj)
	gridlabd.output(f"linklist={linklist}")
	return True

current_object = None
t_next = 0
def on_precommit(t0):
	global t_next
	t_next = (int(t0/3600)+1)*3600
	global current_object
	if not current_object and len(linklist) > 0:
		current_object = linklist[0]
		del linklist[0]
		gridlabd.set_value(current_object,"status","OPEN")
		gridlabd.output(f"on_precommit(t='{datetime.datetime.fromtimestamp(t0)}'): opening link {current_object} --> {datetime.datetime.fromtimestamp(t_next)}")
	elif current_object:
		gridlabd.set_value(current_object,"status","CLOSED")
		gridlabd.output(f"on_precommit(t={datetime.datetime.fromtimestamp(t0)}): closing link {current_object} --> {datetime.datetime.fromtimestamp(t_next)}")
		current_object = None
	else:
		t_next = gridlabd.NEVER
		gridlabd.output(f"on_precommit(t={datetime.datetime.fromtimestamp(t0)}): no links left --> NEVER")
	return t_next

def on_sync(t0):
	global t_next
	return t_next
