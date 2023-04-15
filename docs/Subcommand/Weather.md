[[/Subcommand/Weather]] -- GridLAB-D CLI to manage the local weather data 

# Synopsis

Shell:

~~~
bash$ gridlabd weather help
bash$ gridlabd weather index <pattern>
bash$ gridlabd weather list <pattern>
bash$ gridlabd weather get <pattern>
bash$ gridlabd weather delete <pattern>
bash$ gridlabd weather info <pattern>
bash$ gridlabd weather clean
bash$ gridlabd weather config {show,reset}
bash$ gridlabd weather metar [--index|<station-list>]
~~~

# Description

Weather data is archived in a GitHub repository. The `weather` subcommand manages the local copies of archived weather data in the shared GridLAB-D folder. This allow local system to only have needed weather data and avoid having to download the entire archive from GitHub, which is quite large.

## `index`

~~~
  gridlabd weather index <pattern>
~~~

The `index` subcommand is used to obtain lists of weather files in the archive. The following command obtains a list of all weather files in the archive

~~~
bash$ gridlabd weather index

  AK-Adak_Nas.tmy3
  AK-Ambler.tmy3
  AK-Anaktuvuk_Pass.tmy3
  ...
  WY-Rock_Springs_Arpt_Green_River_Uo.tmy3
  WY-Sheridan_County_Arpt.tmy3
  WY-Worland_Municipal.tmy3
~~~

The following command obtains a list of all the California weather files in the archive

~~~
bash$ gridlabd weather index ^CA

  CA-Alturas.tmy3
  CA-Arcata_Airport.tmy3
  CA-Bakersfield_Meadows_Field.tmy3
  ...
  CA-Van_Nuys_Airport.tmy3
  CA-Visalia_Muni_Awos.tmy3
  CA-Yuba_Co.tmy3
~~~

The following command obtains a list of the weather files in the archive that have the word "San" in their name

~~~
bash$ gridlabd weather index San

  AK-Sand_Point.tmy3
  CA-San_Diego_Lindbergh_Field.tmy3
  CA-San_Diego_Miramar_Nas.tmy3
  ...
  TX-San_Antonio_Intl_Ap.tmy3
  TX-San_Antonio_Kelly_Field_Afb.tmy3
  TX-San_Antonio_Stinson.tmy3
~~~

## `get`

~~~
  gridlabd weather get <pattern>
~~~

The `get` command downloads the specified weather files from the archive.

To download a single weather file use the command:

~~~
bash$ gridlabd weather get CA-San_Diego_Lindbergh_Field.tmy3
  Downloading CA-San_Diego_Lindbergh_Field.tmy3... done
~~~

To download all the weather files in a state, use the command:

~~~
bash$ gridlabd weather get ^CA

  Downloading CA-Alturas.tmy3... done
  Downloading CA-Arcata_Airport.tmy3... done
  Downloading CA-Bakersfield_Meadows_Field.tmy3... done
  ...
  Downloading CA-Van_Nuys_Airport.tmy3... done
  Downloading CA-Visalia_Muni_Awos.tmy3... done
  Downloading CA-Yuba_Co.tmy3... done
~~~

To download all the weather files matching a name pattern, use the command:

~~~
bash$ gridlabd weather get San_Diego
  Refreshing CA-San_Diego_Lindbergh_Field.tmy3... done
  Refreshing CA-San_Diego_Miramar_Nas.tmy3... done
  Refreshing CA-San_Diego_Montgomer.tmy3... done
  Refreshing CA-San_Diego_North_Island_Nas.tmy3... done
~~~

## `list`

~~~
  gridlabd weather list <pattern>
~~~

The `list` command displays a list of the local weather files that have been downloaded from the archive.

To list a single weather file use the command:
~~~
bash$ gridlabd weather list CA-San_Diego_Lindbergh_Field.tmy3
  CA-San_Diego_Lindbergh_Field.tmy3
~~~

To list all the weather files in a state, use the command:

~~~
bash$ gridlabd weather list ^CA
  CA-Alturas.tmy3
  CA-Arcata_Airport.tmy3
  CA-Bakersfield_Meadows_Field.tmy3
  ...
  CA-Van_Nuys_Airport.tmy3
  CA-Visalia_Muni_Awos.tmy3
  CA-Yuba_Co.tmy3
