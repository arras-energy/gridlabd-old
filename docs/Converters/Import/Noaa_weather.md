[[/Converters/Import/Noaa_weather]] -- NOAA weather import

# Synopsis

GLM:

~~~
#input "<noaa-lcd-download.csv>" -o <filename>.glm -f noaa-weather -t weather 
       -p station_id=<station-id> -p latitude=<latitude> -p longitude=<longitude> -p timezone=<tzinfo> 
       [-p ground_albedo=<real>] [-p refresh={TRUE,FALSE}]
~~~

Shell:

~~~
bash$ gridlabd -D csv_load_options="-o <filename>.glm -f noaa-weather -t weather 
      -p station_id=<station-id> -p latitude=<latitude> -p longitude=<longitude> -p timezone=<tzinfo> 
      [-p ground_albedo=<real>] [-p refresh={TRUE,FALSE}]" "<noaa-lcd-download.csv>" <modelname>.glm 
~~~

# Description

The NOAA weather converter allows you to use a downloaded copy of data from [NOAA LCD Datatools](https://www.ncdc.noaa.gov/cdo-web/datatools/lcd).

The converter creates two files using the weather data, a CSV file containing the actual data, and the GLM file specified by the `-o <filename>.glm` option.

The CSV file is formatted to meet the requirements of a standard `tape.player` object.  The GLM file contains a class declaration for the weather object, defined as follows:

~~~
class weather {
	char32 station_id;
	double temperature[degF];
	double humidity[%];
	double wind_speed[mph];
	double wind_dir[deg];
	double pressure[mbar];
	double rainfall[in/h];
	double opq_sky_cov[pu];
	double tot_sky_cov[pu];
	double solar_direct[W/sf];
	double solar_diffuse[W/sf];
	double solar_global[W/sf];
	double ground_reflectivity[pu];
	}	
~~~

In addition, the GLM file contains an object declaration for the weather data, with an embedded player to load the CSV data into the weather object. This is defined as follows:

~~~
object weather
{
	name "<csvname>";
	station_id <station-id>;
	object player
	{
		file "test_data.csv";
		property "temperature,humidity[%],wind_speed[mph],wind_dir[deg],pressure[inHg],rainfall[in/h],opq_sky_cov[pu],solar_global[W/sf],solar_direct[W/sf],solar_diffuse[W/sf];
	};
}
~~~

To obtain data from NOAA, request a download from [NOAA LCD Datatools](https://www.ncdc.noaa.gov/cdo-web/datatools/lcd).  When your data request is fulfilled you may specify the URL emailed to you by NOAA as the input name. However, there is no guarantee that NOAA will preserve the endpoint indefinitely.

The solar calculations are based on the Zhang-Huang model, which estimates the total, direct, and diffuse solar irradiance based on the prevailing weather conditions, time-of-day, and day of year.

The ground reflectivity may be specified using the `ground_albedo` option. Note that this value is static and does not take into account the effect of accumulated snow on the ground recorded in the NOAA data.

# Example

The following example illustrates loading weather data from a test dataset storage on AWS S3.

~~~
clock 
{
	starttime "2018-01-01 00:00:00";
	stoptime "2019-01-01 00:00:00";
}

#input "https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/test_data/noaa.csv" -o test_data.glm -f noaa-weather -t weather -p station_id=72594524283 -p latitude=37.7749 -p longitude=-122.4194 -p timezone=-08:00

module tape;
object recorder
{
	parent "test_data";
	file "weather_recorder.csv";
	property "temperature,humidity";
}
~~~

# See also

* [[/Command/Automatic_import_conversion]]
* [NOAA LCD Datatools](https://www.ncdc.noaa.gov/cdo-web/datatools/lcd)
* [NOAA LCD Data Dictionary](ftp://ftp.ncdc.noaa.gov/pub/data/cdo/documentation/LCD_documentation.pdf)
* [Zhang-Huang Solar Model Documentation](https://simulationresearch.lbl.gov/dirpubs/51436.pdf)
