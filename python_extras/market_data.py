import datetime as dt
import time
import sys
import os
import re
import requests
from requests.auth import HTTPBasicAuth
import pandas as pd
import pytz
import getpass
import json

from urllib.request import urlopen
from zipfile import ZipFile

"""
Shell:
    bash$ gridlabd market_data [-m|--market=MARKETNAME] [-d|--node=NODE] [-s|--startdt=STARTDATETIME]
    [-e|--enddt=ENDDATETIME] [-h|--help|help] [--credentials]

Sample node for CAISO: 0096WD_7_N001
Sample node for ISO-NE: 4001
"""


def get_credentials():
    credentials_path = f"{os.getcwd()}/credentials.json"
    if os.path.exists(credentials_path) and os.path.getsize(credentials_path) > 0:
        with open(credentials_path) as credentials_file:
            credentials_dict = json.load(credentials_file)
            user = credentials_dict["username"]
            pwd = credentials_dict["password"]
    else:
        print(
            f"There is no valid credentials file at path {credentials_path}. Run 'python {os.path.basename(sys.argv[0])} --credentials' to create the file."
        )
        exit(1)
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
    Reformats the date string into the format required by the API.
    :param market: str - Market of pulled data
    :param date_str: str - Date in format returned by convert_timezone function
    :return: str - Date in format required by the API
    """
    if market == "caiso":
        formatted_str = date_dt.strftime("%Y%m%dT%H:%M-0000")
    elif market == "isone":
        formatted_str = date_dt.strftime("%Y%m%d")
    return formatted_str


def get_dates(start_date_str, end_date_str, max_interval):
    """
    Returns a list of dicts, where each dict has 2 keys: 'start_date_str' and 'end_date_str'. The different between
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


def create_folder(data_type, market, start_date, end_date):
    """
    Create a folder to store the output data.
    :param data_type: str - Type of pulled data ('lmp' or 'demand')
    :param market: str - Market of pulled data ('caiso' or 'isone')
    :param start_date: str - Start date of pulled data
    :param end_date: str - End date of pulled data
    :return: str - Relative path to created folder
    """
    # Create folder to store the output data
    folder = f"output/{market}_{data_type}_{start_date}-{end_date}"
    if not os.path.exists(folder):
        os.makedirs(folder)
    return folder


def extract_zip(url, folder):
    """
    Get a zip file from a url and extract it into the given folder
    :param url: str - URL to query
    :param folder: str - folder to save contents of zip file to
    """
    # Download the file from the URL
    zip_resp = urlopen(url)

    # Create new file on hard drive and write contents to it
    temp_zip = open("/tmp/tempfile.zip", "wb")
    temp_zip.write(zip_resp.read())
    temp_zip.close()

    # Extract contents into given path
    zf = ZipFile("/tmp/tempfile.zip")
    zf.extractall(path=os.getcwd() + "/" + folder)
    zf.close()

    # Wait
    time.sleep(3)


def get_caiso_data(node, start_date, end_date):
    """
    Extract CAISO LMP and demand data for the given node and date range.
    :param node: str - CAISO node as input by the user
    :param start_date: str - Start date of data to pull
    :param end_date: str - End date of data to pull
    """
    print(f"Pulling CAISO data from {start_date} to {end_date}")

    lmp_folder = create_folder(
        data_type="lmp", market="caiso", start_date=start_date, end_date=end_date
    )
    demand_folder = create_folder(
        data_type="demand", market="caiso", start_date=start_date, end_date=end_date
    )

    # Formulate components of the URL request
    base_url = "http://oasis.caiso.com/oasisapi/SingleZip"
    result_format = "6"

    # If start and end date are >31 days apart, break them up (CAISO API requirement)
    dates_list = get_dates(
        start_date_str=start_date, end_date_str=end_date, max_interval=31
    )
    for dates_dict in dates_list:
        start_date = dates_dict["start_date_str"]
        end_date = dates_dict["end_date_str"]

        # Get LMP data
        query_name = "PRC_INTVL_LMP"
        market_run_id = "RTM"
        version = "3"
        lmp_url = f"{base_url}?resultformat={result_format}&queryname={query_name}&startdatetime={start_date}&enddatetime={end_date}&version={version}&market_run_id={market_run_id}&node={node}"
        extract_zip(url=lmp_url, folder=lmp_folder)

        # Get demand data
        query_name = "SLD_FCST"
        market_run_id = "RTM"
        execution_type = "RTD"
        version = "1"
        demand_url = f"{base_url}?resultformat={result_format}&queryname={query_name}&startdatetime={start_date}&enddatetime={end_date}&version={version}&market_run_id={market_run_id}&execution_type={execution_type}"
        extract_zip(url=demand_url, folder=demand_folder)

    print(
        f"Process complete. Data saved in output/caiso_<data_type>_{start_date}-{end_date}."
    )


