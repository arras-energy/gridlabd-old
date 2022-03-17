"""Convert MDB table to a GLM objects

Options:

- `class=CLASS`: specify the class name to use if not found in columns
- `module=MODULE`: specify module to load if not found in columns
- `name=NAMECOL`: specify column to use as object name
- `parent=PARENTCOL`: specify column to use as object parent
- `withclass={true,false,yes,no,0,1}`: specify that class definition should be generated from columns
- `chunksize=INTEGER`: specify the chunksize for processing data (default 1000)
"""
import sys, os
from datetime import datetime, timedelta
import pandas_access as mdb

EXENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
DATATYPES = {
    "object" : "char1024",
    "int64" : "int64",
    "float64" : "double",
    "bool" : "bool",
    "datetime64" : "timestamp",
    "timedelta" : "double",
    "category" : "enumeration",
}

# options
CHUNKSIZE = 1000
CLASS = None
WITHCLASS = False

def verbose(msg):
    print(f"VERBOSE [{EXENAME}]: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    print(f"WARNING [{EXENAME}]: {msg}",file=sys.stderr,flush=True)

def error(msg,code=None):
    if msg == None:
        etype, evalue, etrace = sys.exc_info()
        ename = etype.__name__
        msg = f"{ename} {evalue}"
    else:
        print(f"ERROR [{EXENAME}]: {msg}",file=sys.stderr,flush=True)
    if type(code) != type(None):
        exit(code)

def convert(input_name,
            output_name = None,
            options = {}):
    """Convert MDB table to a GLM object list
    """

    # options
    if "class" in options.keys():
        global CLASS
        CLASS = options["class"]
    if "withclass" in options.keys():
        global WITHCLASS
        WITHCLASS = options["withclass"]
    if "chunksize" in options.keys():
        global CHUNKSIZE
        CHUNKSIZE = options["chunksize"].lower() in ["true","yes","1"]

    # load data
    if "table" not in options.keys():
        error(f"table not specified",1)
    else:
        TABLE = options["table"]
    objects = []
    try:
        DB = mdb.read_table(input_name,TABLE,chunksize=CHUNKSIZE)
    except Exception as err:
        error(f"{input_name} open failed ({err})",1)
    for chunk in DB:
        objects.append(chunk)

    # check for class
    if not CLASS and "class" not in DB.columns:
        error("class must be specified if not found in data",1)

    # save data
    if not output_name:
        output_name = TABLE.lower() + ".glm"
    try:
        GLM = open(output_name,"w")
    except Exception as err:
        error(f"{output_name} open failed ({err})",1)

    classes = []
    for obj in objects:
        if CLASS:
            classname = CLASS
        elif "class" in obj.columns:
            classname = obj["class"]
        if WITHCLASS and classname not in classes:
            classes.append(classname)            
            GLM.write(f'class {classname} {{\n')
            for item, dtype in zip(obj.columns,obj.dtypes):
                if str(dtype) not in DATATYPES.keys():
                    dtype = "object"
                else:
                    dtype = DATATYPES[str(dtype)]
                GLM.write(f'\t{dtype} {item};\n')
            GLM.write('}\n')

        for num, values in obj.to_dict('index').items():
            GLM.write(f'object {classname} {{\n')
            for key, value in values.items():
                GLM.write(f'\t{key} "{value}";\n')
            GLM.write('}\n')

convert("autotest/IEEE-123-cyme.mdb",None,{"table":"CYMLOAD","class":"test","withclass":True})
