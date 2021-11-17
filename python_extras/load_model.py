"""Create a load model from load data

SYNTAX

  $ gridlabd load_model -i|--input=INPUTCSV -o|--output=OUTPUTCSV [-k|--order] 
        [-c|--config=CONFIGNAME] [-g|--glm=GLMNAME] [-n|--name=LOADNAME] [-t|--type=LOADTYPE]
        [-v|--verbose] [-w|--warning] [-q|--quiet] [-d|--debug]
        [-h|--help|help] [--test] 
        [OPTIONS ...]

DESCRIPTION

The `load_model` utility creates a load model from N input and M output
channels of data by fitting a Kth-order transfer functions to the data using
a least-squares linear regressions of the equations

    Y0 = -a1*Y1 - ... -aK*YK + b0*U0 + ... + bK*UK + ... + c0*V0 + ... cK*VK
    ...
    Z0 = -d1*Z1 - ... -dK*ZK + e0*U0 + ... + eK*UK + ... + f0*V0 + ... fK*VK

where

    Y0 is the output channel 0 at time t
    Y1 is the output channel 0 at time t-1
    YK is the output channel 0 at time t-K
    ...
    Z0 is the output channel N at time t
    Z1 is the output channel N at time t-1
    ...
    ZK is the output channel N at time t-K
    U0 is the input channel 0 at time t
    ...
    UK is the input channel 0 at time t-K
    V0 is the input channel M at time t
    ...
    VK is the input channel M at time t-K
    a, b, c, d, e, and f are the coefficient of the transfer functions

CONFIGURATION

By default all the columns of the input and output files are used in the load
model identification.  Consequently there will be M x N transfer functions
identified.  To prevent columns from being used you must use the
`-c|--config=CONFIGNAME` option to specify how columns are loaded. The file
CONFIGNAME must be a valid Python file. The class `converters` must be
defined, and contain the properties `inputs` and `outputs`.  These must be 
dictionaries where the keys are the column names, and values of the converters
to be used for those columns.  

For the example below, the following converter files can be used:

    from datetime import datetime
    class converters:

        def real(x):
            try:
                return float(x)
            except:
                return float('NAN')
        
        def timestamp(x):
            return datetime.strptime(x,"%m/%d/%y %H:%M")
        
        def datetime(x):
            return datetime.strptime(x,"%Y-%m-%d %H:%M:%S")

        outputs = {"timestamp":timestamp,"real_power":real}
        inputs = {"datetime":datetime,"temperature":real,"solar":real}

Note that using a converter often speeds up the load process because the
date/time format is specified and the loader does not need to deduce the
format automatically, which can take additional time.

To limit which columns of the input or output files are used, it is sufficient
to omit them from the converter list.  For example, 

    inputs = {"datetime":datetime,"temperature":real}

omits the solar data in the input file.

EXAMPLE

    $ gridlabd load_model -o=power.csv -i=weather.csv -g=/tmp/test.glm

"""

import os, sys, warnings
import numpy as np
import pandas as pd
from datetime import datetime
import random

def identify(Y, X, K = 24,
    daytype = False, hour = False, month = False, weekday = False,
    piecewise = {}
    ):
    """Fit a discrete-time MISO transfer function to data
    
    Fits a discrete-time transfer function of the form
    
        Y(z)   b0 + b1*z^-1 + ... + bK z^-K
        ---- = -----------------------------
        U(z)    1 + a1*z^-1 + ... + bK z^-K
    
    to known values of X and Y.
    
    If the hold-out size N is positive, then the model is fit
    to L-N data values, where L is the length of X.  The hold-out
    test vector Z is then computed on the N last rows of X and Y.
    
    ARGUMENTS
    
        Y (matrix) is the output (LxK)
        
        X (matrix) are the inputs (LxKxM) where M is the number of input
        variables
        
        K (int) is the desired order of the transfer function

        daytype (boolean) enables day-type feature. A single column is added
        with a boolean value indicating whether the datetime corresponds to
        a weekend or holiday.

        hour (boolean) enables the hour-of-day feature.  24 columns are added
        with boolean values indicating whether the datetime corresponds to
        each hour of day, i.e., column 0 is hour 0, column 1 is hour 1, etc.

        month (boolean) enable a month-of-year feature. 12 columns are added
        with boolean values indicating whether the datetime correspnonds to
        each month of year, i.e., column 0 is January, column 1 is February,
        etc.

        weekday (boolean) enables weekday/weekend feature.  48 columns are
        added with booleans values indicating whether the datetime
        corresponds to a weekday or weekend and hour-of-day.  This overrides
        the daytype and hour flags, with a warning. Columns 0-23 are weekday
        hours, and columns 24-47 are weekend hours.

        piecewise (dict) indicates how to apply piecewise fits to columns of
        U. The specification may be any of the following:

            { n : None }         no piecewise fit is performed (default)

            { n : m }            m inflections points are automatically found
                                 such that the rmse is minimized

            { n : [x1 x2 ...] }  multiple inflection points are given
    
    FORMATTING

        The file `load_model_config.py` may be used to modify the default
        format configuration stored in `config`.

    RETURNS
    
        x,M where x = [a1,...,aK,b0,...bN] and N = columns of M
                
    where

        M is the historical data assembled from X and Y
        
        x is the model
        
        Z is the hold out data (if any)
    """
    L = len(Y)
    M = np.hstack([-np.hstack([Y[n:L-K+n] for n in range(K)]),
                   +np.hstack([X[n+1:L-K+n+1] for n in range(K)])])
    Mt = M.transpose()
    x = np.linalg.solve(Mt*M,Mt*Y[K:])
    return x,M