def get_isone_data(node, start_date, end_date):
    """
    Extract ISO-NE LMP and demand data for the given node and date range.
    :param node: str - CAISO node as input by the user
    :param start_date: str - Start date of data to pull
    :param end_date: str - End date of data to pull
    """
    print(f"Pulling ISONE data from {start_date} to {end_date}")
    credentials_dict = get_credentials()

    # Create folders to store the output data
    lmp_folder = create_folder(
        data_type="lmp", market="isone", start_date=start_date, end_date=end_date
    )
    demand_folder = create_folder(
        data_type="demand", market="isone", start_date=start_date, end_date=end_date
    )

    # Formulate components of the URL request
    base_url = "https://webservices.iso-ne.com/api/v1.1"
    file_type = ".json"

    # Loop through all dates in range, one at a time (ISONE API requirement)
    start_date_dt = dt.datetime.strptime(start_date, "%Y%m%d")
    end_date_dt = dt.datetime.strptime(end_date, "%Y%m%d")
    while start_date_dt <= end_date_dt:
        # Get LMP data
        endpoint = (
            f"/fiveminutelmp/day/{start_date_dt.strftime('%Y%m%d')}/location/{node}"
        )
        response = requests.get(
            base_url + endpoint + file_type,
            auth=HTTPBasicAuth(credentials_dict["user"], credentials_dict["pwd"]),
        )
        response_dict = response.json()["FiveMinLmps"]
        lmp_df = pd.concat(
            [pd.DataFrame(v) for k, v in response_dict.items()], keys=response_dict
        )
        lmp_df.to_csv(
            f"{lmp_folder}/isone_fiveminlmp_{start_date_dt.strftime('%Y%m%d')}.csv"
        )

        # Get 5 min RT demand data
        endpoint = f"/fiveminutesystemload/day/{start_date_dt.strftime('%Y%m%d')}"
        response = requests.get(
            base_url + endpoint + file_type,
            auth=HTTPBasicAuth(credentials_dict["user"], credentials_dict["pwd"]),
        )
        response_dict = response.json()["FiveMinSystemLoads"]
        demand_df = pd.concat(
            [pd.DataFrame(v) for k, v in response_dict.items()], keys=response_dict
        )
        demand_df.to_csv(
            f"{demand_folder}/isone_fivemindemand_{start_date_dt.strftime('%Y%m%d')}.csv"
        )

        # Run again for the next day in range
        start_date_dt += dt.timedelta(days=1)

    print(
        f"Process complete. Data saved in output/isone_<data_type>_{start_date}-{end_date}."
    )


def get_market_data(market, node, start_date, end_date):
    if market == "caiso":
        get_caiso_data(node=node, start_date=start_date, end_date=end_date)
    elif market == "isone":
        get_isone_data(node=node, start_date=start_date, end_date=end_date)


def error(msg, code=None):
    """Display and error message and exit if code is a number."""
    if code != None:
        print(f"ERROR [market_data.py]: {msg}", file=sys.stderr)
        exit(code)
    else:
        raise Exception(msg)


def syntax(code=0):
    """Display docs (code=0) or syntax help (code!=0) and exit."""
    if code == 0:
        print(__doc__)
    else:
        # TODO: Fix this syntax
        print(
            f"Syntax: {os.path.basename(sys.argv[0]).replace('.py', '')} [-m|--market=MARKETNAME] [-d|--node=NODE] ["
            f"-s|--startdt=STARTDATETIME] [-e|--enddt=ENDDATETIME] [-h|--help|help]"
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
        if token in ["-m", "--market", "market"]:
            market = value.lower()
        elif token in ["-d", "--node", "node"]:
            node = value
        elif token in ["-s", "--startdate", "startdate"]:
            if bool(re.match(r"\d{8}", value)):
                start_date = value
            else:
                error("Date syntax: YYYYMMDD", code=1)
        elif token in ["-e", "--enddate", "enddate"]:
            if bool(re.match(r"\d{8}", value)):
                end_date = value
            else:
                error("Date syntax: YYYYMMDD", code=1)
        elif token in ["--credentials"]:
            print(
                "ISONE API requires credentials. If you do not have them, please apply for access at: \n "
                "https://www.iso-ne.com/isoexpress/login?p_p_id=58&p_p_lifecycle=0&p_p_state=normal&p_p_mode=view"
                "&saveLastPath=0&_58_struts_action=%2Flogin%2Fcreate_account"
            )
            user = getpass.getpass(prompt="Username: ")
            pwd = getpass.getpass(prompt="Password: ")
            with open(f"{os.getcwd()}/credentials.json", "w") as outfile:
                credentials = {"username": user, "password": pwd}
                json.dump(credentials, outfile)
            exit()

        elif token in ["-g", "--glm"]:
            glm = value
        elif token in ["-n", "--name"]:
            name = value
        elif token in ["-c", "--csv"]:
            csv = value
        else:
            error(f"option '{token}' is not valid")
    if None not in (market, node, start_date, end_date):
        get_market_data(
            market=market, node=node, start_date=start_date, end_date=end_date
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
