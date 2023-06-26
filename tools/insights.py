# Syntax: insights CATEGORY [OPTIONS ...]
"""Gather data on gridlabd usage

SYNTAX

	$ gridlabd insights CATEGORY [OPTIONS ...]

DESCRIPTION

The `insights` utility retrieves gridlabd usage data from the AWS servers.

The following options are available.

  --by={day,month,year} Group dates by day, onth or year (default is None)

	-d|--debug					  Enable debugging (raises exception instead of printing errors)

	-h|--help|help			  Display this help

	-m|--month=MONTH		  Specify the month number (1-12)

	-o|--output[=FILE]	  Output result as CSV to stdout or FILE

	-y|--year=YEAR			  Specify the year number (4 digit year)

  --signup						  Open the ipaddr signup page to get access token

  --token=TOKEN				  Save the ipaddr access token (required for location category)

	--with-branches       Include all branches (not only master)

	--with-github         Include GitHub version checks

Currently, the supported categories are

  data: return a list of individual requests by year, month, day, and ipaddr.

  city: return a list of request counts by date and city

  region: return a list of request counts by date and region

  country: return a list of request counts by date and country

  version: return a list of request counts by version

CAVEAT

Collects version usage data for HiPAS GridLAB-D by year and month.  If month
is not specified, then all months of the current year are returned. If year
is not specified, then all months of all years are returned.

SEE ALSO

* [[/Subcommand/Aws]]
* [https://ipinfo.io/]
"""
import sys, os
import pandas
import datetime
import matplotlib.pyplot as plot
import ipinfo

try:
	with open(f"{os.getenv('HOME')}/.ipinfo/access_token","r") as fh:
		token = fh.read()
except:
	token = None

today = datetime.datetime.now().date()
URL = "http://version.gridlabd.us/access.csv"
debug = False
groupby = None
with_github = False
with_branches = False


E_OK = 0
E_INVALID = 1
E_FAILED = 2

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

def get_data(year,month):
	"""Get raw version check records"""
	data = pandas.read_csv(URL,
		names=['date','ipaddr','query','status','result'],
		index_col=['status'],
		low_memory=False,
		parse_dates=['date'],
		error_bad_lines=False,
		).loc['200'].reset_index().drop('status',axis=1)
	if year or month:
		if not month:
			month = 1
		elif not year:
			year = today.year
		start = datetime.datetime.strptime(f'{year}-{month}-1+00:00','%Y-%m-%d%z')
		if month==12:
			stop = datetime.datetime.strptime(f'{year+1}-1-1+00:00','%Y-%m-%d%z')
		else:
			stop = datetime.datetime.strptime(f'{year}-{month+1}-1+00:00','%Y-%m-%d%z')
		data = data[data['date'] >= start]
		data = data[data['date'] < stop]
	data = data[data['query'].str.match(r'^GET /index.html\?check=')]
	try:
		data['version'] = data['query'].str.replace(r'^GET /index.html\?check=','')
		data['version'] = data['version'].str.split(n=1,expand=True)
		data = data[data['version']!="test"]
	except:
		pass
	data.drop('query',axis=1,inplace=True)
	data.drop('result',axis=1,inplace=True)
	if not with_github:
		data = data[data['version'].str.endswith("-HEAD")==False]
	if not with_branches:
		data = data[data['version'].str.endswith("master")]
	data['day'] = pandas.DatetimeIndex(data['date']).strftime('%Y-%m-%d')
	data['month'] = pandas.DatetimeIndex(data['date']).strftime('%Y-%m')
	data['year'] = pandas.DatetimeIndex(data['date']).strftime('%Y')
	return data.drop('date',axis=1).set_index(['year','month','day','ipaddr'])

def get_city(year,month):
	"""Collect location data from version check records"""
	if not token:
		error("no ipaddr access token found, use --signup to get one",E_FAILED)
	handler = ipinfo.getHandler(token)
	data = get_data(year,month)
	result = data.groupby(data.index.names).count().reset_index()
	ipaddr = list(result['ipaddr'])
	info = handler.getBatchDetails(ipaddr)
	country = []
	region = []
	city = []
	for ip in ipaddr:
		specs = info[ip]
		country.append(specs['country'])
		region.append(specs['region'])
		city.append(specs['city'])
	result['country'] = country
	result['region'] = region
	result['city'] = city
	result.rename({"version":"count"},inplace=True,axis=1)
	return result.groupby(['country','region','city']).sum('count').reset_index()

