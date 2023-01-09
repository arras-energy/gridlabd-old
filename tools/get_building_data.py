"""Get IDF defaults

The scripts parses IDF files to compute the defaults for the powerflow building model
"""

SAMPLE_RATE=1000

import sys, os
import json
import getopt
import datetime
import re
import pandas
import numpy

if len(sys.argv) == 1:
    # TODO: remove this developer-only feature
    if os.path.basename(os.getcwd()) == "tools":
        sys.path.append("../converters")
        sys.argv.append("../../../globus/CA_San_Mateo")
    else:
        print("Syntax: get_idf_defaults [DIRNAME ...]")
        sys.argv.append(".")

from geometry import Polygon
from geohash import geohash
from idfparse import IDF

def vertices(data):
    pts = []
    for key,value in data.items():
        if key.startswith('X,Y,Z Vertex'):
            pts.append(value)
    return Polygon(pts)

building_data = {"building_type":[],"floor_area":[],"UA":[],"CM":[],"CA":[],"UM":[],"UI":[]}
for dirname in sys.argv[1:]:
    print("Reading folder",dirname,"...",file=sys.stderr,flush=True)
    try:
        with open(os.path.join(dirname,"index.csv")) as fh:
            index = fh.readlines()
    except:
        index = os.listdir(dirname)
    skip = SAMPLE_RATE
    total = len(index)
    print("Total records:",total,file=sys.stderr)
    prog = 0
    for file in index:
        if skip == 0:
            skip = SAMPLE_RATE
        else:
            skip -= 1
            continue
        prog += SAMPLE_RATE
        
        print("Processing",file,f"({prog*100.0/total:.1f}%)","...",file=sys.stderr,flush=True)
        if os.path.splitext(file)[1] == ".idf":
            pathname = os.path.join(dirname,file)
            idf = IDF(pathname)

            # clean the name
            name = re.sub('[^-0-9A-Za-z_]','',list(idf['Building'].keys())[0].replace(' ','_'));
            name = re.sub('_+','_',name).strip('_')
            if name[0] == '-':
                name = name[1:63]

            building_type = name.split('-')[0]

            location = list(idf['Site:Location'].keys())[0]
            latitude = idf['Site:Location'][location]['Latitude {deg}']
            longitude = idf['Site:Location'][location]['Longitude {deg}']
            occupancy = sum([data['People per Zone Floor Area {person/m2}'] for data in idf['People'].values()])
            lighting = sum([data['Watts per Zone Floor Area {W/m2}'] for data in idf['Lights'].values()])
            equipment = sum([data['Watts per Zone Floor Area {W/m2}'] for data in idf['ElectricEquipment'].values()])
            
            # basic building properties
            floor_area = sum([vertices(data).area() for data in idf['BuildingSurface:Detailed'].values() if data['Surface Type'] == 'Floor'])
            wall_perimeter = sum([vertices(data).perimeter() for data in idf['BuildingSurface:Detailed'].values() if data['Surface Type'] == 'Floor'])
            wall_height = max([vertices(data).height() for data in idf['BuildingSurface:Detailed'].values() if data['Surface Type'] == 'Wall'])
            wall_area = sum([vertices(data).area() for data in idf['BuildingSurface:Detailed'].values() if data['Surface Type'] == 'Wall'])
            window_area = sum([vertices(data).area() for data in idf['FenestrationSurface:Detailed'].values() if data['Surface Type'] == 'Window'])
            roof_area = sum([vertices(data).area() for data in idf['BuildingSurface:Detailed'].values() if data['Surface Type'] == 'Roof'])
            air_volume = floor_area * wall_height; # m^3
            air_mass = air_volume * 1.2; # kg

            # surface properties
            thermal_conductance = 0.0
            thermal_mass = 0.0
            solar_absorptance = 0.0
            solar_transmittance = 0.0;
            for surface in idf['BuildingSurface:Detailed'].values():
                surface_name = surface['Construction Name']
                area = vertices(surface).area()
                if 'Construction:FfactorGroundFloor' in idf.data.keys() and surface_name in idf['Construction:FfactorGroundFloor']:
                    construction = idf['Construction:FfactorGroundFloor'][surface_name]
                    # print(construction)
                    f_factor = construction['F-Factor {W/m-K}']
                    perimeter = construction['PerimeterExposed {m}']
                    thermal_conductance += f_factor * perimeter
                elif surface_name in idf['Construction']:
                    construction = idf['Construction'][surface_name]
                    for item in construction.values():
                        if item in idf['Material']:
                            layer = idf['Material'][item]
                            conductivity = layer['Conductivity {W/m-K}']
                            thickness= layer['Thickness {m}']
                            specific_heat = layer['Specific Heat {J/kg-K}']
                            density = layer['Density {kg/m3}']
                            thermal_conductance += conductivity*thickness*area
                            thermal_mass += specific_heat*density*thickness*area
                        elif item in idf['Material:NoMass']:
                            layer = idf['Material:NoMass'][item]
                            resistance = layer['Thermal Resistance {m2-K/W}']
                            thermal_conductance += area/resistance
                        else:
                            raise Exception(f"'{item}' not found")
                        # TODO: get absorptance values
                else:
                    raise Exception(f"'{surface_name}' not found")

            for surface in idf['FenestrationSurface:Detailed'].values():
                #print(surface)
                # TODO: get window values
                pass   
            
            building_data["building_type"].append(building_type)
            building_data["floor_area"].append(round(floor_area,2)) # m^2
            building_data["UA"].append(round(thermal_conductance,4)) # W/K
            building_data["CM"].append(round(thermal_mass,1)) # J/K
            building_data["CA"].append(air_mass*1.006) # J/K
            building_data["UI"].append(round(thermal_conductance,4)/2) # W/K
            building_data["UM"].append(round(thermal_conductance,4)/2) # W/K

    building_data = pandas.DataFrame(building_data).set_index(['building_type'])
    building_data.to_csv(f"{os.path.basename(dirname)}.csv",index=True,header=True,float_format='%.4g')
    building_types = building_data.groupby('building_type')
    counts = pandas.DataFrame(building_types.count())
    means = pandas.DataFrame(building_types.mean())
    gmeans = pandas.DataFrame(numpy.exp(numpy.log(building_data).groupby('building_type').mean()))
    medians = pandas.DataFrame(building_types.median())
    stds = pandas.DataFrame(building_types.std())
    gstds = pandas.DataFrame(numpy.exp(numpy.log(building_data).groupby('building_type').std()))
    mins = pandas.DataFrame(building_types.min())
    maxs = pandas.DataFrame(building_types.max())
    building_stats = []
    for column in building_data.columns:
        # print("***",column,"***")
        values = [list(counts.index),[column for x in counts.index],list(counts[column]),list(means[column]),list(stds[column]),
            list(gmeans[column]),list(gstds[column]),list(mins[column]),list(medians[column]),list(maxs[column])]
        building_stats.append(pandas.DataFrame(numpy.array(values).transpose(),
            columns = ["building_type","value","count","mean","std","gmean","gstd","min","median","max"]))
    building_stats = pandas.concat(building_stats).set_index(['building_type','value']).apply(pandas.to_numeric,downcast='float',errors='coerce').fillna(0.0)
    building_stats.to_csv(f"{os.path.basename(dirname)}_stats.csv",index=True,header=True,float_format='%.4g')
