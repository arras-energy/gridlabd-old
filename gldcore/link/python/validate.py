import sys, os
assert(sys.version_info.major>2)

show_failure = True

n_tested = 0
n_passed = 0

def validate(dir=None) :
	if not dir :
		dir = os.getcwd()
	#print(dir)
	items = sorted(os.listdir(dir))
	#print(items)
	if 'validate.no' in items :
		#print('Skipping %s (validate.no found)' % dir)
		return;
	for item in items :
		path = os.path.join(dir,item)
		#print(path)
		if item == 'autotest' and os.path.isdir(path) :
			autotest(path)
		elif os.path.isdir(path) :
			validate(path)
	for dirname, dirnames, filenames in os.walk(dir) :
		for subdir in dirnames :
			if subdir == 'autotest' :
				autotest("/".join([dirname,subdir]))

def autotest(dir) :
	#print("Entering %s..." % dir)
	items = sorted(os.listdir(dir))
	if 'validate.no' in items :
		#print("Validation suppressed by 'validate.no'")
		return 
	for file in items :
		path = "/".join([dir,file])
		workdir = path[0:-4]
		#print(path)
		if not os.path.isdir(path) and file[0:5] == 'test_' and file[-4:] == '.glm' :
			runtest(workdir,file)

def runtest(workdir,glmname) :
	global n_tested, n_passed, exename
	n_tested += 1
	sys.stdout.flush()
	owd = os.getcwd()
	os.system("mkdir -p " + workdir)
	os.chdir(workdir)
	os.system("rm -f *")
	os.system("cp ../%s ." % glmname)
	rc = ( os.system("%s %s 1>gridlabd.out 2>&1" % (exename,glmname)) >> 8)
	if rc == 255 :
		print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
		if show_failure:
			os.system("cat gridlabd.out")
	elif not "_opt" in glmname :
		if rc == 0 :
			if "_err" in glmname or "_exc" in glmname :
				print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
				if show_failure:
					os.system("cat gridlabd.out")
			else :
				#print("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
				n_passed += 1
		else :
			if "_err" in glmname or "_exc" in glmname :
				#print("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
				n_passed += 1
			else :
				print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
				if show_failure:
					os.system("cat gridlabd.out")
	else :
		#print("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
		n_passed += 1
	os.chdir(owd)


if __name__ == '__main__':
	global exename
	owd = os.getcwd()
#	exename = "gridlabd"
	exename = "/".join([owd,"gridlabd_python.py"])
	os.chdir('../../..')
	print("Validating %s, please wait..." % os.getcwd())
	validate(os.getcwd())
	print("Tested.... %d" % n_tested)
	print("Passed.... %d (%f%%)" % (n_passed,n_passed/n_tested*100))
	os.chdir(owd)