def get_region(year,month):
	"""Collect location data from version check records"""
	if not token:
		error("no ipaddr access token found, use --signup to get one",E_FAILED)
	handler = ipinfo.getHandler(token)
	data = get_data(year,month)
	result = data.groupby(data.index.names).count().reset_index()
	ipaddr = list(result['ipaddr'])
	info = handler.getBatchDetails(ipaddr)
	country = []
	region = []
	city = []
	for ip in ipaddr:
		specs = info[ip]
		country.append(specs['country'])
		region.append(specs['region'])
		city.append(specs['city'])
	result['country'] = country
	result['region'] = region
	result['city'] = city
	result.rename({"version":"count"},inplace=True,axis=1)
	return result.groupby(['country','region']).sum('count').reset_index()

def get_country(year,month):
	"""Collect location data from version check records"""
	if not token:
		error("no ipaddr access token found, use --signup to get one",E_FAILED)
	handler = ipinfo.getHandler(token)
	data = get_data(year,month)
	result = data.groupby(data.index.names).count().reset_index()
	ipaddr = list(result['ipaddr'])
	info = handler.getBatchDetails(ipaddr)
	country = []
	region = []
	city = []
	for ip in ipaddr:
		specs = info[ip]
		country.append(specs['country'])
		region.append(specs['region'])
		city.append(specs['city'])
	result['country'] = country
	result['region'] = region
	result['city'] = city
	result.rename({"version":"count"},inplace=True,axis=1)
	return result.groupby(['country']).sum('count').reset_index()

def get_version(year,month):
	"""Collect version data from version check records"""
	data = get_data(year,month)
	data['version'] = data['version'].str.split("-",n=1,expand=True)
	data.reset_index(inplace=True)
	data.drop('year',axis=1,inplace=True)
	data.drop('month',axis=1,inplace=True)
	data.drop('ipaddr',axis=1,inplace=True)
	result = data.groupby(['version']).count()
	result.rename({'day':'count'},inplace=True,axis=1)
	return result.reset_index()

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
			if year == None:
				year = None
			else:
				year = int(value)
		elif tag in ["-m","--month"]:
			if value == None:
				month = None
			else:
				month = int(value)
		elif tag in ["-d","--debug"]:
			debug = True
		elif tag in ["-o","--output"]:
			output = value
		elif tag in ["-h","--help","help"]:
			syntax()
		elif tag in ["-o","--output"]:
			if not value:
				output = "/dev/stdout"
		elif tag in ["--by"]:
			if not value in ["day","month","year"]:
				error(f"options '--by={value}' is invalid",E_INVALID)
			groupby = value
		elif tag in ["--signup"]:
			os.system("open https://ipinfo.io/signup")
			print("save your token with --token=TOKEN option")
			exit(0)
		elif tag in ["--token"]:
			try:
				path = f"{os.getenv('HOME')}/.ipinfo"
				os.makedirs(path,exist_ok=True)
				with open(f"{path}/access_token","w") as fh:
					fh.write(value)
			except:
				e_type, e_value, e_trace = sys.exc_info()
				error(f"{e_value} ({e_type.__name__})",E_FAILED)
			exit(0)
		elif tag in ["--with-github"] and value == None:
			with_github = True
		elif tag in ["--with-branches"] and value == None:
			with_branches = True
		elif "get_"+tag in globals().keys():
			call = globals()["get_"+tag]
		else:
			error(f"{arg} is invalid",E_INVALID)

	if not call:
		error(f"insight category not specified",2)
	else:
		try:
			result = call(year,month)
			if output:
				result.to_csv(output,index=False,header=True)
			else:
				pandas.options.display.max_rows = None
				pandas.options.display.max_colwidth = None
				pandas.options.display.max_columns = None
				pandas.options.display.width = None
				if len(result) > 0:
					print(result)
		except SystemExit:
			pass
		except:
			if debug:
				raise
			e_type, e_value, e_trace = sys.exc_info()
			error(f"{e_value} ({e_type.__name__})",E_FAILED)
