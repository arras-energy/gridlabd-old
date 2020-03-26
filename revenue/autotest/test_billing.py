
def compute_bill(gridlabd,**kwargs):
	clock = gridlabd.get_global('clock')
	obj = gridlabd.get_object(f"{kwargs['classname']}:{kwargs['id']}")
	print(f"computing bill '{obj['name']}' using meter '{obj['meter']}' on tariff '{obj['tariff']}' at time '{clock}'...")
	return