~~~

To list all the weather files matching a name pattern, use the command:

~~~
bash$ gridlabd weather list San_Diego

  CA-San_Diego_Lindbergh_Field.tmy3
  CA-San_Diego_Miramar_Nas.tmy3
  CA-San_Diego_Montgomer.tmy3
  CA-San_Diego_North_Island_Nas.tmy3
~~~

## `delete`

~~~
  gridlabd weather delete <pattern>
~~~

The `delete` command deletes local weather files that have been downloaded from the archive.

To delete a single weather file use the command:

~~~
bash$ gridlabd weather delete CA-San_Diego_Lindbergh_Field.tmy3
~~~

To delete all the weather files in a state, use the command:

~~~
bash$ gridlabd weather delete ^CA
~~~

To delete all the weather files matching a name pattern, use the command:

~~~
bash$ gridlabd weather delete San_Diego
~~~

## `info`

~~~
  gridlabd weather info <pattern>
~~~

The <tt>info</tt> command displays information about local weather files that have been downloaded from the archive.

To display information about a single downloaded weather file use the command:

~~~
bash$ gridlabd weather info CA-San_Diego_Lindbergh_Field.tmy3
  Filepath,StationId,StationName,RegionName,TzOffset,Latitude,Longitude,Elevation
  "/usr/local/share/gridlabd/US/CA-San_Diego_Lindbergh_Field.tmy3",722900,"SAN DIEGO LINDBERGH FIELD",CA,-8.0,32.733,-117.167,4
~~~

To display information about all the downloaded weather files in a state, use the command:

~~~
bash$ gridlabd weather info ^CA

  Filepath,StationId,StationName,RegionName,TzOffset,Latitude,Longitude,Elevation
  "/usr/local/share/gridlabd/US/CA-Alturas.tmy3",725958,"ALTURAS",CA,-8.0,41.500,-120.533,1341
  "/usr/local/share/gridlabd/US/CA-Arcata_Airport.tmy3",725945,"ARCATA AIRPORT",CA,-8.0,40.983,-124.100,62
  "/usr/local/share/gridlabd/US/CA-Bakersfield_Meadows_Field.tmy3",723840,"BAKERSFIELD MEADOWS FIELD",CA,-8.0,35.433,-119.050,149
  ...
  "/usr/local/share/gridlabd/US/CA-Van_Nuys_Airport.tmy3",722886,"VAN NUYS AIRPORT",CA,-8.0,34.217,-118.483,235
  "/usr/local/share/gridlabd/US/CA-Visalia_Muni_Awos.tmy3",723896,"VISALIA MUNI (AWOS)",CA,-8.0,36.317,-119.400,89
  "/usr/local/share/gridlabd/US/CA-Yuba_Co.tmy3",724838,"YUBA CO",CA,-8.0,39.100,-121.567,19
~~~

To display information about all the downloaded weather files matching a name pattern, use the command:

~~~
bash$ gridlabd weather info San_Diego

  Filepath,StationId,StationName,RegionName,TzOffset,Latitude,Longitude,Elevation
  "/usr/local/share/gridlabd/US/CA-San_Diego_Lindbergh_Field.tmy3",722900,"SAN DIEGO LINDBERGH FIELD",CA,-8.0,32.733,-117.167,4
  "/usr/local/share/gridlabd/US/CA-San_Diego_Miramar_Nas.tmy3",722930,"SAN DIEGO MIRAMAR NAS",CA,-8.0,32.867,-117.133,140
  "/usr/local/share/gridlabd/US/CA-San_Diego_Montgomer.tmy3",722903,"SAN DIEGO/MONTGOMER",CA,-8.0,32.817,-117.133,129
  "/usr/local/share/gridlabd/US/CA-San_Diego_North_Island_Nas.tmy3",722906,"SAN DIEGO NORTH ISLAND NAS",CA,-8.0,32.700,-117.200,15
~~~

The information is output in CSV format for convenient manipulation using standard CSV readers. For example, the following command generates this output

