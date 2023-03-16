[[/Converters/Import/Csv_files]] -- CSV file import converters

# Synopsis

~~~
#input "filename.csv" -t <type> [options]
~~~

To use in terminal: 
~~~~
gridlabd -D csv_load_options="-f <from> -t <type>" <file_name>.csv
~~~~

# Description

There are many different types of CSV files that can be imported. Each is documented separately according to the type of data it delivers to the GLM loader.

## External data

External data can be converted to either player files or calibrated GLM objects. Each converter has options that determine whether the resulting GLM model is a raw data played in using a [[/Module/Tape/Player]] object or calibrated physics-based models using the appropriate module, e.g., [[/Module/Residential/House]] object.

## GLM models

GLM models can have data defined in CSV file imports as GLM, either as a set of [[/GLM/Macro/Define]] statements (a configuration), a set of [[/GLM/Directive/Modify]] statements.  In addition, some templates have converters defined to prepare input data, e.g., integration capacity analysis.

## Weather data

Weather data can be obtained from many different sources.  The converters current support NOAA, Onpoint, and Visual Crossing weather data downloads.

# See also

* External data
  * [[/Converters/Import/Ami_data]]
  * [[/Converters/Import/Ceus_data]]
  * [[/Converters/Import/Rbsa_data]]
  * [[/Converters/Import/Scada_data]]
* GLM models
  * [[/Converters/Import/Config_data]]
  * [[/Converters/Import/Ica_data]]
  * [[/Converters/Import/Modify_data]]
  * [[/Converters/Import/Table_data]]  
* Weather data
  * [[/Converters/Import/Noaa_weather]]
  * [[/Converters/Import/Onpoint_weather]]
  * [[/Converters/Import/Visualcrossing_weather]]
