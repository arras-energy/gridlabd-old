
def on_init(t) :
	print("%s.on_init(%d) clock is %s"%(gridlabd.__name__,t,gld.get_global("clock")))
	return gridlabd.NEVER

def on_precommit(t) :
	print("%s.on_preccommit(%d) clock is %s"%(gridlabd.__name__,t,gld.get_global("clock")))
	return gridlabd.NEVER

def on_presync(t) :
	print("%s.on_presync(%d) clock is %s"%(gridlabd.__name__,t,gld.get_global("clock")))
	return gridlabd.NEVER

def on_sync(t) :
	print("%s.on_sync(%d) clock is %s"%(gridlabd.__name__,t,gld.get_global("clock")))
	return gridlabd.NEVER

def on_postsync(t) :
	print("%s.on_postsync(%d) clock is %s"%(gridlabd.__name__,t,gld.get_global("clock")))
	return gridlabd.NEVER

def on_commit(t) :
	print("%s.on_commit(%d) clock is %s"%(gridlabd.__name__,t,gridlabd.get_global("clock")))
	return True

def on_term(t) :
	print("%s.on_term(%d) clock is %s"%(gridlabd.__name__,t,gld.get_global("clock")))
	return None

class MyClass :

	def __init__(self) :
		value = 0.0

	def precommit(self,t) :
		return gridlabd.NEVER

	def presync(self,t) :
		return gridlabd.NEVER

	def sync(self,t) :
		return gridlabd.NEVER

	def postsync(self,t) :
		return gridlabd.NEVER

	def precommit(self,t) :
		return True

	def __del__(self):
		return
