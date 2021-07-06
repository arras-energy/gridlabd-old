[[/Module/Residential/RBSA]] -- RBSA residential load model

# Synopsis

~~~
module residential {
  default_nominal_voltage "240 V"; 
  default_nominal_voltage_A "240+0d V";
  default_nominal_voltage_B "240+120d V";
  default_nominal_voltage_C "240-120d V";
  default_weekday_code "WEEKDAY";
  default_saturday_code "SATURDAY";
  default_sunday_code "SUNDAY";
  default_holiday_code "HOLIDAY";
  default_month_heading "Month";
  default_daytype_heading "Daytype";
  default_hour_heading "Hour";
  default_temperature_heating_balance "55.0 degF";
  default_temperature_cooling_balance "70.0 degF";
  default_temperature_heating_base "55.0 degF";
  default_temperature_cooling_base "70.0 degF";
  default_solargain_base "0.0 W/m^2";
  default_price_base "0.0 $/MWh";
}
~~~

# Description

The RBSA residential building load model is based on the residential building stock assessment energy use data collected by NEAA. The prepared data files are available from the [SLAC Gismo RBSA Data Repository](https://github.com/slacgismo/rbsa_data) in the `csv` folder.

The parent object of a building must be a `powerflow::meter` object.  In the absence of a suitable parent object, the building will use the global variables `default_nominal_voltage_A`, `default_nominal_voltage_B`, `default_nominal_voltage_C` and `default_nominal_voltage` to determine the voltage.  The solver will update the meter's power demand value when the load changes.

The RBSA data file is loaded when the `filename` is specified.  Each file is loaded only once and referenced by each building that uses it.  The loads in the data 

The load composition determines how much power is consumed by each end-use specified in the CEUS data file. The real and reactive fractions are given by the following terms

* `Zr`: the real constant impedance fraction

* `Ir`: the real constant current fraction

* `Pr`: the real constant power fraction

* `Zi`: the reactive constant impedance fraction

* `Ii`: the reactive constant current fraction

* `Pi`: the reactive constant power fraction

In general the sum the real power fractions should be 1.0 and the sum of the reactive power fractions should be less of 0.1.

The `Area` term of the fraction of the `floor_area` affected by the enduse specified.

The `weather` object provides the current temperature, if given.  In the absence of a weather reference, no temperature sensitivity is applied to the load. The power impact of temperature changes are computed as follows:

~~~
if ( temperature < temperature_heating_balance )
    power = ( temperature - temperature_heating_base ) * temperature_heating_sensitivity
else if ( temperature > temperature_cooling_balance )
    power = ( temperature - temperature_cooling_base ) * temperature_cooling_sensitivity
~~~

Similarly, the `weather` is used to obtain the solar irradiance sensitivity impact on power, which is computed as follows:

~~~
power = ( solargain - solargain_base ) * solargain_sensitivity
~~~

The `tariff` object provides the `price` values used to compute the load sensitivity to price. The power impact of price changes is computed as follows:

~~~
power = ( price - price_base ) * price_sensitivity
~~~

## Properties

### `filename`

~~~
method filename;
~~~

Specifies the RBSA data file to load.

### `floor_area`

~~~
double floor_area[sf];
~~~

Specifies the building floor area.

### `composition`

~~~
method composition;
~~~

Specifies an enduse load composition.  Use a separate entry for each enduse.

### `weather`

~~~
object weather;
~~~

Specifies the weather object to use for weather sensitivity.

### `total_power_A`

~~~
complex total_power_A[VA];
~~~

The resulting power on phase A.

### `total_power_B`

~~~
complex total_power_B[VA];
~~~

The resulting power on phase B.

### `total_power_C`

~~~
complex total_power_C[VA];
~~~

The resulting power on phase C.

### `total_real_power`

~~~
double total_real_power[W];
~~~

The resulting total real power.

### `total_reactive_power`

~~~
double total_reactive_power[VAR];
~~~

The resulting total reactive power.

### `temperature_heating_balance`

~~~
double temperature_heating_balance[degF];
~~~

Specifies the heating balance temperature.

### `temperature_cooling_balance`

~~~
double temperature_cooling_balance[degF];
~~~

Specifies the cooling balance temperature.

### `temperature_heating_base`

~~~
double temperature_heating_base[degF];
~~~

Specifies the heating base temperature.

### `temperature_cooling_base`

~~~
double temperature_cooling_base[degF];
~~~

Specifies the cooling base temperature.

### `temperature_heating_sensitivity`

~~~
  double temperature_heating_sensitivity[W/degF];
~~~

Specifies the heating temperature sensitivity.

### `solargain_base`

~~~
double solargain_base[W/m^2];
~~~

Specifies the base solar gain.

### `solargain_sensitivity`

~~~
double solargain_sensitivity[m^2];
~~~

Specifies the solar gain sensitivity.

### `tariff`

~~~
object tariff;
~~~

Specifies the tariff object.

### `price_base`

~~~
double price_base[$/MWh];
~~~

Specifies the base price.

### `price_sensitivity`

~~~
double price_sensitivity[W*MWh/$]; 
~~~

Specifies the price sensitivity.

# Example

The following implements a 3,000 sf single-family home in the Northwest valleys region with 90% constant impedance at unity power factor, and 10% constant power at power factor ~0.89:

~~~
object rbsa {
  filename "NWV_SFHOME.csv";
  floor_area 10 ksf;
  composition "Heating:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Cooling:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Ventilation:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Water_Heating:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Refrigeration:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Lighting:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Plugs:{ZR:0.9;PR:0.1;PI:0.01}";
  composition "Appliances:{ZR:0.9;PR:0.1;PI:0.01}";
}
~~~

# Caveats

The temperature, price, and solar sensitivity are not implemented by enduse yet.  The temperature, solar, and price sensitivities are applied at the whole building level only.

# See also

* [NEEA Residential Building Stock Assessment](https://neea.org/data/residential-building-stock-assessment)

* [SLAC GISMo RBSA Data Repository](https://github.com/slacgismo/rbsa_data)
