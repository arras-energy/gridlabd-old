"""GridLAB-D Geodata Powerline Package

The powerline package computes geographic information about powerlines, such as
line sag, line sway, and line gallop.

All powerline package calculations require the following data:

    latitude - required column in the data

        The latitude is specified as a float with positive north and negative
        south orientation.

    longitude - required column in the data

        The longitude is specified as a float with position east and negative
        west orientation.

    pole_height - required column in the data

        The pole height is specified in meters.

    cable_type - required either in the data or provided in the options

        The cable types are listed in the file geodata_powerline_cabletypes.csv
        located in the GridLAB-D shared geodata folder, which is by default
        /usr/local/share/gridlabd/geodata.

    distance - optional column in the data, if absent it will be computed

        The distance between from the first pole in the data series.

    heading - optional column in the data, if absent it will computed

        The heading from the last pole encountered in the data series.

In addition, the following optional values are supported:

    elevation - optional column in the data, by default 0.0

    powerflow - optional column in the data, by default 0.0, may be provided in
        the options

    wind_speed - optional column in the data, by default 0.0, may be provided in
        the options

    wind_direction - optional column in the data, by default 0.0, may be
        provided in the options

    air_temperature - optional column in the data, by default 30 degC, may be
        provided in the options

    global_horizontal_irradiance - optional column in the data, by default
        1000.0 W/m^2, may be provided in the options

    ground_reflectance - optional column in the data, by default 0.3 pu, may be
        provided in the options

    ice_density - optional column in the data, by default 915.0 kg/m^3, may be
        provided in the options

    ice_thickness - optional column in the data, by default 0.0, may be
        provided in the options

The follows options may also be set:

    nominal_temperature - the temperature at which line loads are based, by
        default 15.0 degC.

LINE SAG

Line sag is the drop in line elevation above ground resulting from a
combination of the line weight, line elasticity, and line temperature. Note
that line sag may be affected by line sway (see below) insofar as line elevation
about ground is reduced as line sway is increased.

If the line sway is non-zero, the line sag will be computed according to the
formula

  linesag(linesway)^2 = linesag(0)^2 - linesway^2

LINE SWAY

Line sway is the lateral displacement of the line as a result of lateral wind
forces on the line.

LINE GALLOP

Line gallop is a vertical multi-nodal oscillation of a line resulting from the
asymetric build up of ice on the line.

"""

version = 1

import os, sys
import requests
from math import cos, sin, atan2, pi, sqrt, atan
from scipy.interpolate import interp1d
import numpy as np
from PIL import Image
import pandas
import json

def TODO(value=float('nan')):
    """TODO default function -- this should never be called in the final product"""
    from inspect import getframeinfo, stack
    caller = getframeinfo(stack()[1][0])
    print(f"TODO({value.__repr__()}): called from {caller.function}()@{caller.filename}:{caller.lineno}")
    return value

default_options = {
    "air_temperature" : 30.0, # degC
    "wind_speed" : 0.0, # m/s
    "wind_direction" : 0.0, # direction
    "ice_thickness" : 0.0, # m
    "power_flow" : 0.0, # W
    "global_horizontal_irradiance" : 1000.0, # W/m^2
    "ground_reflectance" : 0.3, # pu
    "ice_density" : 915.0, # kg/m^3
    "nominal_temperature" : 15.0, # degC - temperature at which line loads are based
    "cable_type" : '', # cable type
}

default_config = {
    "cabletype_file" : "/usr/local/share/gridlabd/geodata_powerline_cabletypes.csv",
}

OPTIONS = default_options
CONFIG = default_config
CABLETYPES = pandas.DataFrame()

# physical constants
g = 9.81 # Gravity of Earth, unit: m/s2
k_s = 5.6704e-8 # Stefan/Boltzmann constant

def WARNING(msg):
    """Default warning message handler"""
    raise Exception(msg)

