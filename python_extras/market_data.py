"""ISO Market Data tool

SYNOPSIS

Shell:

    bash$ gridlabd market_data [-m|--market=MARKETNAME] [-d|--node=NODE] [-s|--startdate=STARTDATE]
    [-e|--enddate=ENDDATE] [-h|--help|help] [--credentials[=FILENAME.json]]
    [-n|--name=OBJNAME] [-c|--csv=CSVFILE] [-g|--glm=GLMFILE]

GLM:

    #market_data [-m|--market=MARKETNAME] [-d|--node=NODE] [-s|--startdate=STARTDATE]
    [-e|--enddate=ENDDATE] [-h|--help|help] [--credentials[=FILENAME.json]]
    [-n|--name=OBJNAME] [-c|--csv=CSVFILE] [-g|--glm=GLMFILE]

Python:

    bash$ gridlabd python
    >>> import market_data as md
    >>> data = md.get_market_data(MARKETNAME,NODE,STARTDATE,ENDDATE[,CREDENTIALS])
    >>> md.write_csv(data,CSVNAME)
    >>> md.write_glm(data,GLMNAME,[NAME])

DESCRIPTION

The market_data tool obtain wholesale electricity market price data from
independent system operators (ISO).  The ISO must be specified using the
MARKETNAME parameter. Currently supported markets are "CAISO" and "ISONE". 

In addition, the NODE must be specified when obtaining locational price data.
Lists of node names may be obtained from the market websites.  For CAISO see
http://www.caiso.com/TodaysOutlook/Pages/prices.html for a map of available
price nodes.  For ISONE see https://www.iso-ne.com/markets-operations/settlements/pricing-node-tables/

The STARTDATE and ENDDATE value must be specified in the form "YYYYMMDD".

When the `--credentials` option is used with parameters, the user is queried
for a username and password. The output is the credentials data that must be
stored for later use when credentials are needed.  The default credentials
files is `credentials.json`.  When FILENAME is provided with the credentials
option, the credentials are loaded from the specified file instead.

EXAMPLES

    bash$ gridlabd market_data -m=CAISO -d

SEE ALSO

* [CAISO](https://caiso.com/)
* [ISONO](https://isone.com/)

- MARKETNAME can be "caiso" or "isone"

- NODE is needed for LMP data; For a list of CAISO nodes, download the
  latest 'Full Network Model Pricing Node Mapping' from the CAISO website.
  For a list of ISONE nodes, downloaded the latest 'PNODE Table' from the
  ISONE website.
    - Sample node for CAISO: 0096WD_7_N001
    - Sample node for ISO-NE: 4001

- STARTDATE & ENDDATE should be in YYYYMMDD

The ISONE API requires credentials. To generate the credentials data, run:
    bash$ python market_data.py --credentials > credential.json
"""

import datetime as dt
import time
import sys
import os
import re
import requests
from requests.auth import HTTPBasicAuth
import pytz
import getpass
import json
import pandas as pd
from io import BytesIO
from pandas.api.types import is_datetime64_any_dtype as is_datetime

from urllib.request import urlopen
from zipfile import ZipFile

verbose_enable = False
warning_enable = True
quiet_enable = False
debug_enable = False

def get_credentials():
    """
    Get the username and password from the local credentials.json file
    :return: dict - Dictionary with username and password
    """
    credentials_path = f"{os.getcwd()}/credentials.json"
    try:
        with open(credentials_path) as credentials_file:
            credentials_dict = json.load(credentials_file)
            user = credentials_dict["username"]
            pwd = credentials_dict["password"]
    except Exception as err:
        error(f"unable to load credentials file {credentials_path} ({msg})",E_INVALID)
    return {"user": user, "pwd": pwd}


def convert_timezone(market, date_str):
    """
    Convert timestamp into timezone required by API.
    Assumes that user input timestamp in local time (ex. PST for CAISO)
    :param market: str - Market of pulled data ('caiso' or 'isone')
    :param date_str: str - Date in format entered by user (YYYYMMDD)
    :return: str - Date in required timezone (YYYYMMDD-HHmm)
    """
    naive_dt = dt.datetime.strptime(date_str, "%Y%m%d")

    # CAISO API uses UTC time and requires an hour/minute stamp
    if market == "caiso":
        local_tz = pytz.timezone("US/Pacific")
        local_dt = local_tz.localize(
            naive_dt, is_dst=None
        )  # Setting to none will raise errors for ambiguous or non-existent times
        converted_dt = local_dt.astimezone(pytz.utc)

    # ISONE API uses local time and takes only date (not time)
    elif market == "isone":
        converted_dt = naive_dt

    return converted_dt


