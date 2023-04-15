# fit_filter [OPTIONS ...]
"""Create filters from data

SYNTAX

  $ gridlabd fit_filter -i|--input=INPUTCSV -o|--output=OUTPUTCSV [-k|--order] 
        [-c|--config=CONFIGNAME] [-g|--glm=GLMNAME] [-I|--init=ISODATETIME]
        [-M|--module=MODULENAME] [-C|--class=CLASSNAME] [-N|--name=OBJNAME]
        [-P|--player[=PLAYERCSV]] [-R|--recorder=RECORDERCSV]
        [--{by,with}_{hour,day,month,year}] [--with_time=unit]
        [-v|--verbose] [-w|--warning] [-q|--quiet] [-d|--debug]
        [-h|--help|help] [--test] 
        [OPTIONS ...]

DESCRIPTION

The `fit_filter` utility creates NxM filters from N input and M output
channels of data by fitting a Kth-order discrete-time transfer functions to
the data using a least-squares linear regressions of the equations

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

If a MODULENAME and CLASSNAME are specified, then an object of that class will be
created with the filter as inputs and object properties as outputs.  If the
module is not specified, the class is defined as a runtime class with all
inputs and outputs specified as doubles.  If OBJNAME is specified the object is 
given that name, otherwise a random object name is generated and appended to the 
OBJNAME global variable.

If PLAYERCSV is specified, then any created object will have the inputs 
assigned from that file.  If RECORDERCSV is specified, then any object created will
have the outputs recorded to that file.

If --with_{hour,day,month,year} is specified, then an hour, day, month, or
year column is added to the input data.  If --by_{hour,day,month,year} is
specified, then the input data is grouped by hour, day, month, or year. If
--with_time is specified, then the elapsed time in the specified units is
included in a column.


CONFIGURATION

By default all the columns of the input and output files are used in the
filter model identification.  Consequently there will be M x N transfer
functions identified.  To prevent columns from being used you must specify
the `-c|--config=CONFIGNAME` option to specify how columns are loaded. Only
columns listed in the `converters` class will be used to generate filters.
The file CONFIGNAME must be a valid Python file. The class `converters` must
be defined, and contain the properties `inputs` and `outputs`.  These must be
dictionaries where the keys are the column names, and values of the
converters to be used for those columns.  

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

Note that using a converter often speeds up the CSV read process because the
date/time format is specified and the loader does not need to deduce the
format automatically, which can take significant additional time.

To limit which columns of the input or output files are used, it is sufficient
to omit them from the converter list.  For example, 

    inputs = {"datetime":datetime,"temperature":real}

omits the solar data in the input file.

EXAMPLE

    $ gridlabd fit_filter -o=power.csv -i=weather.csv -g=/tmp/test.glm

"""

import os, sys, warnings, subprocess, json
import numpy as np
import pandas as pd
from datetime import datetime, timedelta
import random

