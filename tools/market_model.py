# Syntax: market_model [OPTIONS ...]
"""Create market model from ISO data

SYNTAX

  $ python market_model.py [-f|--file=DATACSV] [-i|--input=INPUTCOL(S)] [-o|--output=OUTPUTCOL] [
  -l|--lookahead=LOOKAHEAD] [-k|--order] [-g|--glm=GLMNAME] [-I|--init=ISODATETIME] [-M|--module=MODULENAME] [
  -C|--class=CLASSNAME] [-N|--name=OBJNAME] [-P|--player[=PLAYERCSV]] [-R|--recorder=RECORDERCSV] [--{by,with}_{hour,
  day,month,year}] [-v|--verbose] [-w|--warning] [-q|--quiet] [-d|--debug] [-h|--help|help]

DESCRIPTION

The `market_model` utility creates NxM filters from N input and M output
channels of data by fitting a Kth-order discrete-time transfer functions to
the data using a least-squares linear regressions of the equation:

    Y0 = -a1*Y1 - a2*Y2 - ... - aK*Yk + b0*U0 + b1*U1 + ... + bK*Uk

where

    Y0 is the output channel 0 at time t
    Y1 is the output channel 0 at time t-1
    ...
    YK is the output channel 0 at time t-K

    U0 is the input channel 0 at time t
    U1 is the output channel 0 at time t-1
    ...
    UK is the input channel 0 at time t-K

    a and b are the coefficients of the transfer functions

If a GLMNAME and CLASSNAME are specified, then an object of that class will be
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
filter model identification.  Only columns listed in the `converters` class will be used to generate filters.
The class `converters` must be defined, and contain the properties `cols`.  These must be
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

    def datetime(x):
        return datetime.strptime(x, "%Y-%m-%d %H:%M:%S")

    cols = {"START_TIME_LOCAL": datetime, 'MW': real, 'LMP': real}

Note that using a converter often speeds up the CSV read process because the
date/time format is specified and the loader does not need to deduce the
format automatically, which can take significant additional time.

To limit which columns of the input or output files are used, it is sufficient
to omit them from the converter list.  For example, 

    cols = {"START_TIME_LOCAL":datetime, "MW":real}

omits the LMP data in the input file.

EXAMPLE

    $ python market_model -f=caiso_20220101-20220201.csv -i=LMP -o=MW -k=24 -l=3

"""

import os, sys
import numpy as np
import pandas as pd
from datetime import datetime, timedelta
import random

np.set_printoptions(
    edgeitems=12, linewidth=1000, formatter={"float_kind": (lambda x: f"{x:7.4g}")}
)


def look_ahead(M, Y, K, l):
    """Modify the Y and M matrices to forecast for 'l' days ahead."""
    Y = Y[:-K]
    for n in range(l - 1):
        Y = np.insert(Y, 0, M[:, n].T, axis=1)
    M = M[:, l - 1 :]
    return Y, M


def identify(Y, U, K=24, l=0):
    """
    Fit a discrete time transfer function of the form
    Y(z) = -a1*Y(z^-1) - a2*Y(z^-2) - ... - aK*Y(z^-K) + b0*U(z) + b1*U(z-1) + ... + bK*U(z^-k)
    to known value of U and Y.

    ARGUMENTS:

        Y (matrix) is the output (Lx1)
        U (matrix) are the inputs (LxKxM) where M is the number of input variables
        K (int) is the desired order of the transfer function
    """
    length = len(Y)
    if length < K:
        warning("There is insufficient data for the specified K value.")

    M = np.hstack(
        [
            np.hstack([Y[n : length - K + n] for n in range(1, K + 1)]),
            np.hstack([U[n : length - K + n] for n in range(K + 1)]),
        ]
    )

    Y, M = look_ahead(M, Y, K, l)
    x = np.linalg.lstsq(M, Y, rcond=None)[0]

    if debug_enabled:
        print(np.hstack([M, Y[K + 1 :]]), file=sys.stderr)
        print(x.transpose(), file=sys.stderr)

    return x, Y, M


