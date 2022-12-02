"""Add meters to a MODEL

SYNOPSIS

    bash$ gridlabd create_meters [--with-ami] INPUTFILE [OUTPUTFILE]

DESCRIPTION

    The `create_meters` tool adds meters and optionally recorders
    to the input JSON file.  If the output JSON file is not specified
    the output is written to the input JSON file.

    Meters are added to all load and triplex_load objects that do not
    already have meters. If the load is connected using a link, the
    meter is assigned as the load's parent and link is reconnected to
    the meter.  If the load is connected using a parent object, then
    the parent object is converted to a meter if it is a node, otherwise
    the object is assigned a new meter parent and the meter is linked
    to the model.

    If the `--with-ami` is included, each new meter will be assigned
    a recorder with a sampling interval of 1 hour.
"""

import os, sys
import json

BASENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]

if len(sys.argv) == 1:
    print(f"Syntax: gridlabd {BASENAME} [OPTIONS ...] INPUT [OUTPUT]",file=sys.stderr)
    exit(1)

INPUTFILE = None
OUTPUTFILE = None
WITHAMI = False

for arg in sys.argv[1:]:
    if arg.startswith('-'):
        if arg == '--with-ami':
            WITHAMI = True
        else:
            raise Exception(f"argument {arg} is invalid")
    elif not INPUTFILE:
        INPUTFILE = arg
    elif not OUTPUTFILE:
        OUTPUTFILE = arg
    else:
        raise Exception(f"argument {arg} not expected")

with open(INPUTFILE,"rt") as fh:
    MODEL = json.load(fh)
    OBJECTS = MODEL['objects']

if WITHAMI and 'recorder' not in MODEL['modules']:
    VERSION = MODEL['version'].split('.')
    MODEL['modules']['tape'] = {
        "major": VERSION[0],
        "minor": VERSION[1]
    }
    MODEL['classes']['recorder'] = {
            "file" : {
                "type" : "char1024",
                "access" : "PUBLIC",
                "flags" : "REQUIRED",
                "description" : "file in which to record data"
            },
            "interval" : {
                "type" : "double",
                "access" : "PUBLIC",
                "unit" : "s",
                "default" : "-1 s",
                "description" : "sampling interval"
            },
            "property" : {
                "type" : "method",
                "access" : "PUBLIC",
                "flags" : "REQUIRED",
                "description" : "list of properties to sample"
            },
        }

LINKLIST = {}
LINKOBJS = {}
def get_links(name):
    global LINKLIST
    if not LINKLIST:
        for tag, obj in OBJECTS.items():
            try:
                add_links(obj['to'],obj['from'],tag)
            except KeyError:
                pass
    try:
        return LINKLIST[name]
    except KeyError:
        return None

def add_links(a,b,name):
    global LINKLIST
    if a not in LINKLIST:
        LINKLIST[a] = [b]
        add_links(b,a,name)
    elif b not in LINKLIST[a]:
        LINKLIST[a].append(b)
        add_links(b,a,name)

    global LINKOBS
    if a not in LINKOBJS:
        LINKOBJS[a] = [name]
    elif name not in LINKOBJS[a]:
        LINKOBJS[a].append(name)

def set_link(old,new):
    # print(old,'-->',new,':',LINKOBJS[old])
    for link in LINKOBJS[old]:
        for d in ['to','from']:
            if get_data(link,d) == old:
                set_data(link,d,new)
                # print(link,d,old,'-->',OBJECTS[link][d])

def get_data(name,tag=None):
    try:
        return OBJECTS[name][tag] if tag else OBJECTS[name]
    except KeyError:
        return None

def set_data(name,tag,value=None):
    if value:
        OBJECTS[name][tag] = value
    else:
        del OBJECTS[name][tag]

def add_object(name,data):
    global MODEL
    if name in OBJECTS:
        raise ValueError(f"object '{name}' exists")
    OBJECTS[name] = data
    OBJECTS[name]['id'] = len(OBJECTS.keys())
    if 'meter' in data['class'] and WITHAMI:
        add_object(name+'_ami',{
            'class' : 'recorder',
            'file' : name+'.csv',
            'interval' : '3600',
            'parent' : name,
            'property' : 'measured_real_energy_delta,measured_demand'
            })

def get_parent(name):
    try:
        return OBJECTS[name]['parent']
    except KeyError:
        return None

def fix_parent(parent,name,oclass):
    pclass = get_data(parent,'class')

    # change parent nodes to meters
    if pclass and pclass == oclass.replace('load','node'):

        # change node to meter
        nclass = pclass.replace('load','meter')
        set_data(parent,'class',nclass)

    else:

        # change parent
        parent = get_parent(parent)
        if parent:
            fix_parent(parent,name,oclass)
            set_data(name,'parent',parent)

def warning(msg):
    print(f"WARNING [{BASENAME}]: {msg}",file=sys.stderr)

def error(msg):
    print(f"ERROR [{BASENAME}]: {msg}",file=sys.stderr)

for name in list(OBJECTS):
    obj = OBJECTS[name]
    oclass = obj['class']
    parent = get_parent(name)
    if oclass in ['load','triplex_load']:
        links = get_links(name)
        if parent:

            fix_parent(parent,name,oclass)

        elif links == None:

            warning(f"{oclass} {name} is isolated")

        else:

            meter_name = name+"_meter"
            meter = add_object(meter_name,{
                'class' : oclass.replace('load','meter'),
                'phases' : obj['phases'],
                'nominal_voltage' : obj['nominal_voltage'],
                'measured_energy_delta_timestep' : '3600',
                })
            set_link(name,meter_name)
            set_data(name,'parent',meter_name)

    elif oclass == 'triplex_load':
        link = get_links(name)
        if parent and parent['class'] == 'node':
            obj['class'] == 'triplex_meter'
        else:
            pass
    else:
        pass

with open(OUTPUTFILE,"wt") as fh:
    json.dump(MODEL,fh,indent=4)