def format_date(market, date_dt):
    """
    Reformat the date string into the format required by the API.
    :param market: str - Market of pulled data
    :param date_dt: datetime - Date in format returned by convert_timezone function
    :return: str - Date in format required by the API
    """
    if market == "caiso":
        formatted_str = date_dt.strftime("%Y%m%dT%H:%M-0000")
    elif market == "isone":
        formatted_str = date_dt.strftime("%Y%m%d")
    return formatted_str


def get_dates(market,start_date_str, end_date_str, max_interval):
    """
    Returns a list of dicts, where each dict has 2 keys: 'start_date_str' and 'end_date_str'. The difference between
    the two is the max interval of dates allowed in an API query (ex. 31 days). Each dict in the list should be used
    to formulate a separate query to the API.
    :param start_date_str: str - Date in format entered by user (YYYYMMDD)
    :param end_date_str: str - Date in format entered by user (YYYYMMDD)
    :param max_interval: int - Max number of days the API allows in a single query
    :return: dates_list: list - List of dicts with start and end dates
    """
    dates_list = []
    start_date_dt = dt.datetime.strptime(start_date_str, "%Y%m%d")
    end_date_dt = dt.datetime.strptime(end_date_str, "%Y%m%d")
    start_date_temp_dt = start_date_dt

    while (end_date_dt - start_date_temp_dt).days > 0:
        end_date_temp_dt = start_date_temp_dt + dt.timedelta(days=max_interval)
        if end_date_dt < end_date_temp_dt:
            end_date_temp_dt = end_date_dt
        start_date_temp_str = format_date(
            market, convert_timezone(market, start_date_temp_dt.strftime("%Y%m%d"))
        )
        end_date_temp_str = format_date(
            market, convert_timezone(market, end_date_temp_dt.strftime("%Y%m%d"))
        )
        dates_list.append(
            {"start_date_str": start_date_temp_str, "end_date_str": end_date_temp_str}
        )

        start_date_temp_dt = end_date_temp_dt

    return dates_list


def extract_zip(url):
    """
    Get a zip file from a url and read it into a dataframe.
    :param url: str - URL to query
    :param folder: str - folder to save contents of zip file to
    :return: df - dataframe of extracted contents from url
    """
    # Download the file from the URL
    zip_resp = urlopen(url)
    zf = ZipFile(BytesIO(zip_resp.read()))
    data = pd.read_csv(zf.open(ZipFile.namelist(zf)[0]))

    # Wait
    time.sleep(3)

    return data


def check_missing_data(datetime_col, start_datetime, end_datetime, freq):
    """
    Check whether there are any missing timestamps in the pulled data.
    :param datetime_col: pd.Series - column of datetimes from pulled data
    :start_datetime: pd.datetime - starting datetime of range
    :end_datetime: pd.datetime - ending datetime of range
    :freq: str - frequency of data (5T = 5 min, H = hourly)
    """
    missing_timestamps = (pd.date_range(start_datetime, end_datetime, freq=freq).difference(datetime_col))
    if len(missing_timestamps) > 0:
        error(f'Missing timestamps {missing_timestamps} in data', 1)


