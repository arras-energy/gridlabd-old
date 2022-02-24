# If user enters a path to file, use file as market data
# Else, create market data file using dates
"""
Shell:
    bash$ python market_model [-p|--path=FILEPATH] [-t|--transfer=TRANSFERFUNC] [r|--periods=PERIODS]

TRANSFERFUNC = transfer function = 'p_on_q' or 'q_on_p'
"""
import sys
import os
import pandas as pd
import numpy as np


def read_file(path):
    pq_df = pd.read_csv(path)


def create_matrix(pq_df, m, n):
    pass


def syntax(code=0):
    """Display docs (code=0) or syntax help (code!=0) and exit."""
    if code == 0:
        print(__doc__)
    else:
        # TODO: Fix this syntax
        print(
            f"Syntax: {os.path.basename(sys.argv[0]).replace('.py', '')} [-p|--path=FILEPATH] ["
            f"-t|--transfer=TRANSFERFUNC] [r|--periods=PERIODS] [-h|--help|help] "
        )
    exit(code)


def error(msg, code=None):
    """Display and error message and exit if code is a number."""
    if code != None:
        print(f"ERROR [market_data.py]: {msg}", file=sys.stderr)
        exit(code)
    else:
        raise Exception(msg)


if __name__ == "__main__":
    path = None
    transfer = None
    periods = None

    if len(sys.argv) == 1:
        syntax(1)
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
        if token in ['-p', '--path']:
            path = value
        elif token in ['-t', '--transfer']:
            transfer = value
        elif token in ['-r', '--period', '--periods']:
            periods = value
        else:
            error(f"option '{token}' is not valid")

    if None not in (path, transfer, periods):
        pq_df = read_file(path)

    else:
        if path is None:
            error("User must specify a path to the file with market data. If the file does not exist, generate one by "
                  "calling: \n bash$ python market_data.py [-m|--market=MARKETNAME] [-d|--node=NODE] ["
                  "-s|--startdt=STARTDATETIME] [-e|--enddt=ENDDATETIME]", code=1)
        else:
            error("path (-p), transfer (-t), and periods (-r) are required parameters")
            syntax(1)