def plot_results(Y, U, K):
    """Plot the predicted vs actual results when model is trained on first 75% of data, and tested on last 25%. Use a
    lookahead value of 3."""
    l = 1

    # Split into train and test
    train_size = int(len(Y) * 0.75)
    Y_train = Y[:train_size]
    Y_test = Y[train_size:]
    U_train = np.hstack(U)[:train_size, :]
    U_test = np.hstack(U)[train_size:, :]

    # Calculate x vector and M for train set
    x_train, Y_train, M_train = identify(Y_train, U_train, K, l)

    # Calculate x vector and M for test set
    x_test, Y_test, M_test = identify(Y_test, U_test, K, l)

    # Use the train x vector on the test M to calculate predicted y
    Y_pred = np.matmul(M_test, x_train)
    Y_pred_df = pd.DataFrame(
        Y_pred, columns=[f"{n}hr_ahead_pred" for n in range(1, l + 1)]
    )
    Y_test_df = pd.DataFrame(
        Y_test, columns=[f"{n}hr_ahead_actual" for n in range(1, l + 1)]
    )

    # Plot
    colors = [
        "tab:blue",
        "tab:orange",
        "tab:green",
        "tab:blue",
        "tab:orange",
        "tab:green",
    ]
    styles = ["-", "-", "-", ":", ":", ":"]
    Y_test_df.join(Y_pred_df).plot(color=colors, style=styles)