def get_air_properties(temp):
    """Compute material properties at a given Temperature

    ARGUMENTS

        temp (float)   Air temperature at which properties are calculated

    RETURNS

        float   Specified mass of air, in kg/m^3

        float   Thermal conductivity of air, in W/m.degC

        float   Dynamic viscosity of air, N.s/m^2
    """
    # calculate the constant parameters
    # reference: http://home.eng.iastate.edu/~jdm/wind/TransmissionLineLoadingDesignCriteriaAndHTS.pdf
    temp_x = np.arange(0.0, 110.0, 10) # unit: DegC
    specific_mass = [1.29, 1.25, 1.20, 1.17, 1.13, 1.09, 1.06, 1.03, 1.0, 0.97, 0.95] # unit: kg/m3
    thermal_conductivity = [0.0243, 0.025, 0.0257, 0.0265, 0.0272, 0.028, 0.0287,0.0294, 0.0301, 0.0309, 0.0316] # unit: W/(m*DegC)
    dynamic_viscosity = [0.175e-4, 0.18e-4, 0.184e-4, 0.189e-4, 0.194e-4, 0.199e-4, 0.203e-4, 0.208e-4, 0.213e-4, 0.217e-4, 0.222e-4] # unit: N*s/m2

    f_specific_mass = interp1d(temp_x, specific_mass, fill_value = "extrapolate")
    specific_mass_temp = f_specific_mass(temp)

    f_thermal_conductivity = interp1d(temp_x, thermal_conductivity, fill_value = "extrapolate")
    thermal_conductivity_temp = f_thermal_conductivity(temp)

    f_dynamic_viscosity = interp1d(temp_x, dynamic_viscosity, fill_value = "extrapolate")
    dynamic_viscosity_temp = f_dynamic_viscosity(temp)

    return specific_mass_temp, thermal_conductivity_temp, dynamic_viscosity_temp

hold_values = {}
def hold0(name,value=None):
    """Perform zero-order hold of value

    Returns the previous value of the named variable if curretn value is None.
    Otherwise return the current value.

    ARGUMENTS

        name (str)      variable name

        value (object)  hold value (None for hold value), or 'init' to use
                        set to initial value of OPTIONS

    RETURNS

        object          current value or hold value
    """
    global hold_values
    if value == 'init':
        hold_values[name] = OPTIONS[name]
    elif value != None:
        hold_values[name] = value
        return value
    elif not name in hold_values.keys():
        WARNING(f"unable to get initial value of {name}")
        return None
    else:
        return hold_values[name]

def get_line_tension_coefficient(d_hori):
    """Compute line tension coefficient for the line length

    ARGUMENTS

        d_hori (float or series)  The horizontal distance between the line ends

    RETURNS

        float or series   The line tension coefficient
    """
    # reference: IEC 60826:2017
    # fit of data is used
    return 0.2106718346 + 0.0003126614987*d_hori

def get_distance(pos1, pos2):
    """Compute haversine distance between two locations

    ARGUMENTS

        pos1, pos2 (float tuple)   Specifies the two geographic endpoints as a
                                   (latitude,longtitude) tuple
    """
    lat1 = pos1[0]*pi/180
    lat2 = pos2[0]*pi/180
    lon1 = pos1[1]*pi/180
    lon2 = pos2[1]*pi/180
    a = sin((lat2-lat1)/2)**2+cos(lat1)*cos(lat2)*sin((lon2-lon1)/2)**2
    return 6371e3*(2*atan2(np.sqrt(a),sqrt(1-a)))

