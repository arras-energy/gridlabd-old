from datetime import *
from csv import *

csvfile = open("billing.csv","w")
csvwriter = writer(csvfile);
csvwriter.writerow(["datetime","meter","tariff","billing_days","energy","demand","charges"])

def compute_bill(gridlabd,**kwargs):

	verbose = gridlabd.get_global("verbose")=="TRUE"
	global csvwriter

	# get data
	classname = kwargs['classname']
	id = kwargs['id']
	data = kwargs['data']
	obj = gridlabd.get_object(f"{classname}:{id}")
	meter = gridlabd.get_object(obj["meter"])
	tariff = gridlabd.get_object(obj["tariff"])
	energy = float(meter["measured_real_energy"])/1000
	baseline = float(obj["baseline_demand"])

	# get duration
	clock = datetime.strptime(gridlabd.get_global('clock'),'%Y-%m-%d %H:%M:%S %Z')
	if not "lastreading" in data.keys():
		duration = timedelta(0)
	else:
		duration = clock - data["lastreading"]
	data["lastreading"] = clock
	billing_days = (duration.total_seconds()/86400)

	# compute energy usage
	if not "lastenergy" in data.keys():
		usage = 0.0
	else:
		usage = energy - data["lastenergy"]
	data["lastenergy"] = energy

	# calculate bill
	tariff_name = tariff["name"]
	meter_name = obj['meter']
	if verbose:
		print(f"Bill '{obj['name']}' for meter '{meter_name}' on tariff '{tariff_name}' at time '{clock}':")
		print(f"  Billing days..... %5.0f    days" % (billing_days))
		print(f"  Meter reading.... %7.1f  kWh" % (energy))
	if baseline == 0.0:
		if verbose:
			print(f"  Energy usage..... %7.1f  kWh" % (usage))
		charges = usage * float(tariff["energy_charge_base"])
	else:
		tier1 = min(usage,baseline*billing_days)
		tier2 = min(usage-tier1,baseline*billing_days*4)
		tier3 = usage-tier1-tier2
		if verbose:
			print(f"  Tier 1 usage..... %7.1f  kWh" % (tier1))
			if tier2 > 0:
				print(f"  Tier 2 usage..... %7.1f  kWh" % (tier2))
			if tier3 > 0:
				print(f"  Tier 3 usage..... %7.1f  kWh" % (tier3))
		charges = tier1 * float(tariff["energy_charge_base"]) + tier2 *	float(tariff["energy_charge_100"]) + tier3 * float(tariff["energy_charge_400"])	

	if verbose:
		print(f"  Energy charges... %8.2f US$" % (charges))

	# output billing record only if charges are non-zero
	if charges > 0:
		csvwriter.writerow([clock,meter_name,tariff_name,int(billing_days),round(usage,1),0,round(charges,2)])

	return

