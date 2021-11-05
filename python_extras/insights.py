
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

def get_insights(year=None,month=None):

	if not year:
		year = today.year
	if not month:
		month = today.month
	prefix = f"{path}/{year}-{month}"

	s3 = boto3.client('s3')
	objects = s3.list_objects(Bucket=bucket,Prefix=prefix)
	dt = []
	ip = []
	for obj in objects.get("Contents"):
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
	for arg in sys.argv[1:]:
		args = arg.split("=")
		if type(args) is list and len(args) > 1:
			tag = args[0]
			value = args[1]
		else:
			tag = arg
			value = None
		if arg in ["-y","--year"]:
			year = value
		elif arg in ["-m","--month"]:
			month = value
		elif arg in ["-d","--debug"]:
			debug = True
		elif arg in ["-o","--output"]:
			output = value
		else:
			error(f"{arg} is invalid",1)

	result = get_insights(year,month)
	result.to_csv("/dev/stdout")
