"""Gather data on gridlabd usage

SYNTAX

  $ gridlabd insights CATEGORY [OPTIONS ...]

DESCRIPTION

The `insights` utility retrieves gridlabd usage data from the AWS servers.

The following options are available.

  -c|--csv[=FILE]      Output result as CSV to stdout or FILE

  -d|--debug           Enable debugging (raises exception instead of printing errors)

  -h|--help|help       Display this help

  -m|--month=MONTH     Specify the month number (1-12)

  -o|--output=CSVFILE  Specify the output CSV filename

  -y|--year=YEAR       Specify the year number (4 digit year)

CAVEAT

Currently, only the category `requests` is supported. This returns the number of users
who run gridlabd online each day of the month.

SEE ALSO

* [[/Subcommand/Aws]]
"""
import sys
import pandas
import datetime
import matplotlib.pyplot as plot

today = datetime.datetime.now().date()
URL = "http://version.gridlabd.us/access.csv"
debug = False

def error(msg,code=None):
	print(f"ERROR [insights]: {msg}",file=sys.stderr)
	if debug:
		raise Exception(msg)
	if code != None:
		exit(code)

def syntax(code=None):
	if code == None:
		print(__doc__)
		exit(1)
	else:
		print("Syntax: gridlabd insights CATEGORY [OPTIONS ...]")
	if type(code) == int:
		exit(code)

def get_requests(year,month):

	start = datetime.datetime.strptime(f'{year}-{month}-1+00:00','%Y-%m-%d%z')
	if month==12:
		stop = datetime.datetime.strptime(f'{year+1}-1-1+00:00','%Y-%m-%d%z')
	else:
		stop = datetime.datetime.strptime(f'{year}-{month+1}-1+00:00','%Y-%m-%d%z')
	data = pandas.read_csv(URL,
		names=['date','ipaddr','query','status','result'],
		index_col=['status'],
		low_memory=False,
		parse_dates=['date'],
		error_bad_lines=False,
		).loc['200'].reset_index().drop('status',axis=1)
	data = data[data['date'] >= start]
	data = data[data['date'] < stop]
	data['day'] = pandas.DatetimeIndex(data['date']).strftime('%Y-%m-%d')
	data['month'] = pandas.DatetimeIndex(data['date']).strftime('%Y-%m')
	data['year'] = pandas.DatetimeIndex(data['date']).strftime('%Y')
	data.set_index(['month','ipaddr'],inplace=True)

	return data['result'].groupby('ipaddr').count()

if __name__ == "__main__":
	year = today.year
	month = today.month
	call = None
	output = None

	if len(sys.argv) == 1:
		syntax(1)
	for arg in sys.argv[1:]:
		args = arg.split("=")
		if type(args) is list and len(args) > 1:
			tag = args[0]
			value = args[1]
		else:
			tag = arg
			value = None
		if tag in ["-y","--year"]:
			year = int(value)
		elif tag in ["-m","--month"]:
			month = int(value)
		elif tag in ["-d","--debug"]:
			debug = True
		elif tag in ["-o","--output"]:
			output = value
		elif tag in ["-h","--help","help"]:
			syntax()
		elif tag in ["-c","--csv"]:
			if not value:
				output = "/dev/stdout"
		elif "get_"+tag in globals().keys():
			call = globals()["get_"+tag]
		else:
			error(f"{arg} is invalid",1)

	if not call:
		error(f"insight category not specified",2)
	else:
		result = call(year,month)
		if output:
			result.to_csv(output)
		else:
			print(result.sort_index().reset_index())