def linesag(data):
    """Linesag calculations

    ARGUMENTS

        data (DataFrame)   Data at positions along powerline path where sag
                           must be computed.  Required columns are as follows:

            cable_type      Specifies the cable type as listed in
                           $GLD_ETC/geodata_powerline_cabletypes.csv. If NA,
                           then the last cable_type is used.  The first cable_type
                           must be specified.

            air_temperature    Outdoor air temperature in degC. If missing, sag will
                           be computed for 30 degC.

            wind_speed     Wind speed in m/s.  If missing, sag will be computed
                           without considering effect of wind speed

            wind_direction Wind direction in degree. If missing, sag will be
                           computed without consider effect of wind direction

            ice_thickness  Ice thickness on powerline. If missing, sag will be
                           computed without considering effect of ice loading.

            distance       Line run distance is calculated if missing.

            heading        Line run heading is calculated if missing.

    RETURNS

        DataFrame   The data input dataframe is returned with the "linesag"
                    column added.  If the line sag cannot be computed at a given
                    location, a "nan" value will be stored. If any "nan" values
                    are encountered, a warning will be generating explaining the
                    cause of the "nan" value(s).
    """

    data['linesag'] = float('nan') # default result

    # read cable specs from cable type
    global OPTIONS
    if not 'cable_type' in data.columns and not 'cable_type' in OPTIONS.keys():
        WARNING("cannot compute line sag without any cable type")
        return data['linesag']

    # read optional initial hold values
    for name,value in OPTIONS.items():
        if not name in data.columns:
            hold0(name,'init')
    air_temperature = hold0('air_temperature')
    wind_speed = hold0('wind_speed')
    wind_direction = hold0('wind_direction')
    ice_thickness = hold0('ice_thickness')
    power_flow = hold0('power_flow')
    global_horizontal_irradiance = hold0('global_horizontal_irradiance')
    ground_reflectance = hold0('ground_reflectance')
    ice_density = hold0('ice_density')
    cable_type = hold0('cable_type')

    # check lat,lon
    if not 'latitude' in data.columns or not 'longitude' in data.columns:
        WARNING("cannot compute line sag without latitude and longitude fields")
        return data

    # TODO: vectorize this loop
    p1 = None
    z1 = []
    line_data = []
    line_sags = []
    for id,line in data.iterrows():
        if 'cable_type' in data.columns:
            cable_type = hold0('cable_type',line['cable_type'])
        global CABLETYPES
        if not cable_type:
            WARNING(f"cable type not specified")
            return data['linesag']
        elif not cable_type in CABLETYPES.index:
            WARNING(f"cable type '{cable_type}' not found")
            return data['linesag']
        else:
            cable = CABLETYPES.loc[cable_type]

        # air Temperature
        if 'air_temperature' in data.columns:
            air_temperature = hold0('air_temperature',line['air_temperature'])

        # wind speed
        if 'wind_speed' in data.columns:
            wind_speed = hold0('wind_speed',line['wind_speed'])

        # wind direction
        if 'wind_direction' in data.columns:
            wind_direction = hold0('wind_direction',line['wind_direction'])

        # ice thickness
        if 'ice_thickness' in data.columns:
            ice_thickness = hold0('ice_thickness',line['ice_thickness'])

        # power flow
        if 'power_flow' in data.columns:
            power_flow = hold0('power_flow',line['power_flow'])

        # ground reflectance
        if 'ground_reflectance' in data.columns:
            ground_reflectance = hold0('ground_reflectance',line['ground_reflectance'])

        # elevation
        if 'elevation' in data.columns and 'pole_height' in data.columns:
            z0 = line['elevation'] + line['pole_height']
        else:
            z0 = 0.0

        # distance
        p0 = [line['latitude'],line['longitude']]
        if 'distance' in data.columns:
            d_hori = line['distance']
        elif p1 != None:
            d_hori = get_distance(p0,p1)
            # TODO: back calculate intermediate values
            for zz, ln in zip(z1,line_data):
                ln['linesag'] = get_sag_value(d_hori,line,cable,p0,p1,z0,zz,
                    power_flow,global_horizontal_irradiance,ground_reflectance,
                    ice_thickness,wind_direction,air_temperature,wind_speed,ice_density)
            line_data = []
            z1 = []
        else:
            p1.append(p0)
            z1.append(z0)
            line_data.append(line)
            continue



    return data['linesag']

