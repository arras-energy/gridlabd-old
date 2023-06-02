[[/Subcommand/Loaddata]] -- Downloads building load data from the NREL ResStock and ComStock data lakes. 


# Synopsis

GLM:

~~~
#loaddata [OPTIONS ...]
~~~

Shell:

~~~
$ gridlabd loaddata [OPTIONS ...]
~~~

* '-a|--area=SF':           Building floor area (commercial buildings only), e.g., 10000
* '-b|--building_type=TYPE': Building type, e.g., "APARTMENT", "CONDO", "HOUSE", "TOWNHOUSE"
* '-c|--customer_type=CUST': Customer type, e.g., "RESIDENTIAL", "COMMERCIAL"
* '-e|--enduse[=ENDUSE]':    Enduse name, e.g., "total", none prints list
* '-f|--fuels=FUELS':        List of fuels, e.g., "electric,natural_gas"
* '-g|--glmname=GLMFILE':    GridLAB-D model file name, e.g., "loadmodel.glm"
* '-n|--name=OBJNAME':       GridLAB-D load object name, e.g., "my_load"
* '-o|--output=FILENAME':    Output data file name, e.g., "loaddata.csv", "loaddata.json"
* '-p|--precision=INT':      Output precision, e.g., 4
* '-r|--release=RELEASE':    Dataset release, e.g., "1.1"
* '-s|--state=STATE':        State, e.g., "CA"
* '-t|--timestep=TIMESTEP':  Timeseries resolution, e.g., "1H", "1D", "1W", etc.
* '-u|--upgrade=LEVEL':      Building upgrade, e.g.. 0 through 10
* '-v|--version=YEAR':       Dataset version, e.g., 2021, 2022, 2023
* '-w|--weather=WEATHER':    Weather type, e.g., "ACTUAL", "TYPICAL"
* '-y|--year=YEAR':          Actual weather year, e.g., 2012, 2018

# Description

Electric enduse units are in kWh/sf for commercial buildings and kWh for
residential buildings. All fossil enduse units are in kBtu/sf or kBtu. If
floor area is specified for commercial buildings, then all values are in kWh
or kBtu.

If a GLM file is generated, then the object name specified is used as the player's
parent object. If the object name is omitted, then the the player is given a random
object name, which is appended to the global `LOADDATA`.

The player's start and stop time are stored in the global variables `{objname}_starttime`
and `{objname}_stoptime`.

If fuels, enduse, or building_type are omitted, then a list of the available
values is output to /dev/stdout.

See https://data.openei.org/submissions/4520 for details on available
datasets, versions, building types, upgrades, etc.

# Examples

The following command outputs the total load electric data for a house in California

~~~
$ gridlabd loaddata -b=HOUSE -r=1.1 -s=CA -v=2022 -w=ACTUAL -y=2018 -e=total -f=electricity
                     electricity
timestamp                       
2017-12-31 19:15:00       0.2994
2017-12-31 19:30:00       0.3013
2017-12-31 19:45:00       0.2991
2017-12-31 20:00:00       0.2994
2017-12-31 20:15:00       0.2731
2017-12-31 20:30:00       0.2707
2017-12-31 20:45:00       0.2641
2017-12-31 21:00:00       0.2614
...
~~~

The following command outputs the hourly heating demand for electric and natural gas for a 100,000 sf hotel in California:

~~~
$ gridlabd loaddata -s=CA -r=1 -v=2023 -w=ACTUAL -y=2018 -b=HOTEL -e=heating -f=electricity,natural_gas -t=1H -a=100000
                     electricity  natural_gas
timestamp                                    
2017-12-31 19:00:00       3.4422      14.3716
2017-12-31 20:00:00       5.7445      20.7651
2017-12-31 21:00:00       6.2682      21.5445
2017-12-31 22:00:00       3.5732      18.0518
2017-12-31 23:00:00       2.9333      18.0711
2018-01-01 00:00:00       3.5650      19.2336
2018-01-01 01:00:00       4.8231      21.4534
2018-01-01 02:00:00       5.2353      22.0543
....
~~~
