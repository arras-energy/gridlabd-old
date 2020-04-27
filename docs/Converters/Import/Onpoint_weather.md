[[/Converters/Import/Onpoint_weather]] -- OnPoint weather import

# Synopsis

GLM:
~~~
#input "<aws-endpoint-csv>" -o <filename>.glm -f onpoint-weather -t weather -p country=<country> -p postal_code=<zipcode>
~~~

Shell:
~~~
bash$ gridlabd -D csv_load_options="-o <filename>.glm -f onpoint-weather -t weather  -p country=<country> -p postal_code=<zipcode>" "<aws-endpoint-csv>" <modelname>.glm 
~~~

# Description

The OnPoint weather converter allows you to download and cache a copy of data available from your [OnPoint Weather](https://us-west-1.console.aws.amazon.com/dataexchange/home?region=us-west-1#/products/prodview-cigja6adv533o) subscription on [AWS DataExchange](https://us-west-1.console.aws.amazon.com/dataexchange/home).

The converter creates two files using the weather data, a CSV file containing the actual data, and the GLM file specified by the `-o <filename>.glm` option.

The CSV file is formatted to meet the requirements of a standard `tape.player` object.  The GLM file contains a class declaration for the weather object, defined as follows:

~~~
class weather {
	double temperature[degF];
	double humidity[pu];
	double solar_total[W/m^2];
	double wind_speed[mph];
	double wind_direction[degF];
}	
~~~

In addition, the GLM file contains an object declaration for the weather data, with an embedded player to load the CSV data into the weather object. This is defined as follows:

~~~
object weather
{
	name "<filename>.glm";
	country "<country>";
	postal_code "<zipcode>";
	object player
	{
		file "<filename>.csv";
		property "temperature,humidity,solar_total,wind_speed,wind_direction";
	};
}
~~~

# Example

The following example downloads a sample weather data file provided by OnPoint Weather in the local simulation's working folder, and includes the result GLM file named `my-weather.glm` to load the weather data through a player into a weather object.  Then a recorder samples the weather object to collect the weather values.

~~~
clock 
{
	starttime "2018-01-01 00:00:00";
	stoptime "2019-01-01 00:00:00";
}

#input "https://s3-us-west-1.amazonaws.com/weather.gridlabd.us/test_data/onpoint.csv" -o my-weather.glm -f onpoint-weather -t weather -p country=US -p postal_code=36101

module tape;
object recorder
{
	parent "my-weather";
	file "weather_recorder.csv";
	property "temperature,humidity,solar_total,wind_speed,wind_direction";
}
~~~

# See also

* [[/Command/Automatic_import_conversion]]
* [OnPoint Weather](https://weathersource.com/products/onpoint-weather/)
