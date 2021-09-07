import sys
import gridlabd

def get_age(obj,unit="yr"):
	age = gridlabd.get_value(obj,"age").split();
	if age[1] != unit:
		return gridlabd.convert_unit(float(age[0]),"yr",unit)
	else:
		return float(age[0])

def init(obj,t):
	if get_age(obj) == 0.0:
		gridlabd.error(f"tree.init(obj={obj},t={t}) age not specified")
	return False

def precommit(obj,t):
	print(f"tree.precommit(obj={obj},t={t})",file=sys.stderr)
	return gridlabd.NEVER

def presync(obj,t):
	print(f"tree.presync(obj={obj},t={t})",file=sys.stderr)
	return gridlabd.NEVER

def sync(obj,t):
	print(f"tree.sync(obj={obj},t={t})",file=sys.stderr)
	return gridlabd.NEVER

def postsync(obj,t):
	print(f"tree.postsync(obj={obj},t={t})",file=sys.stderr)
	return t+900

def commit(obj,t):
	print(f"tree.commit(obj={obj},t={t})",file=sys.stderr)
	return gridlabd.NEVER

def finalize(obj,t):
	print(f"tree.finalize(obj={obj},t={t})",file=sys.stderr)
	return gridlabd.NEVER

