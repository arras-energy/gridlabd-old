# Syntax: create_childs [-i|--input=INPUTFILE] [-o|--output=GLMFILE] [OPTIONS ...]
"""Syntax: create_childs [-i|--input=INPUTFILE] [-o|--output=GLMFILE] [OPTIONS ...]

Options
-------
    -P|--parents=NAME:VALUE,... specify parent property pattern to match
    -C|--childs=NAME:VALUE,...  specify child property list to assign

Description
-----------

The `create_childs` tool adds child objects to all objects that match the
parent object pattern specified.

Parent patterns and child properties as specified as a comma-separate list of
`NAME:VALUE` strings, e.g., `class:node` or `nominal_voltage:2.4kV`. Parent
patterns use `regex` pattern matching. Child properties may include `{NAME}` 
format strings where `NAME` is a property of the parent object. This
allows copying of values from the parent object.

Example
-------

The following creates a GLM file containing a `triplex_load` objects attached
to `triplex_node` objects with names starting as `N_` in the file `my-network.json`:

~~~
$ gridlabd create_childs -i=my-network.json -o=loads.glm -P='class:triplex_node,name:^N_' -C='class:triplex_load,nominal_voltage:{nominal_voltage},phases:{phases},constant_power_B:1.2+0.1jkVA'
~~~
"""

import sys, os
import json
import re
import datetime

EXENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]

DEBUG = False
WARNING = True
QUIET = False
VERBOSE = False

E_OK = 0
E_INVALID = 1
E_FAILED = 2
E_SYNTAX = 8
E_EXCEPTION = 9
EXITCODE = E_OK

class GldException(Exception):
    pass

def error(msg,code=None):
    if type(code) is int:
        global EXITCODE
        EXITCODE = code
    raise GldException(msg)

def main():

    PATTERN = {}
    for name,pattern in PARENTS.items():
        PATTERN[name] = re.compile(pattern)
    OBJECTS = {}
    if "class" not in CHILDS:
        error("you must include a class name in the child properties",E_INVALID)
    classname = CHILDS["class"]

    with open(INPUTFILE,"r") as fh:
        model = json.load(fh)
    assert(model['application']=='gridlabd')

    for obj,data in model['objects'].items():
        data['name'] = obj
        ok = True
        for name,pattern in PATTERN.items():
            if not pattern.match(data[name]):
                ok = False
                break
        if ok:
            name = f"{classname}_{obj}"
            OBJECTS[name] = dict(parent=obj,name=name)
            for prop,value in CHILDS.items():
                if not prop in ["class"]:
                    OBJECTS[name][prop] = value.format(**data)

    with open(OUTPUTFILE,"w") as fh:
        print(f"// generated by '{' '.join(sys.argv)}' at {datetime.datetime.now()}",file=fh)
        for obj,data in OBJECTS.items():
            print(f"object {classname} {{",file=fh)
            for prop,value in data.items():
                print(f"    {prop} \"{value}\";",file=fh)
            print("}",file=fh)

    return E_OK

INPUTFILE = "/dev/stdin"
OUTPUTFILE = "/dev/stdout"
PARENTS = None
CHILDS = None

if __name__ == "__main__":

    if len(sys.argv) == 1:
        print(__doc__.split('\n')[0],file=sys.stderr)
        exit(E_SYNTAX)

    for arg in sys.argv[1:]:
        spec = arg.split("=")
        if len(spec) == 1:
            tag = arg
            value = None
        else:
            tag = spec[0]
            value = '='.join(spec[1:])

        if tag in ["-h","--help","help"]:
            print(__doc__)
            exit(E_OK)
        if tag in ["-i","--input"]:
            INPUTFILE = value if value else "/dev/stdin"
        elif tag in ["-o","--output"]:
            OUTPUTFILE = value if value else "/dev/stdout"
        elif tag in ["-P","--parent"]:
            PARENTS = dict([x.split(":") for x in value.split(",")])
        elif tag in ["-C","--childs"]:
            CHILDS = dict([x.split(":") for x in value.split(",")])
        else:
            error(f"option '{arg}' is invalid",E_INVALID)

    if PARENTS is None:
        error("you must specify the parent patterns to match")
    if CHILDS is None:
        error("you must specify the child properties to define")

    EXITCODE = main()
    exit(EXITCODE)
