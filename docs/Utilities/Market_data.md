[[/Utilities/Market_data]] - ISO market data download tool

# Synopsis

Shell:

    bash$ gridlabd market_data [-m|--market=MARKETNAME] [-d|--node=NODE] [-s|--startdate=STARTDATE]
    [-e|--enddate=ENDDATE] [-h|--help|help] [--credentials[=FILENAME.json]]
    [-n|--name=OBJNAME] [-c|--csv=CSVFILE] [-g|--glm=GLMFILE]

GLM:

    #python ${GLD_ETC}/market_data.py [-m|--market=MARKETNAME] [-d|--node=NODE] [-s|--startdate=STARTDATE]
    [-e|--enddate=ENDDATE] [-h|--help|help] [--credentials[=FILENAME.json]]
    [-n|--name=OBJNAME] [-c|--csv=CSVFILE] [-g|--glm=GLMFILE]

Python:

    bash$ gridlabd python
    >>> import market_data as md
    >>> data = md.get_market_data(MARKETNAME,NODE,STARTDATE,ENDDATE[,CREDENTIALS])
    >>> md.write_csv(data,CSVNAME)
    >>> md.write_glm(data,GLMNAME,[NAME])

# Description

The market_data tool obtain wholesale electricity market price data from
independent system operators (ISO).  The ISO must be specified using the
MARKETNAME parameter. Currently supported markets are "CAISO" and "ISONE". 

In addition, the NODE must be specified when obtaining locational price data.
Lists of node names may be obtained from the market websites.  For CAISO see
http://www.caiso.com/TodaysOutlook/Pages/prices.html for a map of available
price nodes.  For ISONE see https://www.iso-ne.com/markets-operations/settlements/pricing-node-tables/

The STARTDATE and ENDDATE value must be specified in the form "YYYYMMDD".

When the `--credentials` option is used without parameters, the user is
prompted for a username and password. When run from a terminal window, a
browser window is automatically opened for the specific ISO website to
facilitate obtaining the username and password. The result is stored in the
default credentials file for GridLAB-D ISO credentials
(i.e., `$HOME/.gridlabd/iso_credentials.json`).  When FILENAME is provided
with the credentials option, the credentials are loaded from the specified
file instead. Currently only the  ISONE API requires credentials. For
example, to generate credentials for ISONE, use the command

    bash$ gridlabd market_data -m=ISONE --credentials

By default the CSV data is output to the stdout, which may be redirected to a
file.  If no GLM file is specified, then the CSV will contain a header row
indicating which columns contain the price and quantity data.  If a GLM file
is specified, then the CSV data must be output to a file.  By default the
GridLAB-D object name for the market data is generated automatically to
ensure it is unique.  The object name may be specified using the `--name`
option.

# Example

    bash$ gridlabd market_data -m=CAISO -d=0096WD_7_N001 -s=20220222 -e=20220223
    START_TIME_PST,LMP,MW
    2022-02-22 00:00:00,50.0,21372.0
    2022-02-22 00:05:00,50.9,21372.0
    2022-02-22 00:10:00,50.3,21372.0
    2022-02-22 00:15:00,49.8,21372.0
    ...

# See also

* [CAISO](https://caiso.com/)
* [ISONO](https://isone.com/)
