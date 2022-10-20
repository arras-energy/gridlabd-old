"""Download Elevation Data from USGS

This script download all the USGS 30 meter elevation data.  You must register at
USGS at https://e4ftl01.cr.usgs.gov/ to obtain a login id and password. Place
the username and password in the file $HOME/.earthdata/credentials in the format
username:password.

The scripts will download all the files in the range of EAST/WEST/NORTH/SOUTH.
The files will be stored in the CACHEDIR.

Normally this data is stored on AWS in the S3 bucket geodata.gridlabd.us. Files
can be obtained using the template

    https://geodata.gridlabd.us/elevation/30m/<LAT>N_<LON>W.tif

where <LAT>,<LON> is the integer portion only of the position.  The files are
delivered at 1 arcsecond resolution, and thus are 3600x3600 pixels with
elevations presented in meters.

TODO: this script needs to upload the images to the S3 bucket for gridlabd.
"""
import os, sys
import requests
import math
from zipfile import ZipFile

# the name of this app
NAME = "download_elevation"

# the lat/lon range of data to download
EAST=-65
WEST=-125
SOUTH=24
NORTH=50

# enable verbose output
VERBOSE = False

# enable debug output
DEBUG = False

ROOTURL = "https://e4ftl01.cr.usgs.gov/ASTT/ASTGTM.003/2000.03.01/ASTGTMV003_N"
TMPDIR = "/tmp"
GLD_ETC = os.getenv("GLD_ETC")
CACHEDIR = "{GLD_ETC}/gridlabd/geodata/elevation/30m"
HOMEDIR = os.getenv("HOME")
CONFIGDIR = f"{HOMEDIR}/.earthdata"

#
# utility functions
#
def error(msg,exitcode=None):
    if DEBUG:
        raise msg
    else:
        print(f"ERROR [{NAME}]: {msg}")
    if exitcode != None:
        exit(exitcode)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{NAME}]: {msg}")

class SessionWithHeaderRedirection(requests.Session):
    AUTH_HOST = 'urs.earthdata.nasa.gov'
    def __init__(self, username, password):
        super().__init__()
        self.auth = (username, password)

    def rebuild_auth(self, prepared_request, response):
        headers = prepared_request.headers
        url = prepared_request.url
        if 'Authorization' in headers:
            original_parsed = requests.utils.urlparse(response.request.url)
            redirect_parsed = requests.utils.urlparse(url)
            if (original_parsed.hostname != redirect_parsed.hostname) and \
                    redirect_parsed.hostname != self.AUTH_HOST and \
                    original_parsed.hostname != self.AUTH_HOST:
                del headers['Authorization']
        return

def download_elevation(pos):
    """Download elevation

    Download elevation data for the specified position.

    ARGUMENTS:
        pos (lat,lon)  The specified position

    RETURNS:
        None

    EXCEPTIONS:
        Varies  If DEBUG is enabled, all errors are exceptions
    """
    try:
        os.makedirs(CACHEDIR,exist_ok=True)

        if pos[0] < 0:
            lat = f"{-math.floor(pos[0])}S"
        elif pos[0] > 0:
            lat = f"{math.floor(pos[0])}N"
        else:
            lat = "0"
        if pos[1] < 0:
            lon = f"{-math.floor(pos[1])}W"
        elif pos[0] > 0:
            lon = f"{math.floor(pos[1])}E"
        else:
            lon = "0"

        zipfile = f"{TMPDIR}/{lat}_{lon}.zip"
        tiffile = f"{CACHEDIR}/{lat}_{lon}.tif"
        if not os.path.exists(tiffile):

            verbose(f"{tiffile} not found, downloading {zipfile}...")

            url = f"{ROOTURL}{abs(math.floor(pos[0]))}W{abs(math.floor(pos[1])):03d}.zip"
            verbose(f"setting url to {url}")

            response = session.get(url, stream=True)
            response.raise_for_status()
            with open(zipfile, 'wb') as fd:
                for chunk in response.iter_content(chunk_size=1024*1024):
                    fd.write(chunk)
            verbose(f"saved zipfile to {zipfile}")

            with ZipFile(zipfile,"r") as zip:
                files = zip.namelist()
                for file in files:
                    if file.endswith(f"{abs(math.floor(pos[0]))}W{abs(math.floor(pos[1])):03d}_dem.tif"):
                        verbose(f"extracting {file} to {TMPDIR}")
                        zip.extract(file,TMPDIR)
                        verbose(f"move {TMPDIR}/{file} to {tiffile}")
                        os.rename(f"{TMPDIR}/{file}",tiffile)
                    else:
                        verbose(f"skipping {file}")
            os.remove(zipfile)
            verbose(f"")
        else:
            verbose(f"{tiffile} is already downloaded")

    except Exception as err:

        error(err)

def main(east,west,north,south):
    for lat in range(south,north):
        for lon in range(west,east):
            print(f"Processing {lat} {lon}")
            download_elevation([lat,lon])

if __name__ == '__main__':
    with open(f"{CONFIGDIR}/credentials","r") as fh:
        credentials=fh.read().strip().split(":")
    session = SessionWithHeaderRedirection(credentials[0],credentials[1])
    verbose(f"session started for user {credentials[0]}")
    main(east=EAST,west=WEST,north=NORTH,south=SOUTH)