def get_sag_value(d_hori,line,cable,p0,p1,z0,z1,
        power_flow,global_horizontal_irradiance,ground_reflectance,
        ice_thickness,wind_direction,air_temperature,wind_speed,ice_density):
    """Calculate line sag values"""
    global OPTIONS
    d_vert = abs(z0-z1)
    k_init = get_line_tension_coefficient(d_hori)
    span = sqrt(d_hori*d_hori + d_vert*d_vert)
    rts = cable['rated_tensile_strength']
    unit_weight = cable['unit_weight']
    if d_vert > 30:
        k_init = min(k_init, unit_weight*span*span/(2*d_vert*rts))
    H_init = rts*k_init
    sag_init = unit_weight*span*span/(8*H_init)
    P_rated = power_flow
    Vll_rated = cable['voltage_rating']
    Irms =  P_rated/(sqrt(3)*Vll_rated)
    R_20C = cable['nominal_resistance']
    coeff_Al = cable['resistivity']
    Q_I_coeff_first = Irms*Irms*R_20C*coeff_Al
    Q_I_coeff_constant = Irms*Irms*R_20C*(1-coeff_Al*(20.0+273.0))
    k_a = 1.0 - cable['reflectivity'] # solar radiation absorption coefficient
    GHI = global_horizontal_irradiance # unit: W/m2
    k_g = ground_reflectance # ground reflect
    diameter = cable['diameter']
    Q_S_constant = k_a*(diameter+2*ice_thickness)*(1+k_g)*GHI

    if not 'heading' in data.columns:
        line_angle = 180*atan2(p1[0]-p0[0],p1[1]-p0[1])/np.pi
    else:
        line_angle = line['heading']
    phi = (wind_direction - line_angle)*np.pi/180
    k_angle = 1.194 - cos(phi) + 0.194*cos(2*phi) + 0.368*sin(2*phi)
    air_mass, k_f, air_viscosity = get_air_properties(air_temperature)
    Nre = wind_speed * (diameter + 2*ice_thickness) * air_mass / air_viscosity
    if wind_speed > 1.1:
        Q_C_coeff_first = k_angle*(1.01+1.35*Nre**0.52)*k_f
        Q_C_constant = -k_angle*(1.01+1.35*Nre**0.52)*k_f*(air_temperature+273.0)
    else:
        Q_C_coeff_first = k_angle*0.754*(Nre**0.6)*k_f
        Q_C_constant = -k_angle*0.754*(Nre**0.6)*k_f*(air_temperature+273.0)
    # for Q_R
    k_e = cable['emissivity']
    Q_R_constant = -5.6704e-8*k_e*(diameter+2.0*ice_thickness)*(air_temperature+273.0)**4
    Q_R_coeff_fourth = 5.6704e-8*k_e*(diameter+2.0*ice_thickness)
    # for new conductor temp under loading
    coef_sag = [-Q_R_coeff_fourth,0.0,0.0,Q_I_coeff_first-Q_C_coeff_first,Q_I_coeff_constant+Q_S_constant-Q_C_constant-Q_R_constant]
    r = np.roots(coef_sag)
    r = r[~np.iscomplex(r)]
    temp_load = np.absolute(r[r > 0.0]) # unit: K
    temp_load = temp_load - 273.0 # unit: DegC
    # calculate the new line sag at loaded condition
    ice_unit_weight = ice_density*np.pi*ice_thickness*(diameter+ice_thickness)*g
    wind_unit_weight = 0.5*air_mass*(wind_speed*sin(phi))**2 *(diameter+2*ice_thickness)
    total_unit_weight = sqrt(wind_unit_weight**2+(unit_weight+ice_unit_weight)**2)

    area = cable['conductor_crosssection_area']
    elasticity = cable['elasticity']
    temp_init = OPTIONS['nominal_temperature']
    coef_thermal = cable['thermal_expansion']
    H_load_second = (unit_weight*d_hori)**2 *area*elasticity/(24*H_init**2)-H_init+(temp_load-temp_init)*coef_thermal*area*elasticity
    H_load_constant = -(total_unit_weight*d_hori)**2 *area*elasticity/24
    coef_H = [1, H_load_second, 0.0, H_load_constant]
    r = np.roots(coef_H)
    r = r[~np.iscomplex(r)]
    H_load = np.absolute(r[r > 0.0])
    #
    sag_load = total_unit_weight*span*span/(8*H_load)
    sag_angle = atan(wind_unit_weight/(ice_unit_weight+unit_weight))
    if z0 > z1:
        d0_hori = d_hori*(1+d_vert/(4*sag_load))/2
        d1_hori = d_hori - d0_hori
        sag1 = total_unit_weight*d1_hori**2 /(2*H_load)
        sag_elevation = z1-sag1*cos(sag_angle)
        dt = get_distance(p0,[p0[0]+d0_hori*(p1[0]-p0[0])/d_hori,p0[1]+d0_hori*(p1[1]-p0[1])/d_hori])
    else:
        d0_hori = d_hori*(1-d_vert/(4*sag_load))/2
        d1_hori = d_hori - d0_hori
        sag0 = total_unit_weight*d0_hori**2 /(2*H_load)
        sag_elevation = z0-sag0*cos(sag_angle)
        dt = get_distance(p0,[p0[1]+d0_hori*(p1[0]-p0[0])/d_hori,p0[1]+d0_hori*(p1[1]-p0[1])/d_hori])
    return sag_elevation

