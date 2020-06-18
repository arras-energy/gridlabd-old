[[/Converters/Import/Noaa_weather]] -- NOAA weather import

# Synopsis

GLM:

~~~
#input "<noaa-lcd-download.csv>" -o <filename>.glm -f noaa-weather -t weather -p location=<location>
~~~

Shell:

~~~
bash$ gridlabd -D csv_load_options="-o <filename>.glm -f noaa-weather -t weather  -p location=<location>" "<noaa-lcd-download.csv>" <modelname>.glm 
~~~

# Description

The NOAA weather converter allows you to use a downloaded copy of data from [NOAA LCD Datatools](https://www.ncdc.noaa.gov/cdo-web/datatools/lcd).

The converter creates two files using the weather data, a CSV file containing the actual data, and the GLM file specified by the `-o <filename>.glm` option.

The CSV file is formatted to meet the requirements of a standard `tape.player` object.  The GLM file contains a class declaration for the weather object, defined as follows:

~~~
class weather 
{
	char32 station_id;
	double temperature[degF];
	double humidity[%];
}	
~~~

In addition, the GLM file contains an object declaration for the weather data, with an embedded player to load the CSV data into the weather object. This is defined as follows:

~~~
object weather
{
	name "<filename>.glm";
	location "<location>";
	object player
	{
		file "<filename>.csv";
		property "temperature,humidity";
	};
}
~~~

To obtain data from NOAA, request a download from [NOAA LCD Datatools](https://www.ncdc.noaa.gov/cdo-web/datatools/lcd).

# Example

The following example illustrates loading weather data from a test dataset storage on AWS S3.

~~~
clock 
{
	starttime "2018-01-01 00:00:00";
	stoptime "2019-01-01 00:00:00";
}

#input "https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/test_data/noaa.csv" -o test_data.glm -f noaa-weather -t weather -p station_id=72594524283

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