def clean_market_data(market, lmp_df, demand_df):
    """
    Merge the LMP and demand dataframes into a cleaned, single dataframe
    :param market: str - Market of pulled data
    :param lmp_df: dataframe - LMP df directly from API query
    :param demand_df: dataframe - Demand df directly from API query
    :return:
    """
    if market == "caiso":
        # Only consider LMP data
        lmp_df = lmp_df[lmp_df["XML_DATA_ITEM"] == "LMP_PRC"].reset_index(drop=True)

        # Only consider CAISO-wide demand
        demand_df = demand_df[demand_df["TAC_AREA_NAME"] == "CA ISO-TAC"].reset_index(drop=True)

        # Convert timestamp to local timezone and check for missing timestamps
        lmp_df["INTERVALSTARTTIME_GMT"] = pd.to_datetime(
            lmp_df["INTERVALSTARTTIME_GMT"]
        )
        lmp_df["START_TIME_PST"] = lmp_df["INTERVALSTARTTIME_GMT"].dt.tz_convert(
            "US/Pacific"
        )
        check_missing_data(lmp_df['START_TIME_PST'], lmp_df['START_TIME_PST'].iloc[0], lmp_df['START_TIME_PST'].iloc[-1], '5T')

        demand_df["INTERVALSTARTTIME_GMT"] = pd.to_datetime(
            demand_df["INTERVALSTARTTIME_GMT"]
        )
        demand_df["START_TIME_PST"] = demand_df["INTERVALSTARTTIME_GMT"].dt.tz_convert(
            "US/Pacific"
        )
        check_missing_data(demand_df['START_TIME_PST'], demand_df['START_TIME_PST'].iloc[0], demand_df['START_TIME_PST'].iloc[-1], 'H')

        # Drop unnecessary columns
        lmp_df = lmp_df.rename(columns={"VALUE": "LMP"})
        lmp_df = lmp_df[["START_TIME_PST", "LMP"]]

        demand_df = demand_df[["START_TIME_PST", "MW"]]

        # Merge lmp and demand dfs
        market_data_df = pd.merge(lmp_df, demand_df, how="outer", on="START_TIME_PST")
        market_data_df = market_data_df.sort_values("START_TIME_PST")

        # Front fill data
        market_data_df = market_data_df.ffill()

    if market == "isone":
        # Convert timestamp to local timezone and check for missing timestamps
        lmp_df["START_TIME_EST"] = pd.to_datetime(lmp_df["BeginDate"])
        check_missing_data(lmp_df['START_TIME_EST'], lmp_df['START_TIME_EST'].iloc[0], lmp_df['START_TIME_EST'].iloc[-1], '5T')

        demand_df["START_TIME_EST"] = pd.to_datetime(demand_df["BeginDate"])
        check_missing_data(demand_df['START_TIME_EST'], demand_df['START_TIME_EST'].iloc[0], demand_df['START_TIME_EST'].iloc[-1], '5T')

        # Drop unnecessary columns
        lmp_df = lmp_df.rename(columns={"LmpTotal": "LMP"})
        lmp_df = lmp_df[["START_TIME_EST", "LMP"]]

        demand_df = demand_df.rename(columns={"LoadMw": "MW"})
        demand_df = demand_df[["START_TIME_EST", "MW"]]

        # Merge lmp and demand dfs
        market_data_df = pd.merge(lmp_df, demand_df, how="outer", on="START_TIME_EST")
        market_data_df = market_data_df.sort_values("START_TIME_EST")

    return market_data_df


def get_caiso_data(node, start_date, end_date):
    """
    Extract CAISO LMP and demand data for the given node and date range.
    :param node: str - CAISO node as input by the user
    :param start_date: str - Start date of data to pull
    :param end_date: str - End date of data to pull
    :return: dict - Keys are the names of the dfs, values are the dfs (lmp_df, demand_df)
    """
    verbose(f"Pulling CAISO data from {start_date} to {end_date}")

    # Formulate components of the URL request
    base_url = "http://oasis.caiso.com/oasisapi/SingleZip"
    result_format = "6"

    # If start and end date are >31 days apart, break them up (CAISO API requirement)
    lmp_df_list = []
    demand_df_list = []
    dates_list = get_dates("caiso",
        start_date_str=start_date, end_date_str=end_date, max_interval=31
    )

    for dates_dict in dates_list:
        temp_start_date = dates_dict["start_date_str"]
        temp_end_date = dates_dict["end_date_str"]

        # Get LMP data
        query_name = "PRC_INTVL_LMP"
        market_run_id = "RTM"
        version = "3"
        lmp_url = f"{base_url}?resultformat={result_format}&queryname={query_name}&startdatetime={temp_start_date}&enddatetime={temp_end_date}&version={version}&market_run_id={market_run_id}&node={node}"
        lmp_df = extract_zip(url=lmp_url)
        lmp_df_list.append(lmp_df)

        # Get demand data
        query_name = "SLD_FCST"
        market_run_id = "ACTUAL"
        version = "1"
        demand_url = f"{base_url}?resultformat={result_format}&queryname={query_name}&startdatetime={temp_start_date}&enddatetime={temp_end_date}&version={version}&market_run_id={market_run_id}"
        demand_df = extract_zip(url=demand_url)
        demand_df_list.append(demand_df)

    market_data_df = clean_market_data(
        market="caiso",
        lmp_df=pd.concat(lmp_df_list),
        demand_df=pd.concat(demand_df_list),
    )
    return market_data_df


