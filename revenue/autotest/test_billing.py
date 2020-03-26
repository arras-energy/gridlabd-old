from datetime import *

def compute_bill(gridlabd,**kwargs):

	# get data
	classname = kwargs['classname']
	id = kwargs['id']
	data = kwargs['data']
	obj = gridlabd.get_object(f"{classname}:{id}")
	meter = gridlabd.get_object(obj["meter"])
	tariff = gridlabd.get_object(obj["tariff"])
	energy = float(meter["measured_real_energy"])/1000

	# get duration
	clock = datetime.strptime(gridlabd.get_global('clock'),'%Y-%m-%d %H:%M:%S %Z')
	if not "lastreading" in data.keys():
		duration = timedelta(0)
	else:
		duration = clock - data["lastreading"]
	data["lastreading"] = clock

	# compute energy usage
	if not "lastenergy" in data.keys():
		usage = 0.0
	else:
		usage = energy - data["lastenergy"]
	data["lastenergy"] = energy

	# compute charges
	if tariff["name"] in ["E-1","EM","ES","ESR","ET"]:
		charges = usage * float(tariff["energy_charge_base"])
	else:
		raise Exception("tariff '{tariff}' is not supported")

	# print bill
	print(f"Bill '{obj['name']}' for meter '{obj['meter']}' on tariff '{obj['tariff']}' at time '{clock}':")
	print(f"  Number of day.... %5.0f    days" % (duration.total_seconds()/86400))
	print(f"  Meter reading.... %7.1f  kWh" % (energy))
	print(f"  Energy usage..... %7.1f  kWh" % (usage))
	print(f"  Energy charges... %8.2f US$" % (charges))

	return

