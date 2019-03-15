import sys, os
assert(sys.version_info.major>2)
import traceback
import timeit

show_failure = True
show_verbose = False
show_debug = False
dry_run = False

n_tested = 0
n_passed = 0

def debug(msg) :
	if show_debug :
		print("DEBUG: %s" % msg)

def verbose(msg) :
	if show_verbose :
		print("... %s" % msg)

def validate(dir=None) :
	if not dir :
		dir = os.getcwd()
	debug("validate(dir=%s)" % dir)
	debug("validate: dir=%s"%dir)
	items = sorted(os.listdir(dir))
	debug("validate: items=%s"%items)
	if 'validate.no' in items :
		debug('validate: skipping %s (validate.no found)' % dir)
		return;
	for item in items :
		path = os.path.join(dir,item)
		debug("validate path=%s"%path)
		if not os.path.isdir(path) :
			continue
		elif item == "." or item ==".." :
			continue
		elif item == 'autotest' and os.path.isdir(path) :
			autotest(path)
		else :
			validate(path)

def autotest(dir) :
	debug("autotest(dir=%s)" % dir)
	items = sorted(os.listdir(dir))
	debug("autotest: items=%s" % items)
	if 'validate.no' in items :
		debug("autotest: skipping %s (validate.no found)" % dir)
		return 
	for file in items :
		path = "/".join([dir,file])
		workdir = path[0:-4]
		debug("autotest: path=%s"%path)
		if not os.path.isdir(path) and file[0:5] == 'test_' and file[-4:] == '.glm' :
			runtest(workdir,file)

def run_command(cmd) :
	global dry_run
	if dry_run :
		print("dryrun%% %s" % cmd)
		return 0
	else :
		debug("run_command: cmd=%s" % cmd)
		return (os.system(cmd) >> 8)

def runtest(workdir,glmname) :
	global n_tested, n_passed, exename
	n_tested += 1
	sys.stdout.flush()
	owd = os.getcwd()
	run_command("mkdir -p " + workdir)
	os.chdir(workdir)
	run_command("rm -f *")
	run_command("cp ../%s ." % glmname)
	rc = run_command("%s %s 1>gridlabd.out 2>&1" % (exename,glmname))
	if not dry_run :
		if rc == 255 :
			print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
			if show_failure:
				run_command("cat gridlabd.out")
		elif not "_opt" in glmname :
			if rc == 0 :
				if "_err" in glmname or "_exc" in glmname :
					print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
					if show_failure:
						run_command("cat gridlabd.out")
				else :
					verbose("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
					n_passed += 1
			else :
				if "_err" in glmname or "_exc" in glmname :
					verbose("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
					n_passed += 1
				else :
					print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
					if show_failure:
						run_command("cat gridlabd.out")
		else :
			verbose("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
			n_passed += 1
	os.chdir(owd)


if __name__ == '__main__':
	start = timeit.default_timer()
	global exename
	owd = os.getcwd()
	exename = "/".join([owd,"gridlabd_python.py"])
	os.chdir('../../..')
	print("Validating %s, please wait..." % os.getcwd())
	# TODO: this needs to be read from prefix rather than using system default
	os.putenv("GLPATH","/usr/local/lib/gridlabd:/usr/local/share/gridlabd")
	os.putenv("CXXFLAGS","-w -O3 -g -I/usr/local/share/gridlabd -I/usr/local/include/gridlabd")
	validate(os.getcwd())
	os.chdir(owd)
	stop = timeit.default_timer()
	print("Tested.... %d" % n_tested)
	print("Failed.... %d" % (n_tested-n_passed))
	print("Passed.... %d (%.1f%%)" % (n_passed,n_passed/n_tested*100))
	print("Time...... %.1f sec" % (stop-start))
