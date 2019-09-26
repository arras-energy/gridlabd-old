"""GridLAB-D Python validation routine

This script runs the full gridlabd validate test suite using
the Python module build from the gldcore/link/python folder.

Syntax: python3 validate.py [options]

Options:
  --debug    enable debugging output
  --dry-run  don't actually run tests
  --help     get this help
  --no-save  don't save results in validate.txt
  --quiet    don't display failure output
  --verbose  enable verbose output

Example:

  host% python3 validate.py

See also:
- GridLAB-D --validate command line option
"""
import sys, os, subprocess
assert(sys.version_info.major>2)
import traceback
import timeit

n_tested = 0
n_passed = 0
exename = '/usr/local/bin/gridlabd'
dry_run = False
show_debug = False
show_failure = False
show_verbose = False
save_output = False

def debug(msg) :
	"""Output a debugging message (see --debug option)"""
	if show_debug :
		print("DEBUG: %s" % msg)

def verbose(msg) :
	"""Output a verbose message (see --verbose option)"""
	if show_verbose :
		print("... %s" % msg)

def validate(dir=None) :
	"""Runs validate test files found below 'dir'"""
	if not dir :
		dir = os.getcwd()
	if not os.path.isdir(dir) :
		return
	debug("validate(dir=%s)" % dir)
	items = sorted(os.listdir(dir))
	debug("validate: items=%s"%items)
	if 'validate.no' in items :
		debug('validate: skipping %s (validate.no found)' % dir)
		return;
	for item in items :
		path = os.path.join(dir,item)
		if not os.path.isdir(path) :
			continue
		elif item == "." or item ==".." :
			continue
		elif item == 'autotest' and os.path.isdir(path) :
			autotest(path)
		else :
			validate(path)

def autotest(dir) :
	"""Runs validate tests files found in 'dir'"""
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
	"""Runs the system command (see --dry-run option)"""
	global dry_run
	if dry_run :
		print("dryrun> %s" % cmd)
		return
	else :
		debug("run_command(cmd='%s')" % cmd)
		return (os.system(cmd) >> 8)

def runtest(workdir,glmname) :
	"""Runs the test 'glmname' in 'workdir' folder"""
	global n_tested, n_passed, exename
	n_tested += 1
	sys.stdout.flush()
	owd = os.getcwd()
	if dry_run:
		print("dryrun> runtest(workdir='%s',glmname='%s'" % (workdir,glmname))
		print("dryrun> (workdir=%s)"%owd)
	run_command("mkdir -p " + workdir)
	run_command("cp %s.glm %s" % (workdir,workdir))
	run_command("cd %s" % workdir)
	print("Running %s..." % glmname)
	rc = run_command("/usr/local/bin/python3 %s/gldcore/link/python/python_gridlabd.py -W %s %s 1>gridlabd.out 2>&1" % (owd,workdir,glmname))
	if not dry_run:
		if rc == 255 :
			print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
			if show_failure:
				run_command("cat gridlabd.out")
		elif not "_opt" in glmname :
			if rc == 0 :
				if "_err" in glmname or "_exc" in glmname :
					print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
					if show_failure : run_command("cat gridlabd.out")
					if save_output : run_command("(echo '*** %s test failed ***' ; cat gridlabd.out) >> %s" % (workdir,save_output))
				else :
					verbose("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
					n_passed += 1
			else :
				if "_err" in glmname or "_exc" in glmname :
					verbose("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
					n_passed += 1
				else :
					print("FAIL %s exit %d" % ("/".join([workdir,glmname]),rc))
					if show_failure : run_command("cat gridlabd.out")
					if save_output : run_command("cat gridlabd.out >> %s" % save_output)
		else :
			verbose("PASS %s exit %d" % ("/".join([workdir,glmname]),rc))
			n_passed += 1
	run_command("cd %s" % owd)

if __name__ == '__main__':
	if "--help" in sys.argv :
		import validate
		help(validate)
		quit()
	show_debug = ("--debug" in sys.argv)
	show_verbose = ("--verbose" in sys.argv)
	dry_run = ("--dry-run" in sys.argv)
	show_failure = (not "--quiet" in sys.argv)
	save_output = (not "--no-save" in sys.argv)
	debug("sys.argv=%s" % sys.argv)
	srcdir = os.path.abspath(sys.argv[0]).replace("/gldcore/link/python/validate.py","")
	debug("srcdir=%s" % srcdir)
	gridlabd = subprocess.check_output(["which","gridlabd"]).decode()
	if not gridlabd : raise Exception("unable to locate gridlabd in $PATH (which return '%s')" % gridlabd)
	install = gridlabd.replace("/bin/gridlabd\n","")
	if not os.path.isdir(install) : raise Exception("install folder %s is not found" % install)
	debug("install=%s" % install)
	binpath = install + "/bin"
	if not os.path.isdir(binpath) : raise Exception("bin folder %s is not found" % binpath)
	debug("binpath=%s" % binpath)
	libpath = install + "/lib/gridlabd"
	if not os.path.isdir(libpath) : raise Exception("lib folder %s is not found" % libpath)
	debug("libpath=%s" % libpath)
	datpath = install + "/share/gridlabd"
	if not os.path.isdir(datpath) : raise Exception("data folder %s is not found" % datpath)
	debug("datpath=%s" % datpath)
	incpath = install + "/include/gridlabd"
	if not os.path.isdir(incpath) : raise Exception("include folder %s is not found" % incpath)
	debug("incpath=%s" % incpath)
	exename = "python3 "+srcdir + "/gldcore/link/python/gridlabd_python.py"
	print(exename[0:7])
	if exename[0:7] != "python3" and not os.path.isfile(exename) : raise Exception("gridlabd program %s is not found" % exename)
	debug("exename=%s" % exename)
	owd = os.getcwd()
	if save_output : 
		save_output = owd + "/validate.txt"
		run_command("echo Validation output on $(date) > %s" % save_output)
	# TODO: this needs to be read from prefix rather than using system default
	os.environ["GLPATH"] = ":".join([binpath,libpath,datpath,incpath])
	os.environ["CXXFLAGS"] = " -I".join(["-w -O3 -g",datpath,incpath])
	debug("GLPATH=%s" % os.getenv("GLPATH"))
	debug("CXXFLAGS=%s" % os.getenv("CXXFLAGS"))
	os.chdir(srcdir)
	print("Validating %s, please wait..." % os.getcwd())
	try :
		start = timeit.default_timer()
		validate(os.getcwd())
		stop = timeit.default_timer()
	except :
		os.chdir(owd)
		raise
	os.chdir(owd)
	print("Tested.... %d" % n_tested)
	print("Failed.... %d" % (n_tested-n_passed))
	print("Passed.... %d (%.1f%%)" % (n_passed,n_passed/n_tested*100))
	print("Time...... %.1f sec" % (stop-start))
