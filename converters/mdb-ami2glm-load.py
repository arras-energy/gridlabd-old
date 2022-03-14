import sys, os
from datetime import datetime, timedelta
import pandas
from meza import io as mdb

meter = "1252657E"
player = f"/Users/dchassin/Downloads/{meter}.csv"
mdbfile = "/Users/dchassin/Downloads/AMI_KWH.mdb"
csvfile = mdbfile.replace(".mdb",".csv")

reload = False

def verbose(msg):
    print(f"VERBOSE: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    print(f"WARNING: {msg}",file=sys.stderr,flush=True)

def error(msg,code=None):
    print(f"ERROR: {msg}",file=sys.stderr,flush=True)
    if type(code) != type(None):
        exit(code)

def convert(input_name,
            output_name = None,
            target_list = None):

    verbose(f"reading {input_name}")
    records = mdb.read(input_name)
    meter = []
    date = []
    value = []
    verbose(f"scanning records")
    try:
        for row in records:
            structure_no = row["Structure_No"]
            if not target_list or structure_no in target_list:
                meter.append(structure_no)
                date.append(datetime.strptime(row["Usage_Date"],"%m/%d/%y %H:%M:%S") + timedelta(hours=int(row["Hour_Num"])))
                value.append(round(float(row["SUM_of_Usage"]),2))
    except Exception as err:
        verbose(f"iteration stopped ({err})")

    data = pandas.DataFrame({"meter":meter,"usage":value},index=date)
    data.sort_index(inplace=True)
    data.index.name = "# datetime"
    return data

if not reload and os.path.exists(csvfile):
    verbose("loading csv data")
    data = pandas.read_csv(csvfile)
else:
    data = convert(mdbfile,target_list=[meter])
    verbose("saving csv data")
    data.to_csv(csvfile)

meter = data[data["meter"]==meter].drop("meter",axis=1)
meter.columns = ["# datetime","measured_real_power"]
meter.set_index("# datetime",inplace=True)
meter.to_csv(player)

with open(csvfile.replace(".csv",".glm"),"w") as glm:
    print("module powerflow;",file=glm);
    print("module tape;",file=glm)
    print("object triplex_meter",file=glm)
    print("{",file=glm)
    print("  phases AS;",file=glm)
    print("  nominal_voltage 120 V;",file=glm)
    print("  object player",file=glm)
    print("  {",file=glm)
    print(f"    file \"{csvfile}\";",file=glm)
    print("    property \"measured_real_power\";",file=glm)
    print("  };",file=glm)
    print("}",file=glm)
