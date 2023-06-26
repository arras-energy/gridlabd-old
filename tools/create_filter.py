# Syntax: create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]
"""[[/Utilities/Create_filter]] - Create a filter from a transfer function

SYNOPSIS

Shell:

    bash$ gridlabd create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]

GLM:

    #create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]

Options:

    `-c|--continuous`: flag to indicate numerator and denominator are in continuous time

    `-m|--minimum=MINIMUM`: specify the minimum output value (default is none)

    `-M|--maximum=MAXIMUM`: specify the maximum output value (default is none)

    `-o|--output=GLMFILE`: specify the output GLM file name (default is `/dev/stdout`)

    `-r|--resolution=RESOLUTION`: specify the output resolution (default is none)

    `-s|--skewtime=SKEW`: specify the time skew (default is 0)

    `-t|--timestep=TIMESTEP`: specify the time step (default is 1s)

DESCRIPTION

The `create_filter` tool creates filter for use as inputs to GridLAB-D object
properties.  The numerator and denominator must be specified as a comma-delimited
list of values in order of ascending order, e.g., `a_0,a_1,...,a_N` for an N-order
system.

When RESOLUTION is specified, then both MINIMUM and MAXIMUM must also be specified.


EXAMPLE

The following example creates a filter, from a second-order continuous-time
model, with 8 bits of resolution and a dynamic range of (0,1):

    bash% gridlabd create_filter test 1 1,0.2 -c -r=8 -m=0 -M=1 -c
    filter test(z,1,0,resolution=8,minimum=0.0,maximum=1.0) = (+1-0.529623z+0.286505z^2) / (+0.25+0.0651926z+0.0632483z^2);

SEE ALSO

* [[/Utilities/Fit_filter]]
"""

import sys, os
import control

E_OK = 0 # success
E_INVALID = 1 # invalid command option
E_FAILED = 2 # processing failed

DEBUG = False
QUIET = False
VERBOSE = False
WARNING = True

CONTINUOUS = False
MINIMUM = None
MAXIMUM = None
OUTPUTGLM = "/dev/stdout"
RESOLUTION = None
SKEW = None
TIMESTEP = '1'
FLOAT_FORMAT = '+g'

def main(argv={}):

    # no arguments provided
    if len(argv) > 1 and argv[1] in ["-h","--help"]:
        print(__doc__)
        exit(E_OK)
    elif len(argv) < 4:
        print("Syntax: gridlabd create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]",file=sys.stderr)
        exit(E_FAILED)

    NAME = argv[1]
    NUMERATOR = [float(x) for x in argv[2].split(',')]
    DENOMINATOR = [float(x) for x in argv[3].split(',')]

    # parse argument list
    if len(argv) > 4:
        for arg in [x.split('=') for x in argv[4:]]:
            tag = arg[0]
            if len(arg) == 1:
                value = None
            else:
                value = '='.join(arg[1:]).split(',')
                if len(value) == 1:
                    value = value[0]
            if tag in ["-v","--verbose"]:
                global VERBOSE
                VERBOSE = True
            elif tag in ["-w","--warn","--warning"]:
                global WARNING
                WARNING = False
            elif tag in ["-q","--quiet","-s","--silent"]:
                global QUIET
                QUIET = True
            elif tag in ["-d","--debug"]:
                global DEBUG
                DEBUG = True
            elif tag in ["-i","--input"]:
                global INPUTCSV
                if type(value) is list:
                    INPUTCSV = value
                else:
                    INPUTCSV = [value]
            elif tag in ["-o","--output"]:
                global OUTPUTGLM
                OUTPUTGLM = value
            elif tag in ["-c","--continuous"]:
                global CONTINUOUS
                if value != None:
                    if value.lower() in ["true","false","yes","no"]:
                        CONTINUOUS = ( value.lower() in ["true","yes"])
                    else:
                        error(f"value '{VALUE}' for continuous flag is not boolean")
                elif value == None:
                    CONTINUOUS = True
                else:
                    error(f"option '{tag}={value}' is invalid",E_INVALID)
            elif tag in ["-m","--minimum"]:
                global MINIMUM
                MINIMUM = float(value)
            elif tag in ["-M","--maximum"]:
                global MAXIMUM
                MAXIMUM = float(value)
            elif tag in ["-r","--resolution"]:
                global RESOLUTION
                RESOLUTION = int(value)
            elif tag in ["-s","--skew"]:
                global SKEW
                SKEW = value
            elif tag in ["-t","--timestep"]:
                global TIMESTEP
                TIMESTEP = value
            else:
                error(f"option '{'='.join(arg)}' is not valid",E_INVALID)

    if CONTINUOUS:

        H = control.tf(NUMERATOR,DENOMINATOR)
        G = control.c2d(H,float(TIMESTEP))
        NUMERATOR = G.num[0][0].tolist()
        DENOMINATOR = G.den[0][0].tolist()

    with open(OUTPUTGLM,"w") as glm:
        SPEC = f"z,{TIMESTEP}"

        if SKEW == None:
            SPEC += ",0"
        else:
            SPEC += f",{SKEW}"
        
        if RESOLUTION != None:
            if RESOLUTION <= 0:
                error("resolution must be a positive integer",E_INVALID)
            if None in [MINIMUM,MAXIMUM]:
                error("cannot use resolution unless minimum and maximum are also specified",E_INVALID)
            SPEC += f",resolution={RESOLUTION}"
        
        if MINIMUM != None:
            SPEC += f",minimum={MINIMUM}"
        
        if MAXIMUM != None:
            if MINIMUM != None and MINIMUM >= MAXIMUM:
                error("minimum must be stricly less than maximum",E_INVALID)
            SPEC += f",maximum={MAXIMUM}"

        if len(NUMERATOR) > len(DENOMINATOR):
            error("filter must be strictly causal (numerator order must be less than or equal to denominator order)",E_INVALID)

        NUM = ""
        for n,a in enumerate(DENOMINATOR):
            if n == 0:
                NUM += f"{a:+g}"
            elif n == 1:
                NUM += f"{a:+g}z"
            else:
                NUM += f"{a:+g}z^{n}"

        DEN = ""
        for n,b in enumerate(NUMERATOR):
            if n == 0:
                DEN += f"{b:+g}"
            elif n == 1:
                DEN += f"{b:+g}z"
            else:
                DEN += f"{b:+g}z^{n}"

        glm.write(f"""// generated by `{' '.join(argv)}`
filter {NAME}({SPEC}) = ({NUM}) / ({DEN});
""")

    exit(E_OK)

def error(msg,code=None,exception=None):
    """Error processing
    msg - error message to display when QUIET is not enabled
    code - exit code to use (None if no exit is desired)
    err - base exception is use when DEBUG is enabled
    """
    if DEBUG:
        raise Exception(msg) from exception
    elif not QUIET:
        print(f"ERROR [{EXENAME}]: {msg}",file=sys.stderr)
    if type(code) is int:
        exit(code)

def warning(msg):
    if WARNING:
        print(f"WARNING [{EXENAME}]: {msg}",file=sys.stderr)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{EXENAME}]: {msg}",file=sys.stderr)

def debug(msg):
    if DEBUG:
        print(f"DEBUG [{EXENAME}]: {msg}",file=sys.stderr)

if __name__ == "__main__":

    EXENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
    main(sys.argv)
