[[CEUS]] -- Commercial End-Use Survey data-driven building model
# Synopsis
~~~
  module commercial {
    default_nominal_voltage 240;
    default_nominal_voltage_A 240+0d;
    default_nominal_voltage_B 240+120d;
    default_nominal_voltage_C 240-120d;
    default_weekday_code "WEEKDAY";
    default_saturday_code "SATURDAY";
    default_sunday_code "SUNDAY";
    default_holiday_code "HOLIDAY";
    default_month_heading "Month";
    default_daytype_heading "Daytype";
    default_hour_heading "Hour";
    default_temperature_heating_balance 55.0 degF;
    default_temperature_cooling_balance 70.0 degF;
    default_temperature_heating_base 55.0 degF;
    default_temperature_cooling_base 70.0 degF;
    default_solargain_base 0.0 W/m^2;
    default_price_base 0.0 $/MWh;
  }
  class ceus {
    method filename;
    double floor_area[sf];
    method composition;
    object weather;
    complex total_power_A[VA];
    complex total_power_B[VA];
    complex total_power_C[VA];
    double total_real_power[W];
    double total_reactive_power[VAR];
    object weather;
    double temperature_heating_balance[degF];
    double temperature_cooling_balance[degF];
    double temperature_heating_base[degF];
    double temperature_cooling_base[degF];
    double temperature_heating_sensitivity[W/degF];
    double solargain_base[W/m^2];
    double solargain_sensitivity[m^2];
    object tariff;
    double price_base[$/MWh];
    double price_sensitivity[W*MWh/$]; 
  }
~~~

# Description

The CEUS commercial building load model is based on the commercial energy use data collected by Itron for the California Energy Commission. The prepared data files are available from the GitHub repository https://github.com/slacgismo/ceus_data in the `csv` folder.

The parent object of a building must be a `powerflow meter` object.  In the absence of a suitable parent object, the building will use the global variables `default_nominal_voltage_A`, `default_nominal_voltage_B`, `default_nominal_voltage_C` and `default_nominal_voltage` to determine the voltage.  The solver will update the meter's power demand value when the load changes.

The CEUS data file is loaded when the `filename` is specified.  Each file is loaded only once and referenced by each building that uses it.  The loads in the data 

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

# Example

The following implements a 10,000 sf office building in California forecasting climate zone 1 with 90% constant impedance at unity power factor, and 10% constant power at power factor ~0.89:
~~~
  object ceus {
    filename "FCZ01_SOFF.csv";
    floor_area 10 ksf;
    composition "Heating:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Cooling:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Ventilation:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Water_Heating:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Refrigeration:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Exterior_Lighting:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Interior_Lighting:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Office_Equipment:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Miscellaneous:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Process:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Motors:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Air_Compressors:{ZR:0.9;PR:0.1;PI:0.01}";
  }
~~~

# Caveats

The temperature, price, and solar sensitivity are not implemented by enduse.  These sensitivities are applied at the whole building level only.

# See also 
* [[Module/Commercial]]
* [California Commercial End-use Survey (CEUS) Website](https://www.energy.ca.gov/ceus)
* [SLAC GISMo CEUS Data Repository](https://github.com/slacgismo/ceus_data)
