from datetime import datetime

def try_everything(obj=None,t=None,context=None):
	if obj:
		a = gridlabd.get_object(obj)
		#print(a)
		b = gridlabd.get_value(obj,'x')
		gridlabd.set_value(obj,'y',b)
		#print('%s(obj=<%s>,t=<%s>): %s'%(context,obj,t,a))
	else:
		c = gridlabd.get('objects')
		d = gridlabd.get('globals')
		#print('%s(t=<%s>): objects=[%s], globals[%s]'%(obj,t,c,d))

# enable a callback when the module is initialized by gridlabd at the start of a simulation
def on_init(t) :
	try_everything(None,t,'on_init')
	return True
def on_precommit(t) :
	print(datetime.fromtimestamp(t))
	try_everything(None,t,'on_precommit')
	return gridlabd.NEVER
def on_presync(t) :
	try_everything(None,t,'on_presync')
	return gridlabd.NEVER
def on_sync(t) :
	try_everything(None,t,'on_sync')
	return gridlabd.NEVER
def on_postsync(t) :
	try_everything(None,t,'on_postsync')
	return gridlabd.NEVER
def on_commit(t) :
	try_everything(None,t,'on_commit')
	return True
def on_term(t):
	try_everything(None,t,'on_term')

def init(obj,t):
	try_everything(obj,t,'init')
	return 0
def precommit(obj,t) :
	try_everything(obj,t,'precommit')
	return gridlabd.NEVER
def presync(obj,t):
	try_everything(obj,t,'presync')
	return gridlabd.NEVER
def sync(obj,t):
	try_everything(obj,t,'sync')
	return gridlabd.NEVER
def postsync(obj,t):
	try_everything(obj,t,'postsync')
	return gridlabd.NEVER
def commit(obj,t) :
	try_everything(obj,t,'commit')
	return gridlabd.NEVER
def finalize(obj,t):
	try_everything(obj,t,'finalize')
	return gridlabd.NEVER
def term(obj,t):
	try_everything(obj,t,'term')
	return gridlabd.NEVER