def get_isone_data(node, start_date, end_date):
    """
    Extract ISO-NE LMP and demand data for the given node and date range.
    :param node: str - CAISO node as input by the user
    :param start_date: str - Start date of data to pull
    :param end_date: str - End date of data to pull
    """
    verbose(f"Pulling ISONE data from {start_date} to {end_date}")
    credentials_dict = get_credentials()

    # Formulate components of the URL request
    base_url = "https://webservices.iso-ne.com/api/v1.1"
    file_type = ".json"

    # Loop through all dates in range, one day at a time (ISONE API requirement)
    lmp_df_list = []
    demand_df_list = []
    dates_list = get_dates("isone",
        start_date_str=start_date, end_date_str=end_date, max_interval=1
    )

    for dates_dict in dates_list:
        temp_start_date = dates_dict["start_date_str"]

        # Get LMP data
        endpoint = f"/fiveminutelmp/day/{temp_start_date}/location/{node}"
        response = requests.get(
            base_url + endpoint + file_type,
            auth=HTTPBasicAuth(credentials_dict["user"], credentials_dict["pwd"]),
        )
        response_dict = response.json()["FiveMinLmps"]
        lmp_df = pd.concat(
            [pd.DataFrame(v) for k, v in response_dict.items()], keys=response_dict
        )
        lmp_df_list.append(lmp_df)

        # Get 5 min RT demand data
        endpoint = f"/fiveminutesystemload/day/{temp_start_date}"
        response = requests.get(
            base_url + endpoint + file_type,
            auth=HTTPBasicAuth(credentials_dict["user"], credentials_dict["pwd"]),
        )
        response_dict = response.json()["FiveMinSystemLoads"]
        demand_df = pd.concat(
            [pd.DataFrame(v) for k, v in response_dict.items()], keys=response_dict
        )
        demand_df_list.append(demand_df)

    market_data_df = clean_market_data(
        market="isone",
        lmp_df=pd.concat(lmp_df_list),
        demand_df=pd.concat(demand_df_list),
    )
    return market_data_df


def get_market_data(market, node, start_date, end_date):
    """
    Get market data (LMP and demand data) for the given market, node, and date range.
    :param market: str - Market of pulled data
    :param node: str - Node as input by the user
    :param start_date: str - Start date of data to pull
    :param end_date: str - End date of data to pull
    :return: dataframe - df of complete market data
    """
    if market == "caiso":
        market_data_df = get_caiso_data(
            node=node, start_date=start_date, end_date=end_date
        )
    elif market == "isone":
        market_data_df = get_isone_data(
            node=node, start_date=start_date, end_date=end_date
        )
    return market_data_df


def writeglm(
    market, node, start_date, end_date, market_data):
    """Write market data object.
    name is name of obj in gld
    Default GLM and CSV values are handled as follows
    GLM    CSV    Output
    ------ ------ ------
    None   None   CSV->stdout
    GLM    None   GLM, CSV->GLM/.glm/.csv
    None   CSV    GLM->stdout, CSV
    GLM    CSV    GLM, CSV
    The default name is "market_data@<market>_<node>_<start_date>-<end_date>"
    """
    float_format = "%.1f"
    date_format = "%Y-%m-%d %H:%M:%S"
    name = f"market_data@{market}_{node}_{start_date}-{end_date}"
    glm = f"{market}_{node}_{start_date}-{end_date}.glm"
    csv = f"{market}_{node}_{start_date}-{end_date}.csv"

    with open(glm, "w") as f:
        f.write(f"class market_data\n{{\n")
        for column in market_data.columns:
            dtype = market_data[column].dtype
            if dtype == object:
                gld_dtype = "char1024"
            elif dtype == int:
                gld_dtype = "int64"
            elif dtype == float:
                gld_dtype = "double"
            elif is_datetime(market_data[column]):
                gld_dtype = "timestamp"
            else:
                error(
                    f"Datatype <{dtype}> in column <{column}> does not have a corresponding datatype for GLD."
                )
            f.write(f"\t{gld_dtype} {column};\n")
        f.write("}\n")
        market_data.columns = list(map(lambda x: x.split("[")[0], market_data.columns))
        f.write("module tape;\n")
        f.write(f"#ifdef MARKET_DATA\n")
        f.write(
            f"#set MARKET_DATA=$MARKET_DATA {name}\n"
        )  # add curly braces around MARKET_DATA
        f.write("#else\n")
        f.write(f"#define MARKET_DATA={name}\n")
        f.write("#endif\n")
        f.write(f"object market_data\n{{\n")
        f.write(f'\tname "{name}";\n')
        f.write(f"\tmarket {market};\n")
        f.write(f"\tnode {node};\n")
        f.write("\tobject player\n\t{\n")
        f.write(f'\t\tfile "{csv}";\n')
        f.write(f"\t\tproperty \"{','.join(market_data.columns)}\";\n")
        f.write("\t};\n")
        f.write("}\n")

    market_data.to_csv(csv, float_format=float_format, date_format=date_format)
    return dict(glm=glm, csv=csv, name=name)