if __name__ == "__main__":

    basename = os.path.basename(sys.argv[0]).replace(".py", "")

    K = 24
    inputs = None
    outputs = None
    glmname = "/dev/stdout"
    modulename = None
    classname = None
    objname = None
    playername = None
    recordername = None
    lookahead = 1
    verbose_enabled = False
    warning_enabled = True
    debug_enabled = False
    quiet_enabled = False
    init_time = None
    constraint = None
    resolution = None
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
    E_OK = 0  # no error
    E_INPUT = 1  # input file error
    E_OUTPUT = 2  # output file error
    E_INVALID = 3  # invalid arguments
    E_CONFIG = 4  # invalid configuration
    E_OPTION = 5  # invalid options

    def error(msg, code=None):
        if not quiet_enabled:
            print(f"ERROR [{basename}]: {msg}", file=sys.stderr)
        if not debug_enabled and code != None:
            exit(code)
        elif debug_enabled:
            raise

    def syntax(code=0):
        if code == 0:
            print(__doc__, file=sys.stderr)
        else:
            print(
                f"Syntax: {basename} [-f | --file = DATACSV] [-i | --input = INPUTCOL(S)] [-o | --output = OUTPUTCOL] "
                f"[-l | --lookahead = LOOKAHEAD] [-k | --order] [-g | --glm = GLMNAME] [-I | --init = ISODATETIME] ["
                f"-M | --module = MODULENAME] [-C | --class =CLASSNAME][-N | --name=OBJNAME] [-P | --player["
                f"=PLAYERCSV]][-R | --recorder=RECORDERCSV] [--{{by, with}}_{{hour, day, month, year}}] [-v | "
                f"--verbose][-w | --warning][-q | --quiet][-d | --debug] [-h | --help | help]"
            )
        exit(code)

    def verbose(msg):
        if verbose_enabled:
            print(f"VERBOSE [{basename}]: {msg}", file=sys.stderr)

    def warning(msg):
        if warning_enabled:
            print(f"WARNINGS [{basename}]: {msg}", file=sys.stderr)

    class converters:
        def real(x):
            try:
                return float(x)
            except:
                return float("NAN")

        def datetime(x):
            return datetime.strptime(x, "%Y-%m-%d %H:%M:%S")

        cols = {"START_TIME_LOCAL": datetime, "MW": real, "LMP": real}

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
        if token in ["-h", "--help", "help"]:
            syntax()
        elif token in ["-B", "--bits"]:
            resolution = int(value)
        elif token in ["-C", "--class"]:
            classname = value
        elif token in ["-f", "--file"]:
            data_file = value
        elif token in ["-d", "--debug"]:
            debug_enabled = not debug_enabled
        elif token in ["-i", "--input"]:
            inputs = value.split(",")
        elif token in ["-I", "--init"]:
            init_time = datetime.fromisoformat(value)
        elif token in ["-g", "--glm"]:
            glmname = value
        elif token in ["-k", "--order"]:
            K = int(value)
        elif token in ["-l", "--lookahead"]:
            lookahead = int(value)
        elif token in ["-M", "--module"]:
            modulename = value
        elif token in ["-N", "--name"]:
            objname = value
        elif token in ["-o", "--output"]:
            outputs = value.split(",")
        elif token in ["-P", "--player"]:
            playername = value
        elif token in ["-q", "--quiet"]:
            quiet_enabled = not quiet_enabled
        elif token in ["-R", "--recorder"]:
            recordername = value
        elif token in ["-t", "--type"]:
            modeltype = value
        elif token in ["-v", "--verbose"]:
            verbose_enabled = not verbose_enabled
        elif token in ["-w", "--warning"]:
            warning_enabled = not warning_enabled
        elif token in ["--stdev"]:
            constraint = int(value)
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
            error(f"option '{token}' is not valid", E_OPTION)

    # processing requirements met
    if inputs and outputs:

        # set output
        if glmname != "/dev/stdout":
            if not glmname.endswith(".glm"):
                error(
                    f"output file '{glmname}' is not a supported file format", E_INVALID
                )
            try:
                sys.stdout = open(glmname, "w")
            except Exception as err:
                error(err, E_OUTPUT)

        # load data
        try:
            original_data = pd.read_csv(
                data_file, usecols=converters.cols.keys(), converters=converters.cols
            )
            outputs.insert(0, "START_TIME_LOCAL")
            output_data = original_data[outputs]
            output_names = output_data.columns[1:]
            ndx = list(
                map(
                    lambda t: datetime(t.year, t.month, t.day, t.hour),
                    pd.DatetimeIndex(output_data[output_data.columns[0]]),
                )
            )
            data = pd.DataFrame(output_data.groupby(ndx)[output_data.columns[1]].mean())
            verbose(data)

            inputs.insert(0, "START_TIME_LOCAL")
            input_data = original_data[inputs]
            col0 = input_data.columns[0]
            if with_day:
                weekdays = list(map(lambda x: x.weekday(), input_data[col0]))
                for weekday in set(weekdays):
                    input_data[f"weekday{weekday}"] = list(
                        map(lambda x: int(x == weekday), weekdays)
                    )
            if with_weekend:
                weekdays = list(map(lambda x: x.weekday(), input_data[col0]))
                input_data["weekday"] = list(map(lambda x: int(x < 5), weekdays))
            if with_hour:
                hours = list(map(lambda x: x.hour, input_data[col0]))
                for hour in set(hours):
                    input_data[f"hour{hour}"] = list(
                        map(lambda x: int(x == hour), hours)
                    )
            if with_month:
                months = list(map(lambda x: x.month, input_data[col0]))
                for month in set(months):
                    input_data[f"month{month}"] = list(
                        map(lambda x: int(x == month), months)
                    )
            input_names = input_data.columns[1:]
            ndx = list(
                map(
                    lambda t: datetime(t.year, t.month, t.day, t.hour),
                    pd.DatetimeIndex(input_data[input_data.columns[0]]),
                )
            )
            data = input_data.groupby(ndx).mean().join(data).dropna()
            data = data.sort_index(ascending=False)
            verbose(data)

        except Exception as err:

            print(f"#error {basename} {err}")
            error(err, E_INPUT)

        if init_time:
            back_time = init_time - timedelta(hours=K + 1)
            print(
                f"\n// initial input and state vector from {back_time} to {init_time}"
            )
            U0 = data[input_names][back_time : init_time - timedelta(hours=1)].to_dict(
                "list"
            )
            for name in input_names:
                print(
                    f"#define U0_{name}={','.join(list(map(lambda z: str(z), U0[name])))}"
                )
                print(f"#define T0_{name}={data.index[0]}")
                print(f"#define TN_{name}={data.index[-1]}")
            X0 = data[output_names][back_time : init_time - timedelta(hours=1)].to_dict(
                "list"
            )
            for name in output_names:
                print(
                    f"#define X0_{name}={','.join(list(map(lambda z: str(z), X0[name])))}"
                )
                print(f"#define T0_{name}={data.index[0]}")
                print(f"#define TN_{name}={data.index[-1]}")

        for output_name in output_names:

            # perform model fit
            try:

                Y = np.matrix(data[output_name]).transpose()
                U = []
                for input_name in input_names:
                    U.append(np.matrix(data[input_name]).transpose())
                plot_results(Y, U, K)

                x, Y, M = identify(Y, np.hstack(U), K, lookahead)
                # pd.DataFrame(x).to_csv('x.csv')
                # pd.DataFrame(Y).to_csv('Y.csv')
                # pd.DataFrame(M).to_csv('M.csv')

            except Exception as err:

                print(f"#error {basename} {err}")
                error(err, E_INVALID)

            if debug_enabled:
                print("")
                print(f"// input = {M}".replace("\n", "\n//         "))
                print(f"// output = {Y[K + 1:]}".replace("\n", "\n//          "))
                print(
                    f"\n// model = [{','.join(list(map(lambda z: str(z), x.transpose().round(2).tolist()[0])))}]"
                )

            opts = ["0"]
            if resolution:
                opts.append(f"resolution={resolution}")
            if constraint:
                mean = data[output_name].mean()
                std = data[output_name].std()
                opts.append(
                    f"minimum={mean - constraint * std},maximum={mean + constraint * std}"
                )

            n_inputs = len(data.columns) - 1
            for n in range(n_inputs):
                # TODO: Review formulas
                print(f"\n// {data.columns[n]} --> {output_name}")
                print(
                    f"filter {output_name}_{data.columns[n]}(z,1h,{','.join(opts)}) = (",
                    end="",
                )
                for k in range(0, K + 1):
                    print(f"{x[K - k * n_inputs - 1, 0]:+f}z^{K - k:.0f}", end="")
                print(f"{x[K + n, 0]:+f} ) / (z^{K:.0f}", end="")
                for k in range(1, K):
                    print(f"{-x[K - k, 0]:+f}z^{K - k:.0f}", end="")
                print(f"{-x[0, 0]:+f})", end=";\n")

            if playername:
                print("module tape;")
                print("class", "input", "{")
                for name in input_names:
                    print("   ", "double", name, end=";\n")
                print("}")
                input_object = f"input_{random.randint(1e15, 1e16):x}"
                print("#define", f"INPUTNAME={input_object}")
                print("object input {")
                print("   ", "name", input_object, end=";\n")
                print("   ", "object", "player", "{")
                print("   ", "   ", "file", f'"{playername}"', end=";\n")
                print("   ", "}", end=";\n")
                print("}")

                if classname:
                    if modulename:
                        print("module", modulename, end=";\n")
                    print("class", classname, "{")
                    for name in output_names:
                        print("   ", "double", name, end=";\n")
                    print("}")

                    if not objname:
                        objname = f"{classname}_{random.randint(1e15, 1e16):x}"

                    print("#define", f"OUTPUTNAME={objname}")
                    print("object", classname, "{")
                    print("   ", "name", f"{objname}", end=";\n")
                    for input_name in input_names:
                        print(
                            "   ",
                            output_name,
                            f"{output_name}_{input_name}({input_object}:{input_name},:U0_{input_name},:X0_{output_name})",
                            end=";\n",
                        )
                    if recordername:
                        print("   ", "object", "recorder", "{")
                        print("   ", "    ", "file", recordername, end=";\n")
                        print(
                            "   ", "    ", "property", ",".join(output_names), end=";\n"
                        )
                        print("   ", "    ", "interval", -1, end=";\n")
                        print("   ", "}", end=";\n")
                    print("}")

    else:
        syntax(1)
