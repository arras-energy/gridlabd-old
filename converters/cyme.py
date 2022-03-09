"""CYME data access library"""
import sys, os, subprocess
import json, csv
import pandas
import io, re, shutil

def mdb(*args,**kwargs):
	"""MDB tools"""
	cmd = ["mdb-"+args[0]]
	cmd.extend(args[1:])
	result = subprocess.Popen(cmd, stdout=subprocess.PIPE
		).communicate()[0];
	if "decode" in kwargs.keys():
		if not kwargs['decode'] == None:
			result = result.decode(kwargs['decode'])
	else:
		result = result.decode("utf-8")
	if not ( "strip" in kwargs.keys() and kwargs['strip'] == False ):
		result = result.strip()
	if "split" in kwargs.keys():
		result = result.split(kwargs['split'])
	return result

version = int("%02d%02d%02d" % tuple(map(lambda x:int(x),mdb("ver","-M",split=' ')[1][1:].split('.'))))
assert(version>=701)

#
# MDB access
#

def tables(mdb_name,table_name='.*',flags=re.IGNORECASE):
	"""MDB list of tables"""
	table_list = mdb("tables","-1",mdb_name, split='\n')
	result = []
	regex = re.compile(table_name,flags=flags)
	for table in table_list:
		if table != "" and regex.fullmatch(table) != None:
			result.append(table)
	return result

def export(mdb_name,table_name='.*',flags=re.IGNORECASE):
	"""MDB table data export"""
	# Get the list of table names with "mdb-tables"
	table_list = tables(mdb_name,table_name=table_name,flags=flags)
	table_data = {}
	for table in table_list:
		table_data[table] = mdb("export",mdb_name,table)
	return table_data

def dataframe(mdb_name,table_name='.*',drop_empty=True,flags=re.IGNORECASE):
	"""MDB to dataframe"""
	mdb_data = export(mdb_name,table_name,flags)
	result = {}
	for name, data in mdb_data.items(): 
		df = pandas.read_csv(io.StringIO(data),header=0)
		if not ( drop_empty and len(df) == 0 ):
			result[name] = df
	return result

def writecsv(mdb_name,table_name='.*',output_folder=None,drop_empty=False,raw_names=False,empty_folder=False):
	"""MDB write csv"""
	count = 0
	if not mdb_name[-4:] == ".mdb":
		raise Exception(f"input '{mdb_name}' must an MDB file (missing '.mdb' extension)")
	if output_folder == None:
		output_folder = mdb_name[:-4] + "/"
	if not output_folder[-1] == "/":
		raise Exception(f"output '{output_folder}'' must be a folder (missing trailing '/')")
	if empty_folder:
		shutil.rmtree(output_folder,ignore_errors=True)
		os.makedirs(output_folder,exist_ok=False)
	else:
		os.makedirs(output_folder,exist_ok=True)
	for name, data in dataframe(mdb_name,table_name,drop_empty=drop_empty).items():
		if not ( drop_empty and len(data) == 0 ):
			if not raw_names:
				if name[0:3] == 'CYM':
					name = name[3:].lower()
			name = name.replace(' ','_') + ".csv"
			data.to_csv(output_folder + name)
			count += 1
	return count

# if __name__ == '__main__':
# 	testfile = "autotest/IEEE-13-cyme.mdb"
# 	if os.path.exists(testfile):
# 		output_folder = testfile.replace(".mdb","/")
# 		n = writecsv(input_file=testfile,output_folder=output_folder,drop_empty=True,empty_folder=True)
# 		print(f"{testfile}: {n} files written to '{output_folder}'")
