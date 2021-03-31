import sys
def on_term(t) : 
	with open('gridlabd.err') as gld_err : 
		assert("WARNING [check]" not in gld_err.read()  )	

	with open('gridlabd.wrn') as gld_wrn : 
		assert("WARNING [check]" not in gld_wrn.read()  )	
