
def on_commit(gld,t) :
	print("%s.on_commit(%d) clock is %s"%(gld.__name__,t,gld.get_global("clock")))
	return True