def error(msg, code=None):
    """Display and error message and exit if code is a number."""
    if debug_enable:
        raise Exception(msg)
    if not quiet_enable:
        print(f"ERROR [market_data]: {msg}", file=sys.stderr)
    if type(code) is int:
        exit(code)

def warning(msg):
    if warning_enable:
        print(f"VERBOSE [market_data]: {msg}",file=sys.stderr)

def verbose(msg):
    if verbose_enable:
        print(f"VERBOSE [market_data]: {msg}",file=sys.stderr)

def syntax(code=0):
    """Display docs (code=0) or syntax help (code!=0) and exit."""
    if code == 0:
        print(__doc__)
    else:
        print(
            f"Syntax: {os.path.basename(sys.argv[0]).replace('.py', '')} [-m|--market=MARKETNAME] [-d|--node=NODE] ["
            f"-s|--startdate=STARTDATETIME] [-e|--enddate=ENDDATETIME] [-h|--help|help]"
        )
    exit(code)


if __name__ == "__main__":
    market = None
    node = None
    start_date = None
    end_date = None
    glm = None
    name = None
    csv = None

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

        if token in ["-h", "--help", "help"]:
            syntax()
        elif token in ["-v","--verbose"]:
            verbose_enable = True
        elif token in ["--debug"]:
            debug_enable = True
        elif token in ["-w","--warning"]:
            warning_enable = False
        elif token in ["-q","--quiet"]:
            quiet_enable = True
        elif token in ["-m", "--market"]:
            market = value.lower()
        elif token in ["-d", "--node"]:
            node = value
        elif token in ["-s", "--startdate"]:
            if bool(re.match(r"\d{8}", value)):
                start_date = value
            else:
                error("Date syntax: YYYYMMDD", code=1)
        elif token in ["-e", "--enddate"]:
            if bool(re.match(r"\d{8}", value)):
                end_date = value
            else:
                error("Date syntax: YYYYMMDD", code=1)
        elif token in ["--credentials"]:
            if not market in  ["isone"]:
                error(f"{market.upper()} does not require credentials",E_INVALID)
            elif sys.stdin.isatty():
                if not os.system("open 'https://www.iso-ne.com/isoexpress/login?p_p_id=58&p_p_lifecycle=0&p_p_state=normal&p_p_mode=view"
                    "&saveLastPath=0&_58_struts_action=%2Flogin%2Fcreate_account'"):
                    error(
                        "unable to open a browser window to get credentials, please visit \n "
                        "https://www.iso-ne.com/isoexpress/login?p_p_id=58&p_p_lifecycle=0&p_p_state=normal&p_p_mode=view"
                        "&saveLastPath=0&_58_struts_action=%2Flogin%2Fcreate_account"
                    )
                user = getpass.getpass(prompt="Username: ")
                pwd = getpass.getpass(prompt="Password: ")
                with open(f"{os.getcwd()}/credentials.json", "w") as outfile:
                    credentials = {"username": user, "password": pwd}
                    json.dump(credentials, outfile)
                exit(E_OK)
            else:
                error("cannot get credentials (not a tty)",code=2)

        elif token in ["-g", "--glm"]:
            glm = value
        elif token in ["-n", "--name"]:
            name = value
        elif token in ["-c", "--csv"]:
            csv = value
        else:
            error(f"option '{token}' is not valid")
    if None not in (market, node, start_date, end_date):
        market_data_df = get_market_data(
            market=market, node=node, start_date=start_date, end_date=end_date
        )
        writeglm(
            market=market,
            node=node,
            start_date=start_date,
            end_date=end_date,
            market_data=market_data_df,
        )
    else:
        error(
            "market (-m), node (-d), start date (-s), and end date (-e) are required parameters"
        )
        if market is None:
            error("Markets can be: 'caiso', 'isone'", code=1)
        elif node is None:
            error(
                "User must indicate a node. For a list of CAISO nodes, download the latest 'Full Network Model "
                "Pricing Node Mapping' from the CAISO website. For a list of ISONE nodes, downloaded the latest "
                "'PNODE Table' from the ISONE website.",
                code=1,
            )
