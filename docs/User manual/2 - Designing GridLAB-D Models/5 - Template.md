# Templates

Templates are special purpose GLM models that perform specific analysis tasks on other models.  For example, a residential template can be loaded after a network model to automatically attach houses to a network.  

The general syntax for using a template is

~~~
% gridlabd MODEL.glm TEMPLATE.glm
~~~

Note that the template is always loaded *after* the model to which it is applied.  We will see why in a moment.  

Public templates can be downloaded from the GridLAB-D template repository on GitHub.  The [[/Subcommand/Template]] is used to download a template from the GitHub repository.  Private template can also be created and install on systems.  The minimum requirements for creating a template are the creation of a GLM file. Many template also have a corresponding Python file, and possible other ancillary files. 

# Examples

For these examples, we will use the following network model, which includes 5 single-phase load busses.

File `network.glm`:

~~~
module powerflow;
#for PHASE in A B C
object triplex_load:..5
{
  nominal_voltage 120V;
  phases ${PHASE}S;
}
#done
~~~

## Clock template

A template can be used to set the simulation clock.

File `clock_template.glm`:

~~~
clock 
{
  timezone "PST+8PDT";
  starttime "2021-01-01 00:00:00 PST";
  stoptime "2022-01-01 00:00:00 PST";
}
~~~

## Weather template

A template can be used for loading weather data.

File `weather_template.glm`:

~~~
#weather get "CA-SanFrancisco_Intl_Ap.tmy3"
#input "CA-SanFrancisco_Intl_Ap.tmy3"
~~~

## Object template

A template can be used to automatically attach single-phase meters and houses to load objects in the network model.  

File `house_template.glm`:

~~~
module residential
#for LOAD in ${FIND class=triplex_load}
object triplex_meter
{
  parent ${LOAD};
  nominal_voltage @${LOAD};
  phases @${LOAD};
  object house
  {
  };
}
#done
~~~

The `#for LOAD in ${FIND class=triplex_load}` searches the model for all the `triplex_meter` object and loops through them assigning the objects' names sequentially to the `LOAD` variable. The `@` prefix in the `triplex_meter` object `nominal_voltage` and `phases` properties copy the same properties of the `triplex_load` object.

## Sensitivity template

A template can be used to perform sensitivity analysis as follows:

File `sensitivity_template.glm`:

~~~
clock
{
  timezone PST+8PDT;
  starttime "2021-01-01 00:00:00 PST";
  stoptime "2022-01-01 00:00:00 PST";
}
module tape
{
  csv_header_type NAME;
}
module residential;
#for LOAD in ${FIND class=triplex_load}
object triplex_meter
{
  parent ${LOAD};
  nominal_voltage @${LOAD};
  phases @${LOAD};
  measured_energy_delta_timestep 3600;
  object house
  {
#ifdef PASS
    thermal_integrity_level NORMAL;
#else
    thermal_integrity_level VERY_GOOD;
#endif
  };
  object recorder
  {
    file "${LOAD}_${PASS:-base}.csv";
    interval 3600;
    property measured_real_energy_delta;
  };
}
#done
#ifndef PASS
#on_exit 0 eval ${command_line/.bin/ -D PASS=test}
#else
#on_exit 0 /usr/local/bin/python3 ./${filename/.glm/.py}
#endif
~~~

File `sensitivity_template.py`:

~~~
import pandas
total = 0
for num in range(15):
  base = pandas.read_csv(f"triplex_load:{num}_base.csv")
  test = pandas.read_csv(f"triplex_load:{num}_test.csv")
  total += (test['measured_real_energy_delta']-base['measured_real_energy_delta']).sum().round(-3)/1000

print(f"Change: {total} kWh/y")
~~~

The `tape` module option `csv_header_type NAME` formats the CSV files with name headers that are compatible with `pandas.read_csv()`.  The `PASS` variable is used to identify whether the simulation is running a base case or a test case.  The `#on_exit` macro is used to run post processing.  In the base (when `PASS` is not defined), the command runs the test case.  In the second case, the command run the template's python script to load and compare the CSV files and compute the sensitivity.

## Configuring templates

Often templates use variables to allow users to control the behavior of the template.  These variable can be set using a configuration file in GLM or CSV format.  For example, the clock template can use a GLM configuration file as follows:

File `config.glm`:

~~~
#define TIMEZONE="PST+8PDT"
#define STARTTIME="2021-01-01 00:00:00 PST"
#define STOPTIME="2022-01-01 00:00:00 PST"
~~~

File `clock_template.glm`:

~~~
#include "config.glm"
clock 
{
  timezone ${TIMEZONE};
  starttime ${STARTTIME};
  stoptime ${STOPTIME};
}
~~~

Alternatively, a CSV configuration can be used:

File `config.csv`:

~~~
TIMEZONE,"PST+8PDT"
STARTTIME,"2021-01-01 00:00:00 PST"
STOPTIME,"2022-01-01 00:00:00 PST"
~~~

File `clock_template.glm`:

~~~
#input "config.csv" -f config -t config
clock 
{
  timezone ${TIMEZONE};
  starttime ${STARTTIME};
  stoptime ${STOPTIME};
}
~~~