def linesway(data):
    """TODO"""
    data['linesway'] = float('nan') # default result

    if not 'wind_speed' in data.columns:
        data['linesway'] = float('nan')
        return data['linesway']

    WARNING("linesway not implemented yet")
    data['linesway'] = TODO() # default result

    return data['linesway']

def linegallop(data):
    """TODO"""
    #
    # Galloping due to icing effect
    #
    # Frequency calculation
    #
    #   F (Hz) = sqrt(T*g/W) * 0.5*n/L
    #
    # where:
    #
    #   T = conductor tension (N)
    #   g = 9.81 m/s^2 (gravitational acceleration)
    #   W = conductor weight (N/m)
    #   L = conductor length
    #   n = standing wave node count (>=1)
    #
    # See https://preformed.com/images/pdfs/Energy/Transmission/Motion_Control/Air_Flow_Spoiler/Conductor_Galloping_Basics-EN-ML-1166.pdf
    #
    # Magnitude calculation
    #
    # Mitigation strategies
    #
    #  1) Detuning pendulum
    #  2) Airflow spoiler
    #
    data['linegallop'] = float('nan') # default result

    if not 'ica_thickness' in data.columns:
        data['linegallop'] = float('nan')
        return data['linegallop']

    WARNING("linegallop not implemented yet")
    data['linegallop'] = TODO() # default result

    return data['linegallop']

def apply(data, options=default_options, config=default_config, warning=print):

    global CABLETYPES
    CABLETYPES = pandas.read_csv(config['cabletype_file'],index_col='id')

    global WARNING
    WARNING = warning

    global OPTIONS
    OPTIONS = options

    global CONFIG
    CONFIG = config

    result = pandas.DataFrame(data)
    result["linesag"] = linesag(data)
    result["linesway"] = linesway(data)
    result["linegallop"] = linegallop(data)

    return result

# perform validation tests
if __name__ == '__main__':

    import unittest

    CABLETYPES = pandas.read_csv(sys.argv[0].replace('geodata_powerline.py','geodata_powerline_cabletypes.csv'),index_col='id')

    data = pandas.DataFrame({
        'latitude' : [37.41504514,37.41469802,],
        'longitude' : [-122.2056472,-122.2084875,],
        'pole_configuration' : ['flat 3','side T'],
        'horizontal_spacing' : [20.0,18.0,],
        'pole_height' : [18,20],
        'elevation' : [88.45,99.125],
        'cable_type' : ['TACSR/AC 610mm^2',None],
    })

    class TestPowerline(unittest.TestCase):

        def test_linesag(self):
            result = linesag(pandas.DataFrame(data))
            self.assertEqual(result['linesag'].to_list(),[0,0])

        def test_linesway(self):
            result = linesway(pandas.DataFrame(data))
            self.assertEqual(result['linesway'].to_list(),[0,0])

        def test_linegallop(self):
            result = linegallop(pandas.DataFrame(data))
            self.assertEqual(result['linegallop'].to_list(),[0,0])

    unittest.main()
