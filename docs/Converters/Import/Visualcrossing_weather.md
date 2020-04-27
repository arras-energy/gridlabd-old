[[/Converters/Import/Visualcrossing_weather]] -- Visual Crossing weather import

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

The Visual Crossing weather converter allows you to use a downloaded copy of data from [Visual Crossing](https://www.visualcrossing.com).

The converter creates two files using the weather data, a CSV file containing the actual data, and the GLM file specified by the `-o <filename>.glm` option.

The CSV file is formatted to meet the requirements of a standard `tape.player` object.  The GLM file contains a class declaration for the weather object, defined as follows:

~~~
class weather {
	double temperature[degF];
	double humidity[pu];
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

# Example

The following example demonstrates loading the Visual Crossing test dataset stored on AWS.

~~~
clock 
{
	starttime "2020-01-01 00:00:00";
	stoptime "2020-01-05 00:00:00";
}

#input "https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/test_data/visualcrossing.csv" -o test_data.glm -f visualcrossing-weather -t weather -p location="Menlo Park, CA"

module tape;
object recorder
{
	parent "test_data";
	file "weather_recorder.csv";
	property "temperature,humidity,wind_speed,opq_sky_cov";
}
~~~

# See also

* [[/Command/Automatic_import_conversion]]
* [Visual Crossing Website](https://www.visualcrossing.com)
