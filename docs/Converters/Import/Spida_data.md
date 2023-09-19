[[/Converters/Import/Spida_data]] -- SPIDACalc data import

# Synopis

GLM:
~~~
#convert SPIDACALC.xls GEODATA.csv -f xls-spida -t csv-geodata [precision=2] [extract_equipment=yes] [include_network=yes]
~~~

# Description

This converter extracts pole geodata from a SPIDAcalc pole asset report spreadsheet and generates a GriDLAB-D geodata CSV file.


# Options

- `precision=2`: specify the number of digits in a number and function ROUND will be used (default 2)

- `extract_equipment=yes`: enable the conversion of pole-mounted equipment, dummy values will be used for equipment properties (default None)

- `include_network=yes`: enable the generation of a bus-type feeder, dummy values will be used for properties of feeder and equipment (default None)

- `include_weather=NAME`: name the weather object and do not use dummy values for weather data (default None)

# Input 

[SPIDAcalc](https://www.spidasoftware.com/spidacalc/) is proprietarty pole loading and line design software that some utilities use to model the forces on transmission and distribution systems. The data includes detailed specifications and location data for poles, mounted equipment and lines.
	
The input .xls must include a sheet called 'Design - Pole'.
If `extract_equipment=yes`, then it must also include a sheet called 'Design - Structure'

This SPIDACalc API documentation explains data in the pole asset report spreadsheet:
	https://github.com/spidasoftware/schema/blob/66cb5d102dde5bf23b487dbddb261d3a88fc0907/doc/wfs.md 
	https://github.com/spidasoftware/schema/blob/master/doc/calc.md

For quick reference, some of the less obvious meanings are included below:
In 'Design - Pole':
* `AGL` means Above Ground Length
* `GLC` means GroundLine Circumference

In 'Design - Structure':
* `Direction` is a bearing for the mounted equipment, measured in degrees clockwise from North.
* A WEP is a Wire End Point, indicating the pole or building at the other end of a wire. Here `Offset/Lead` corresponds to the length of wire between the pole and this Wire End Point.
* For Insulators: "If on pole, then `offest` is attach height. If on cross arm, then `offset` is the distance from one end of the cross arm." - (SPIDAcalc docs)[https://github.com/spidasoftware/schema/blob/66cb5d102dde5bf23b487dbddb261d3a88fc0907/doc/wfs.md]
* For CrossArms: "Crossarm bearing is the direction the end of the crossarm points, not the direction of the bolt attachment." - (SPIDAcalc docs)[https://github.com/spidasoftware/schema/blob/66cb5d102dde5bf23b487dbddb261d3a88fc0907/doc/wfs.md]


# Output

A csv where each row represents a GridLAB-D object, columns are properties for that object. 
**See the documentation for the respective GridLAB-D objects to interpret the properties.**

The following objects can be included in the output: 
`pole`, `pole_configuration`
If `extract_equipment=yes`, it will also include `pole_mount`.


# See also

* [[/Converters/Import/Csv_files]]