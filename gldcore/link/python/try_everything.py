from datetime import datetime

def try_everything(obj=None,t=None,context=None):
	if obj:
		a = gridlabd.get_object(obj)
		b = gridlabd.get_value(obj,'x')
		c = gridlabd.set_value(obj,'y',b)
		d = gridlabd.set_global('test',str(t))
		return
	else:
		a = gridlabd.get('objects')
		b = gridlabd.get('globals')
		c = gridlabd.get('classes')
		d = gridlabd.get('modules')
		e = gridlabd.get('transforms')
		f = gridlabd.get('schedules')
		return

# module event handlers
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

# object event handlers
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
