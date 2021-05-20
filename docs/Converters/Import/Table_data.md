[[/Converters/Import/Table_data]] -- Table data import

# Synopis

This converter allows for any arbitrary object to be imported via CSV-formatted table. 

# Description

Any arbitrary objects can be listed in a CSV file and convertered to GLM-formatted object. The `class` can either be defined through `options` or specified directly in the CSV. The table format is as below : 


class | #1 | #2 | #3 | #4 | #5 | #6 | #7 | #8 | #9 | #10 | #11
--- | --- | --- | --- |--- |--- |--- |--- |--- |--- |--- |---
Seconds | 301 | 283 | 290 | 286 | 289 | 285 | 287 | 287 | 272 | 276 | 269


class | name | tilt_angle | tilt_direction | weather | configuration | equipment_area | equipment_height | install_year	| repair_time | latitude | longitude | phases | nominal_voltage | tmyfile
--- | --- | --- | --- |--- |--- |--- |--- |--- |--- |--- |--- |--- |---
pole | pole1 | 5 deg | 270	| weather | WOOD-C-45/5 | | | 1990 | 1 h | 37.4275 | 122.1697 | ABC | 12470
pole | pole2 | 8 deg | 270	| weather | WOOD-C-45/5 | | | 2000 | 8 h | 37.127 | 122.1646 | ABC | 12470
climate | weather | | 	|  |  | | |  |  |  |  |  | CA-Chino_Airport.tmy3

Sample usage within GLM
~~~
#input "input_file.csv" -f "table" -t "object" 
~~~

This will generate an "input_file.glm" and automatically include the objects within the model. 

# See also

* [[/Converters/Import/Csv_files]]
