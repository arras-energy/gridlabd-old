import sys

def compute_bill(gridlabd,**kwargs):
	name = gridlabd.get_object(f"{kwargs['classname']}:{kwargs['id']}")
	print(f"test_billing.py/compute_bill(gridlabd={gridlabd},**kwargs={kwargs}): calculating bill for {gridlabd.get_object(name)}...")
	return

if __name__ == '__main__':
	import gridlabd as gld
	compute_bill(gld,classname='billing',id=0)