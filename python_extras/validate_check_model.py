import sys
import gridlabd
def on_term(t) : 
	with open('gridlabd.err') as gld_err : 
		if "ERROR [check]" not in gld_err.read():
			gridlabd.error("check error detected")

	with open('gridlabd.wrn') as gld_wrn : 
		if "WARNING [check]" not in gld_wrn.read():
			gridlabd.error("check warning detected")
