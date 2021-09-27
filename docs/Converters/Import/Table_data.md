[[/Converters/Import/Table_data]] -- Table data import

# Synopis

This converter allows for any arbitrary object to be imported via CSV-formatted table. 

# Description

Any arbitrary objects can be listed in a CSV file and convertered to GLM-formatted object. The table format is as below : 

class | name | tilt_angle | tilt_direction | weather | configuration | equipment_area | equipment_height | install_year	| repair_time | latitude | longitude | phases | nominal_voltage | tmyfile
--- | --- | --- | --- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |---
powerflow.pole | pole1 | 5 deg | 270	| weather | WOOD-C-45/5 | | | 1990 | 1 h | 37.4275 | 122.1697 | ABC | 12470
powerflow.pole | pole2 | 8 deg | 270	| weather | WOOD-C-45/5 | | | 2000 | 8 h | 37.127 | 122.1646 | ABC | 12470
climate | weather | | 	|  |  | | |  |  |  |  |  | | CA-Chino_Airport.tmy3

Sample usage within GLM
~~~
#input "input_file.csv" -f "table" -t "object" 
~~~

The `class` can either be defined through `options` or specified directly in the CSV. If you'd like to omit the class in the table you can specify it in the input command line instead as 

~~~
#input "input_file.csv" -f "table" -t "object" -C "powerflow.pole"
~~~
The function will overwrite the `class` specification if the class is already specified in the file. 

The object names can be omitted and the function will autopopulate the object in the format as `<class_name>_<row_number>`

This will generate an "input_file.glm" and automatically include the objects within the model. 

Note, for the module definition to be included in the converter output, the class definition has to contain the module name before the class as 
~~~
powerflow.pole
~~~


# See also

* [[/Converters/Import/Csv_files]]