~~~
bash$ gridlabd weather info San_Diego

  Filepath,StationId,StationName,RegionName,TzOffset,Latitude,Longitude,Elevation
  "/usr/local/share/gridlabd/US/CA-San_Diego_Lindbergh_Field.tmy3",722900,"SAN DIEGO LINDBERGH FIELD",CA,-8.0,32.733,-117.167,4
  "/usr/local/share/gridlabd/CA-San_Diego_Miramar_Nas.tmy3",722930,"SAN DIEGO MIRAMAR NAS",CA,-8.0,32.867,-117.133,140
  "/usr/local/share/gridlabd/US/CA-San_Diego_Montgomer.tmy3",722903,"SAN DIEGO/MONTGOMER",CA,-8.0,32.817,-117.133,129
  "/usr/local/share/gridlabd/US/CA-San_Diego_North_Island_Nas.tmy3",722906,"SAN DIEGO NORTH ISLAND NAS",CA,-8.0,32.700,-117.200,15
~~~

## `clean`

~~~
  gridlabd weather clean
~~~

The `clean` removes existing data from the archive index cache and refreshes the current index.

## `config`

~~~
  gridlabd weather config {show,reset}
~~~

The weather data configuration file `gridlabd-weather.conf` is located in the `${GLD_ETC}` folder in the installation folder. (By default the local installation prefix is `/usr/local` but the system administrator can change the installation folder using the `--prefix` option when building a custom install of `gridlabd`.) You can change the default behavior of the `weather` subcommand by creating and altering the weather data configuration file.  

To get a list of the current weather data configuration use the command


~~~
bash$ gridlabd weather config show
  GITHUB="https://github.com"
  GITHUBUSERCONTENT="https://raw.githubusercontent.com"
  COUNTRY="US"
  GITUSER="hipas"

  GITREPO="gridlabd-weather"
  GITBRANCH="master"
  DATADIR="/usr/local/share/gridlabd/weather"
  SVNLSOPTIONS="--config-option=servers:global:http-timeout=60 --non-interactive -r1"
~~~

You can use these values as a guide to creating a customized configuration file. When the file is not present, these following default values are used automatically.

To change the folder in which your weather data files are stored, use the command:

~~~
bash$ gridlabd weather config set DATADIR /tmp/weather
~~~

To see where the weather data is stored, use the the command:

~~~
bash$ gridlabd weather config get DATADIR
  /tmp/weather
~~~

To reset the weather data configuration, use the command

~~~
bash$ gridlabd weather config reset
~~~

## `metar`

Creates weather objects for NOAA METAR weather stations. To get a list of available stations use the following command:

~~~
bash$ gridlabd weather metar --index
PADK
PAKH
PAUT
PZ74
...
~~~

To get the weather object for a station use the following command:

~~~
bash$ gridlabd weather metar KSFO
// generated by metar2glm.py on 2021-08-27T21:00:48.545274Z

class weather
{
  char8 country;
  char8 region;
  char32 station;
  double elevation[ft];
  timestamp time;
  double temperature[degF];
  double dew_point[degF];
  double wind_speed[m/s];
  double wind_dir[deg];
  double visibility[mile];
  double pressure[mbar];
  char256 clouds;
  char1024 metar;
  on_init "python:metar_weather.weather_init";
}

object weather
{
    country "US";
    region "CA";
    station "SAN FRANCISCO";
    latitude "37.62";
    longitude "-122.37";
    elevation "2.0";
    name "KSFO";
    time "2021-08-27 19:56:00 UTC";
    temperature 24.0 degC;
    dew_point 17.0 degC;
    wind_speed 7.0 knot;
    visibility 14484.096000000001 m;
    pressure 1008.1306845999999 mbar;
    clouds "a few clouds at 1000 feet";
    metar "KSFO 271956Z 01007KT 9SM FEW010 24/17 A2977 RMK AO2 SLP079 T0239017";
}
~~~

Multiple stations may be listed, so that:

~~~
bash$ gridlabd weather metar KSFO KSEA
~~~

will output multiple objects, one for each station.

# Environment Variables

The weather subcommand needs to know where you wish to store the weather files.  The environment variable `GLD_ETC` is used to indicate where the `weather` folder is.  By default this is the `/usr/local/share/gridlabd` folder.

# Tutorial

[video:KTeOFbt-aiE]

