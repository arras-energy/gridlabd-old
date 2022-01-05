"""Dataframe utility

SYNTAX:

    $ gridlabd dataframe [OPTIONS ...]
"""
import sys
import pandas as pd
import datetime as dt

left_name = "/dev/stdin"
left_suffix = "_left"
left_dateformat = "%Y-%m-%d %H:%M:%S %Z"
left_datecolumn = 0

right_name = None
right_suffix = "_right"
right_dateformat = "%Y-%m-%d %H:%M:%S %Z"
right_datecolumn = 0

join_how = "inner"
join_on = "timestamp"

output_name = "/dev/stdout"

plot_options = {
    "grid" : True,
}

def error(msg, code=None):
    if type(code) is int:
        print(f"ERROR [dataframe]: {msg} (exit code {code})")
        exit(code)
    elif code == None:
        print(f"ERROR [dataframe]: {msg}")
    else:
        raise Exception(msg)

def from_float(x):
    try:
        return float(x)
    except:
        return float('nan')

for args in list(map(lambda x:x.split("="),sys.argv[1:])):
    if len(args) > 1:
        token = args[0]
        value = "=".join(args[1:])
    else:
        token = args
        value = None
    if token in ["-i","--input_name"]:
        left_name = value
    elif token in ["-j","--join_name"]:
        right_name = value
    elif token in ["--left_suffix"]:
        left_suffix = value
    elif token in ["--right_suffix"]:
        right_suffix = value
    elif token in ["--left_dateformat"]:
        left_dateformat = value
    elif token in ["--right_dateformat"]:
        right_dateformat = value
    elif token in ["--join_how"]:
        join_how = value
    elif token in ["--join_on"]:
        join_on = value
    elif token in ["-o","--output_name"]:
        output_name = value
    else:
        error(f"option '{args}' is not valid",1)

try:
    result = pd.read_csv(left_name,
        index_col = left_datecolumn,
        parse_dates = [left_datecolumn],
        date_parser = (lambda t:dt.datetime.strptime(t,left_dateformat)),
        # converters = {
        #     "timestamp":lambda t:dt.datetime.strptime(t,left_dateformat),
        #     "real_power":from_float
        # },
        )

    if right_name:
        right = pd.read_csv(right_name,
            index_col = right_datecolumn,
            parse_dates = [right_datecolumn],
            date_parser = (lambda t:dt.datetime.strptime(t,right_dateformat)),
            # converters = {
            #     "timestamp":lambda t:dt.datetime.strptime(t,right_dateformat),
            #     "real_power":from_float
            # },
            )

        result = result.join(right,on=join_on,how=join_how,lsuffix=left_suffix,rsuffix=right_suffix)

except Exception as err:
    error(err,2)

if output_name.endswith(".png"):
    ax = result.plot(**plot_options)
    ax.figure.savefig(output_name)
elif output_name.endswith(".csv"):
    result.to_csv(output_name)
else:
    error("output format is not supported",1)
