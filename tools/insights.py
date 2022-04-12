"""Gather data on gridlabd usage

SYNTAX

  $ gridlabd insights CATEGORY [OPTIONS ...]

DESCRIPTION

The `insights` utility retrieves gridlabd usage data from the AWS servers.

The following options are available.

  -d|--debug           Enable debugging (raises exception instead of printing errors)

  -h|--help|help       Display this help

  -m|--month=MONTH     Specify the month number (1-12)

  -o|--output=CSVFILE  Specify the output CSV filename

  -p|--prefix=PATH     Specify the log prefix pathname

  -y|--year=YEAR       Specify the year number (4 digit year)

CAVEAT

Currently, only the category `requests` is supported. This returns the number of users
who run gridlabd online each day of the month.

SEE ALSO

* [[/Subcommand/Aws]]
"""
import sys
import boto3
import pandas
import datetime
import matplotlib.pyplot as plot

today = datetime.datetime.now().date()
bucket = "logs.gridlabd.us"
path = "version"
debug = False
output = "/dev/stdout"

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

def get_requests(year=None,month=None):

	if not year:
		year = today.year
	if not month:
		month = today.month
	prefix = f"{path}/{year}-{month}"

	s3 = boto3.client('s3')
	objects = s3.list_objects(Bucket=bucket,Prefix=prefix)
	dt = []
	ip = []
	contents = objects.get("Contents")
	if contents:
		for obj in contents:
			data = s3.get_object(Bucket=bucket, Key=obj.get("Key"))
			records = data["Body"].read().decode("utf-8").split("\n")
			for record in records:
				field = record.split()
				if len(field) > 10 and field[10] == "/version.gridlabd.us":
					dt.append(datetime.datetime.strptime(field[2].strip(" []"),"%d/%b/%Y:%H:%M:%S").date())
					ip.append(field[4])

	df = pandas.DataFrame(ip,index=dt,columns=["requests"])
	df.index.name = "date"
	return df.groupby("date").count()

if __name__ == "__main__":
	year = None
	month = None
	call = None

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
			year = value
		elif tag in ["-m","--month"]:
			month = value
		elif tag in ["-d","--debug"]:
			debug = True
		elif tag in ["-o","--output"]:
			output = value
		elif tag in ["-p","--prefix"]:
			path = value
		elif tag in ["-h","--help","help"]:
			syntax()
		elif "get_"+tag in globals().keys():
			call = globals()["get_"+tag]
		else:
			error(f"{arg} is invalid",1)

	if not call:
		error(f"insight category not specified",2)
	else:
		result = call(year,month)
		result.to_csv(output)