np.set_printoptions(edgeitems=12,linewidth=1000,formatter={"float_kind":(lambda x:f"{x:7.4g}")})

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
"""
    L = len(Y)
    M = np.hstack([np.hstack([Y[n+1:L-K+n] for n in range(K)]),
                   np.hstack([X[n+1:L-K+n] for n in range(K+1)])])
    Mt = M.transpose()
    x = np.linalg.solve(Mt*M,Mt*Y[K+1:])
    if debug_enabled:
        print(np.hstack([M,Y[K+1:]]),file=sys.stderr)
        print(x.transpose(),file=sys.stderr)
    return x,M

if __name__ == '__main__':

    basename = os.path.basename(sys.argv[0]).replace('.py','')
    
    K = 24
    inputs = None
    outputs = None
    glmname = "/dev/stdout"
    modulename = None
    classname = None
    config = None
    objname = None
    playername = None
    recordername = None
    nominal_voltage = 120.0
    verbose_enabled = False
    warning_enabled = True
    debug_enabled = False
    quiet_enabled = False
    init_time = None
    time_skew = 0
    constraint = None
    resolution = None
    with_time = None
    with_hour = False
    with_day = False
    with_weekend = False
    with_month = False
    with_year = False
    by_hour = False
    by_day = False
    by_month = False
    by_year = False

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

    class converters:
        outputs = {}
        inputs = {}

    for arg in sys.argv[1:]:
        args = arg.split("=")
        if type(args) is list and len(args) > 1:
            token = args[0]
            value = "=".join(args[1:])
        elif type(args) is list:
            token = args[0]
            value = None
        else:
            token = args
            value = None
        if token in ["-h","--help","help"]:
            syntax()
        elif token in ["-B","--bits"]:
            resolution = int(value)
        elif token in ["-c","--config"]:
            config = value
        elif token in ["-C","--class"]:
            classname = value
        elif token in ["-d","--debug"]:
            debug_enabled = not debug_enabled
        elif token in ["-i","--input"]:
            inputs = value
        elif token in ["-I","--init"]:
            init_time = datetime.fromisoformat(value)
        elif token in ["-g","--glm"]:
            glmname = value
        elif token in ["-k","--order"]:
            K = int(value)
        elif token in ["-M","--module"]:
            modulename = value
        elif token in ["-N","--name"]:
            objname = value
        elif token in ["-o","--output"]:
            outputs = value
        elif token in ["-P","--player"]:
            playername = value
        elif token in ["-q","--quiet"]:
            quiet_enabled = not quiet_enabled
        elif token in ["-R","--recorder"]:
            recordername = value
        elif token in ["-t","--type"]:
            modeltype = value
        elif token in ["-v","--verbose"]:
            verbose_enabled = not verbose_enabled
        elif token in ["-w","--warning"]:
            warning_enabled = not warning_enabled
        elif token in ["--autotest"]:
            inputs = "weather.csv"
            outputs = "power.csv"
        elif token in ["--skew"]:
            time_skew = value
        elif token in ["--stdev"]:
            constraint = int(value)
        elif token in ["--with_time"]:
            with_time = value
        elif token in ["--with_hour"]:
            with_hour = True
        elif token in ["--with_day"]:
            with_day = True
        elif token in ["--with_weekend"]:
            with_weekend = True
        elif token in ["--with_month"]:
            with_month = True
        elif token in ["--with_year"]:
            with_year = True
        elif token in ["--by_hour"]:
            by_hour = True
        elif token in ["--by_weekday"]:
            by_day = True
        elif token in ["--by_month"]:
            by_month = True
        elif token in ["--by_year"]:
            by_year = True
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

        # begin outputing model
        print(f"// generated from 'gridlabd {basename} {' '.join(sys.argv[1:])}'")
        gldver = subprocess.run("gridlabd -D suppress_repeat_messages=FALSE --version=number --version=build --version=branch".split(),capture_output=True).stdout.decode('utf-8').split('\n')
        print(f"#define FIT_FILTER_VERSION_NUMBER={gldver[0]}")
        print(f"#define FIT_FILTER_VERSION_BUILD={gldver[1]}")
        print(f"#define FIT_FILTER_VERSION_BRANCH={gldver[2]}",flush=True)

        # load data
        try:

            if converters.outputs:
                output_cols = converters.outputs.keys()
            else:
                output_cols = None
            output_data = pd.read_csv(outputs, usecols=output_cols, converters=converters.outputs)
            output_names = output_data.columns[1:]
            ndx = list(map(lambda t:datetime(t.year,t.month,t.day,t.hour),pd.DatetimeIndex(output_data[output_data.columns[0]])))
            data = pd.DataFrame(output_data.groupby(ndx)[output_data.columns[1]].mean())
            verbose(data)

            if converters.inputs:
                input_cols = converters.inputs.keys()
            else:
                input_cols = None
            input_data = pd.read_csv(inputs, usecols=input_cols, converters=converters.inputs)
            col0 = input_data.columns[0]
            if with_time:
                tndx = input_data[col0]-input_data[col0][0]
                input_data["time"] = list(map(lambda x: x.total_seconds(),tndx))
                if with_time == "week":
                    input_data["time"] /= (3600*24*7)
                elif with_time == "day":
                    input_data["time"] /= (3600*24)
                elif with_time == "hour":
                    input_data["time"] /= 3600
                elif with_time != "second":
                    error(f"time unit '{with_time}' is not valid",2)
            if with_day:
                weekdays = list(map(lambda x: x.weekday(),input_data[col0]))
                for weekday in set(weekdays):
                    input_data[f"weekday{weekday}"] = list(map(lambda x:int(x==weekday),weekdays))
            if with_weekend:
                weekdays = list(map(lambda x: x.weekday(),input_data[col0]))
                input_data["weekday"] = list(map(lambda x:int(x<5),weekdays))
            if with_hour:
                hours = list(map(lambda x: x.hour,input_data[col0]))
                for hour in set(hours):
                    input_data[f"hour{hour}"] = list(map(lambda x:int(x==hour),hours))
            if with_month:
                months = list(map(lambda x: x.month,input_data[col0]))
                for month in set(months):
                    input_data[f"month{month}"] = list(map(lambda x: int(x==month),months))
            input_names = input_data.columns[1:]
            ndx = list(map(lambda t:datetime(t.year,t.month,t.day,t.hour),pd.DatetimeIndex(input_data[input_data.columns[0]])))
            data = input_data.groupby(ndx).mean().join(data).dropna()
            verbose(data)

        except Exception as err:

            print(f"#error {basename} {err}")
            error(err,E_INPUT)

        if init_time:
            back_time = init_time - timedelta(hours=K+1)
            print(f"\n// initial input and state vector from {back_time} to {init_time}")
            U0 = data[input_names][back_time:init_time-timedelta(hours=1)].to_dict('list')
            for name in input_names:
                print(f"#define U0_{name}={','.join(list(map(lambda z:str(z),U0[name])))}")
                print(f"#define T0_{name}={data.index[0]}")
                print(f"#define TN_{name}={data.index[-1]}")
            X0 = data[output_names][back_time:init_time-timedelta(hours=1)].to_dict('list')
            for name in output_names:
                print(f"#define X0_{name}={','.join(list(map(lambda z:str(z),X0[name])))}")
                print(f"#define T0_{name}={data.index[0]}")
                print(f"#define TN_{name}={data.index[-1]}")

        for output_name in output_names:

            # perform model fit
            try:

                Y = np.matrix(data[output_name]).transpose()
                U = []
                for input_name in input_names:
                    U.append(np.matrix(data[input_name]).transpose())
                x,M = identify(Y,np.hstack(U),K)

            except Exception as err:

                print(f"#error {basename} {err}")
                error(err,E_INVALID)

            if debug_enabled:
                print("")
                print(f"// input = {M}".replace("\n","\n//         "))
                print(f"// output = {Y[K+1:]}".replace("\n","\n//          "))
                print(f"\n// model = [{','.join(list(map(lambda z:str(z),x.transpose().round(2).tolist()[0])))}]")

            opts = ["0"]
            if resolution:
                opts.append(f"resolution={resolution}")
            if constraint:
                mean = data[output_name].mean()
                std = data[output_name].std()                
                opts.append(f"minimum={mean-constraint*std},maximum={mean+constraint*std}")

            n_inputs = len(data.columns)-1
            for n in range(n_inputs):
                print(f"\n// {data.columns[n]} --> {output_name}")
                print(f"filter {output_name}_{data.columns[n]}(z,1h,{','.join(opts)}) = (",end="")
                for k in range(0,K+1):
                    print(f"{x[-k*n_inputs-1,0]:+f}z^{K-k:.0f}",end="")            
                print(f"{x[K+n,0]:+f} ) / (z^{K:.0f}",end="")         
                for k in range(1,K):
                    print(f"{-x[K-k,0]:+f}z^{K-k:.0f}",end="")
                print(f"{-x[0,0]:+f})",end=";\n")

            if playername:
                print("module tape;")
                print("class","input","{")
                for name in input_names:
                    print("   ","double",name,end=";\n")
                print("}")
                input_object = f"input_{random.randint(1e15,1e16):x}"
                print("#define",f"INPUTNAME={input_object}")
                print("object input {")
                print("   ","name",input_object,end=";\n")
                print("   ","object","player","{")
                print("   ","   ","file",f"\"{playername}\"",end=";\n")
                print("   ","}",end=";\n")
                print("}")

                if classname:
                    if modulename:
                        print("module","powerflow",end=";\n")
                    print("class",classname,"{")
                    for name in output_names:
                        print("   ","double",name,end=";\n")
                    print("}")

                    if not objname:
                        objname = f"{classname}_{random.randint(1e15,1e16):x}"

                    print("#define",f"OUTPUTNAME={objname}")
                    print("object",classname,"{")
                    print("   ","name",f"{objname}",end=";\n")
                    for input_name in input_names:
                        print("   ",output_name,f"{output_name}_{input_name}({input_object}:{input_name},:U0_{input_name},:X0_{output_name})",end=";\n")
                    if recordername:
                        print("   ","object","recorder","{")
                        print("   ","    ","file",recordername,end=";\n")
                        print("   ","    ","property",",".join(output_names),end=";\n");
                        print("   ","    ","interval",-1,end=";\n")
                        print("   ","}",end=";\n")
                    print("}")

    else:
        syntax(1)    
