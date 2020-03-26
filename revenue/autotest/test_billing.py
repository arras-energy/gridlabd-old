
def compute_bill(gridlabd,**kwargs):
	clock = gridlabd.get_global('clock')
	classname = kwargs['classname']
	id = kwargs['id']
	data = kwargs['data']
	obj = gridlabd.get_object(f"{classname}:{id}")
	meter = gridlabd.get_object(obj["meter"])
	tariff = gridlabd.get_object(obj["tariff"])
	energy = float(meter["measured_real_energy"])
	if not "lastenergy" in data.keys():
		data["lastenergy"] = 0
	usage = energy - data["lastenergy"]

	if tariff["name"] in ["E-1","EM","ES","ESR","ET"]:
		charges = usage/1000 * float(tariff["energy_charge_base"])
	else:
		raise Exception("tariff '{tariff}' is not supported")

	print(f"computing bill '{obj['name']}' using meter '{obj['meter']}' on tariff '{obj['tariff']}' at time '{clock}'...")
	print(f"  lastenergy... {data['lastenergy']}")
	print(f"  usage........ {usage}")
	print(f"  energy....... {energy}")
	print(f"  charges...... {charges}")

	data["lastenergy"] += energy

	return

