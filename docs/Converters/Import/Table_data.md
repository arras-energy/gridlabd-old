[[/Converters/Import/Table_data]] -- Table data import

# Synopis

This converter allows for any arbitrary object to be imported via CSV-formatted table. 

# Description

Any arbitrary objects can be listed in a CSV file and convertered to GLM-formatted object. The `class` can either be defined through `options` or specified directly in the CSV. The table format is as below : 

class | name | tilt_angle | tilt_direction | weather | configuration | equipment_area | equipment_height | install_year	| repair_time | latitude | longitude | phases | nominal_voltage | tmyfile
--- | --- | --- | --- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |---
pole | pole1 | 5 deg | 270	| weather | WOOD-C-45/5 | | | 1990 | 1 h | 37.4275 | 122.1697 | ABC | 12470
pole | pole2 | 8 deg | 270	| weather | WOOD-C-45/5 | | | 2000 | 8 h | 37.127 | 122.1646 | ABC | 12470
climate | weather | | 	|  |  | | |  |  |  |  |  | CA-Chino_Airport.tmy3

Sample usage within GLM
~~~
#input "input_file.csv" -f "table" -t "object" 
~~~

If you'd like to omit the class in the table you can specify it in the input command line instead as 

~~~
#input "input_file.csv" -f "table" -t "object" -C "pole"
~~~
The caveat with doing so is that all the objects specified in the CSV must match the class specified in the input command. The code will overwrite this specification if the class is specified in the file.

This will generate an "input_file.glm" and automatically include the objects within the model. 

# See also

* [[/Converters/Import/Csv_files]]
