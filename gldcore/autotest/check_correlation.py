import sys, pandas, numpy;

if len(sys.argv) > 0:
	data = pandas.read_csv(sys.argv[1],header=None);
	cov = numpy.cov(m=data[1],y=data[2])
	s1 = eval(sys.argv[2]);
	s12 = eval(sys.argv[3]);
	s2 = eval(sys.argv[4]);
	err = eval(sys.argv[5]);
	maxerr = numpy.max(numpy.abs([[s1,s12],[s12,s2]]-cov))
	if maxerr > err :
		print(sys.argv[0],": covariance error (%g) is too large, cov = %s"%(maxerr,cov.flatten()))
		quit(1);
else:
    raise Exception("Syntax: python3 check_correlation.py <s1> <s2> <maximum-error>")
