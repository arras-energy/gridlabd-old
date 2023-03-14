#!/usr/local/bin/python3
import sys
assert(sys.version_info.major>2)
import traceback
import gridlabd

def callstr(argv) :
	return ("%s(%s)" % (argv[0],",".join(argv[1:])))

for arg in sys.argv[1:] :
	gridlabd.command(arg)
try :
	rc = (gridlabd.start('wait') & 255 )
	if ( rc > 128 & rc < 255 ) :
		print("%s: signal %d caught" % (callstr(sys.argv),rc&127))
	elif rc == 128 :
		print("%s: unknown signal caught" % callstr(sys.argv))
except:
	traceback.print_exc()
	rc = 255
print("%s -> exit %d" % (callstr(sys.argv),rc))
sys.exit(rc)