if __name__ == '__main__':

    basename = os.path.basename(sys.argv[0]).replace('.py','')
    
    K = 24
    load_name = None
    modeltype = "filter"
    phases = "ABC"
    nominal_voltage = 120.0
    verbose_enabled = False
    warning_enabled = True
    debug_enabled = False
    quiet_enabled = False

    # exit codes
    E_OK = 0 # no error
    E_INPUT = 1 # input file error
    E_OUTPUT = 2 # output file error
    E_INVALID = 3 # invalid arguments
    E_CONFIG = 4 # invalid configuration
    E_OPTION = 5 # invalid options

    def error(msg,code=None):
        if not quiet_enabled:
            print(f"ERROR [{basename}]: {msg}",file=sys.stderr)
        if not debug_enabled and code != None:
            exit(code)
        elif debug_enabled:
            raise

    def syntax(code=0):
        if code == 0:
            print(__doc__,file=sys.stderr)
        else:
            print(f"Syntax: {basename} -o|--output=OUTPUTCSV -i|--input=INPUTCSV [-g|--glm=GLMNAME] [-n|--name=LOADNAME] [-t|--type=LOADTYPE] [--test] [-h|--help|help] [OPTIONS ...]")
        exit(code)

    def verbose(msg):
        if verbose_enabled:
            print(f"VERBOSE [{basename}]: {msg}",file=sys.stderr)

    def warning(msg):
        if warnings_enabled:
            print(f"WARNINGS [{basename}]: {msg}",file=sys.stderr)

    inputs = None
    outputs = None
    glmname = "/dev/stdout"
    config = None

    class converters:
        outputs = None
        inputs = None

    for arg in sys.argv[1:]:
        args = arg.split("=")
        if type(args) is list and len(args) > 1:
            token = args[0]
            value = args[1]
        elif type(args) is list:
            token = args[0]
            value = None
        else:
            token = args
            value = None
        if token in ["-h","--help","help"]:
            syntax()
        elif token in ["-c","--config"]:
            config = value
        elif token in ["-o","--output"]:
            outputs = value
        elif token in ["-i","--input"]:
            inputs = value
        elif token in ["-g","--glm"]:
            glmname = value
        elif token in ["-n","--name"]:
            load_name = value
        elif token in ["-k","--order"]:
            K = int(value)
        elif token in ["-t","--type"]:
            modeltype = value
        elif token in ["-w","--warning"]:
            warning_enabled = not warning_enabled
        elif token in ["-v","--verbose"]:
            verbose_enabled = not verbose_enabled
        elif token in ["-q","--quiet"]:
            quiet_enabled = not quiet_enabled
        elif token in ["-d","--debug"]:
            debug_enabled = not debug_enabled
        elif token in ["--test"]:
            inputs = "weather.csv"
            outputs = "power.csv"
        else:
            error(f"option '{token}' is not valid",E_OPTION)

    # load user-specified config
    try:
        import importlib
        sys.path.insert(0,os.path.dirname(config))
        config_items = importlib.import_module(os.path.basename(config).replace('.py',''))
        for item in dir(config_items):
            if not item.startswith("__"):
                value = getattr(config_items,item)
                setattr(sys.modules[__name__],item,value)
                verbose(f"{config}: configuration item {item} = {value} ok")
    except Exception as msg:
        if config:
            error(f"unable to import config file '{config}' ({msg})",E_CONFIG)

    # processing requirements met
    if inputs and outputs:
        
        # set output
        if glmname != "/dev/stdout":
            if not glmname.endswith(".glm"):
                error(f"output file '{glmname}' is not a supported file format",E_INVALID)
            try:
                sys.stdout = open(glmname,"w")
            except Exception as err:
                error(err,E_OUTPUT)

        # load data
        try:

            output_data = pd.read_csv(outputs, usecols=converters.outputs.keys(), converters=converters.outputs)
            output_names = output_data.columns[1:]
            ndx = list(map(lambda t:datetime(t.year,t.month,t.day,t.hour),pd.DatetimeIndex(output_data[output_data.columns[0]])))
            data = pd.DataFrame(output_data.groupby(ndx)[output_data.columns[1]].mean())
            verbose(data)

            input_data = pd.read_csv(inputs, usecols=converters.inputs.keys(), converters=converters.inputs)
            input_names = input_data.columns[1:]
            ndx = list(map(lambda t:datetime(t.year,t.month,t.day,t.hour),pd.DatetimeIndex(input_data[input_data.columns[0]])))
            data = input_data.groupby(ndx).mean().join(data).dropna()
            verbose(data)

        except Exception as err:

            error(err,E_INPUT)

        # begin outputing model
        print(f"// generated from 'gridlabd {basename} {' '.join(sys.argv[1:])}'")
        for output_name in output_names:

            # perform model fit
            try:

                Y = np.matrix(data[output_name]).transpose()
                U = []
                for input_name in input_names:
                    U.append(np.matrix(data[input_name]).transpose())
                x,M = identify(Y,np.hstack(U),K)

            except Exception as err:

                error(err,E_INVALID)

            if modeltype == "filter":

                print(f"// x={x.transpose().round(2).tolist()[0]}")

                for n in range(len(data.columns)-1):
                    print(f"filter {name}_{data.columns[n]}(z,1h) = (",end="")
                    for k in range(0,K-1):
                        print(f"{x[k+(n+1)*K-1,0]:+f}z^{K-k-1:.0f}",end="")            
                    print(f"{x[K-1,0]:+f} ) / (z^{K-1:.0f}",end="")         
                    for k in range(1,K-1):
                        print(f"{x[k-1,0]:+f}z^{K-k-1:.0f}",end="")
                    print(f"{x[K-2,0]:+f})",end=";\n")

                input_object = f"input_{random.randint(1e15,1e16):x}"
                print("module tape;")
                print("class","input","{")
                for n in range(len(data.columns)):
                    print("   ","double",data.columns[n],end=";\n")
                print("}")
                print("object input {")
                print("   ","name",input_object,end=";\n")
                print("   ","object","player","{")
                print("   ","   ","file",f"\"{inputs}\"",end=";\n")
                print("   ","}",end=";\n")
                print("}")

                print("module","powerflow",end=";\n")
                if not load_name:
                    load_name = f"load_{random.randint(1e15,1e16):x}"
                print("object load {")
                print("   ","name",f"{load_name}",end=";\n")
                print("   ","phases",phases,end=";\n")
                print("   ","nominal_voltage",nominal_voltage,"V",end=";\n")
                for input_name in input_names:
                    print("   ","object","load","{")
                    print("   ","   ","name",f"{load_name}_{input_name}",end=";\n")
                    if "D" in phases:
                        print("   ","   ","constant_power_AB_real",f"{name}_{input_name}({input_object}:{input_name})",end=";\n")
                        print("   ","   ","constant_power_BC_real",f"{name}_{input_name}({input_object}:{input_name})",end=";\n")
                        print("   ","   ","constant_power_CA_real",f"{name}_{input_name}({input_object}:{input_name})",end=";\n")
                    else:
                        print("   ","   ","constant_power_A_real",f"{name}_{input_name}({input_object}:{input_name})",end=";\n")
                        print("   ","   ","constant_power_B_real",f"{name}_{input_name}({input_object}:{input_name})",end=";\n")
                        print("   ","   ","constant_power_C_real",f"{name}_{input_name}({input_object}:{input_name})",end=";\n")
                    print("   ","}",end=";\n")
                print("}")
            else:
                error(f"modeltype '{modeltype}' is not valid",E_INVALID)
    else:
        syntax(1)    
