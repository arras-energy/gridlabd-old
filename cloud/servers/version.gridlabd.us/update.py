import pandas

data = pandas.read_csv("access.csv",header=None)
data.columns=["datetime","client","request","status","size"]
data = data[data['status']==200]
data = data[data['request'].str.startswith("GET /version/")]
data['branch'] = [x[1].split('/')[-1] for x in data['request'].str.split()]

print(data.groupy(['branch','client']).count()['status'])
