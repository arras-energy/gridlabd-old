"""poles loading analysis for a GridLAB-D model

SYNTAX

Shell:

    bash$ gridlabd pole_analysis INPUTFILE OPTIONS ...

GLM:

    #gridlabd pole_analysis INPUTFILE OPTIONS ...

Python:

    >>> import pole_analysis
    >>> pole_analysis.main(INPUTFILE,OPTIONS ...)

Output options:
  
  --output=CVSNAME                  set the output CVS file name
                                    (default is /dev/stdout)
Analysis options:
  
  --analysis=ANALYSIS               set the type of pole analysis
                                    (available analysis includes worst_angle,
                                    critical_speed, and loading_scenario)

  --pole=NAME                       set the name of pole to be analysed
                                    (default is ALL)

  --wind_speed=SPEED                set the wind speed in m/s 

  --wind_direction=DEGREE           set the wind direction in degree 

  --direction_increment=DEGREE      set the wind direction swapping increment 

  --speed_increment=SPEED           set the wind speed swapping increment 

  --segment=NUMBER                  set the number of the analysis segment 


DESCRIPTION

The `pole_analysis` subcommand conducts loading analysis for pole objects at 
both grounding level and superstructure. When the pole is fastened with guy 
wires, pole structures below at which the guy wire is mounted are assumed to 
be rigid. The output is written to `/dev/stdout` unless the `--output=CSV` 
option is given.

The `--analysis=ANALYSIS` option is required to specify the analysis. The 
`--wind_speed=SPEED` and `--wind_direction=DEGREE ` option set the wind speed 
and and wind direction used in loading_scenario analysis. The default value is 
8.8233 m/s for wind speed and 0.0 degree for wind direction.

The `--direction_increment=DEGREE` option sets the swapping angle for finding the 
worst wind loading angle which has the biggest impact. The `--speed_increment=SPEED` 
option sets the speed increment to indentify the pole failure speed. 

"""

import sys, os, json, datetime, subprocess
from math import cos, sin, tan, atan, pi, sqrt
import numpy as np
import pandas as pd
from haversine import haversine, Unit

def error(msg,code=None):
    """Display error message and exit with code"""
    print(f"ERROR [pole_analysis]: {msg}",file=sys.stderr)
    if code == None:
        return
    if type(code) is int:
        exit(code)
    raise Exception(f"exit code '{code}' is not valid")

def warning(msg):
    """Display a warning message"""
    print(f"WARNING [pole_analysis]: {msg}",file=sys.stderr)

def syntax(code=None):
    """Display syntax/help message

    Parameters:

        - code (int or None)  Output to stdout if evaluates to False, otherwise output to stderr. 
                              If None, output full help, otherwise output only syntax line.
                              If integer, exit with code

    Exceptions:

        - ValueError          Type of code is not int or None
    """
    if not code:
        output = (lambda x: print(x,file=sys.stdout))
    else:
        output = (lambda x: print(x,file=sys.stderr))
    if code == None:
        output(__doc__)
    else:
        output(f"Syntax: gridlabd pole_analysis INPUTFILE OPTIONS ...")
    if type(code) is int:
        exit(code)
    elif code != None:
        raise ValueError(f"error code '{code}' is not valid")

# default variables
VERBOSE = False
DEFAULT_WIND_SPEED = 8.8233 # in m/s for wind load 1
DEFAULT_WIND_DIRECTION = 0.0 # in degree
DEFAULT_DIRECTION_INCREMENT = 10.0 # in deg
DEFAULT_SPEED_INCREMENT = 5.0 # in m/s
DEFAULT_SEGMENT_DIVES = 100.0
DEFAULT_MODULUE_ELASTICITY = 343000000.0 # lbs/ft2
ice_density = 0.03312 # lbs/in3
initDef = 0.0
initRot = 0.0

# Initialise data containers
reactions = np.array([0.0,0.0]) #Reactions (xV, yV) - Defined as array of floats
pole_analysis_template = {
    "analysis_height" : [],
    "PW_record" : [],
    "CT_record" : [],
    "CW_record" : [],
    "EW_record" : [],
    "WP_record" : [],
    "WC_record" : [],
    "WE_record" : [],
    "reactions" : [],
    "xShearForce" : [],
    "yShearForce" : [],
    "xBendingMoment" : [],
    "yBendingMoment" : [],
    "xRotation" : [],
    "yRotation" : [],
    "xDeflection" : [],
    "yDeflection" : [],
    "stress" : [],
    "critical_wind_speed" : 0.0,
    "critical_wind_direction" : 0.0,
    "worst_angle" : 0.0,
    "worst_angle_speed" : 0.0,
}
pole_data_template = {
    "ground_diameter" : 32.5/3.14, # in inch
    "top_diameter" : 19.5/3.14, # in inch
    "pole_length" : 45, # in ft
    "pole_depth" : 4.5, # in ft
    "guy_height" : 0.0, # in ft
    "tilt_angle" : 0.0, # in degree
    "tilt_direction" : 0.0, # in degree
    "fiber_strength" : 8000.0, # in psi
    "material_density" : 35.0, # in lbs/sf3
    "overload_factor_vertical" : 1.9,
    "overload_factor_transverse_general" : 1.75,
    "overload_factor_transverse_wire" : 1.65,
    "strength_factor_250b_wood" : 0.85,
    "pole_mounts" : {},
}
mount_data_line_template = {
    "equipment_is_line" : True,
    # in feet
    "pole_spacing" : 10.0,
    "height" : 1.0,
    "offset" : 0.0,
    # in inch
    "cable_diameter" : 0.1,
    "ice_thickness" : 0.0,
    # in degree
    "direction" : 0.0,
    # in lbs or lbs/ft
    "weight" : 0.1,
    "strength" : 100.0,
}
mount_data_equipment_template = {
    "equipment_is_line" : False,
    # in feet
    "height" : 1.0,
    "offset" : 1.0,
    # in sqrt feet
    "area" : 1.0,
    # in degree
    "offset_direction" : 0.0,
    # in lbs
    "weight" : 100.0,
}

pole_analysis = {}
ANALYSIS = None
wind_speed = None
wind_direction = None
direction_increment = None
speed_increment = None
modulus_elasticity = None
segment_dives = None
poles_selected = None
outputfile = None

def verbose(msg):
    if VERBOSE:
        print(msg)

def pole_diameter(g_diameter,t_diameter,h,H):
    x_diameter = g_diameter - h*(g_diameter-t_diameter)/H
    return x_diameter

def reactions_PW(g_diameter,t_diameter,height,density,tilt_angle,tilt_direction):
    # reactions due to pole weight and pole tilt
    pole_weight = density*sin(pi*tilt_angle/180)*pi\
                    *(g_diameter**2+g_diameter*t_diameter+t_diameter**2)*height/(3*4)
    xV = -pole_weight*cos(pi*tilt_direction/180)
    yV = -pole_weight*sin(pi*tilt_direction/180)
    return xV, yV

def reactions_CT(strength,tilt_angle,heading_direction):
    # reactions due to cable tension
    cable_tension = strength*cos(pi*tilt_angle/180)
    xV = -cable_tension*cos(pi*heading_direction/180)
    yV = -cable_tension*sin(pi*heading_direction/180)
    return xV, yV

def reactions_CW(g_diameter,t_diameter,height,weight,gP,zP,tilt_angle,tilt_direction,offset_direction):
    # reactions due to cable weight
    x_diameter = pole_diameter(g_diameter,t_diameter,zP,height)
    x_radius = x_diameter/2
    cable_weight_moment = weight*x_radius
    if tilt_angle == 0:
        xV = -cable_weight_moment*cos(pi*offset_direction/180)/(zP-gP)
        yV = -cable_weight_moment*sin(pi*offset_direction/180)/(zP-gP)
    else:
        xyV = -(weight*sin(pi*tilt_angle/180)+cable_weight_moment*cos(pi*tilt_angle/180)/(zP-gP))
        xV = xyV*cos(pi*tilt_direction/180)
        yV = xyV*sin(pi*tilt_direction/180)
    return xV, yV

def reactions_EW(weight,gP,zP,offset,tilt_angle,tilt_direction,offset_direction):
    # reactions due to equiption weight
    equipment_weight_moment = weight*offset
    if tilt_angle == 0:
        xV = -equipment_weight_moment*cos(pi*offset_direction/180)/(zP-gP)
        yV = -equipment_weight_moment*sin(pi*offset_direction/180)/(zP-gP)
    else:
        xyV = -(weight*sin(pi*tilt_angle/180)+equipment_weight_moment*cos(pi*tilt_angle/180)/(zP-gP))
        xV = xyV*cos(pi*tilt_direction/180)
        yV = xyV*sin(pi*tilt_direction/180)
    return xV, yV

def reactions_WP(wind_presure,wind_direction,g_diameter,t_diameter,height,tilt_angle):
    # reactions due to wind loads (in lbs/ft2) on pole
    wind_load = wind_presure*height**2*(g_diameter+2*t_diameter)*cos(pi*tilt_angle/180)/72
    xV = -wind_load*cos(pi*wind_direction/180)
    yV = -wind_load*sin(pi*wind_direction/180)
    return xV, yV

def reactions_WC(wind_presure,wind_direction,cable_area,tilt_angle,heading_direction):
    # reactions due to wind loads (in lbs/ft2) on cable
    wind_load = wind_presure*cable_area*abs(sin(pi*(wind_direction-heading_direction)/180))*cos(pi*tilt_angle/180)
    xV = -wind_load*cos(pi*heading_direction/180)
    yV = -wind_load*sin(pi*heading_direction/180)
    return xV, yV

def reactions_WE(wind_presure,wind_direction,equipment_area,tilt_angle):
    wind_load = wind_presure*equipment_area*cos(pi*tilt_angle/180)
    xV = -wind_load*cos(pi*wind_direction/180)
    yV = -wind_load*sin(pi*wind_direction/180)
    return xV, yV

def shear_moment_WE(xV,yV,zP,gP,Z):
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable tension
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable tension
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable tension
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable tension
    for i, x in enumerate(Z):
        if x >= gP and x <= zP:
            xshear = xV
            yshear = yV
            xmoment = -xV*(zP-x)
            ymoment = -yV*(zP-x)
        else:
            xshear = 0.0
            yshear = 0.0
            xmoment = 0.0
            ymoment = 0.0
        xShear[i] = xshear
        yShear[i] = yshear
        xMoment[i] = xmoment
        yMoment[i] = ymoment
    return xShear, yShear, xMoment, yMoment  

def shear_moment_WC(xV,yV,zP,gP,Z):
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable tension
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable tension
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable tension
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable tension
    for i, x in enumerate(Z):
        if x >= gP and x <= zP:
            xshear = xV
            yshear = yV
            xmoment = -xV*(zP-x)
            ymoment = -yV*(zP-x)
        else:
            xshear = 0.0
            yshear = 0.0
            xmoment = 0.0
            ymoment = 0.0
        xShear[i] = xshear
        yShear[i] = yshear
        xMoment[i] = xmoment
        yMoment[i] = ymoment
    return xShear, yShear, xMoment, yMoment  

def shear_moment_WP(wind_presure,wind_direction,gP,Z,g_diameter,t_diameter,height,tilt_angle):
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for equipment weight
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for equipment weight
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for equipment weight
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for equipment weight
    for i, x in enumerate(Z):
        if x >= gP:
            x = x - gP
            x_diameter = pole_diameter(g_diameter,t_diameter,x,height)
            shear = -wind_presure*(t_diameter+x_diameter)*(height-x)*cos(pi*tilt_angle/180)/2
            moment = wind_presure*cos(pi*tilt_angle/180)*(height**2*(g_diameter+2*t_diameter)/72 \
            + x*(x_diameter*(x-height)-height*t_diameter - x**2*(g_diameter-t_diameter)/(3*height))/24)
        else:
            shear = 0.0
            moment = 0.0
        xShear[i] = -shear*cos(pi*wind_direction/180)
        yShear[i] = -shear*sin(pi*wind_direction/180)
        xMoment[i] = moment*cos(pi*wind_direction/180)
        yMoment[i] = moment*sin(pi*wind_direction/180)
    
    return xShear, yShear, xMoment, yMoment 

def shear_moment_EW(xV,yV,zP,gP,Z):
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for equipment weight
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for equipment weight
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for equipment weight
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for equipment weight
    for i, x in enumerate(Z):
        if x >= gP and x <= zP:
            xshear = xV
            yshear = yV
            xmoment = -xV*(zP-gP)
            ymoment = -yV*(zP-gP)
        else:
            xshear = 0.0
            yshear = 0.0
            xmoment = 0.0
            ymoment = 0.0
        xShear[i] = xshear
        yShear[i] = yshear
        xMoment[i] = xmoment
        yMoment[i] = ymoment
    return xShear, yShear, xMoment, yMoment 

def shear_moment_CW(xV,yV,zP,gP,Z):
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable weight
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable weight
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable weight
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable weight
    for i, x in enumerate(Z):
        if x >= gP and x <= zP:
            xshear = xV
            yshear = yV
            xmoment = -xV*(zP-x)
            ymoment = -yV*(zP-x)
        else:
            xshear = 0.0
            yshear = 0.0
            xmoment = 0.0
            ymoment = 0.0
        xShear[i] = xshear
        yShear[i] = yshear
        xMoment[i] = xmoment
        yMoment[i] = ymoment
    return xShear, yShear, xMoment, yMoment 

def shear_moment_CT(xV,yV,zP,gP,Z,heading_direction):
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable tension
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for cable tension
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable tension
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for cable tension
    for i, x in enumerate(Z):
        if x >= gP and x <= zP:
            xshear = xV
            yshear = yV
            xmoment = -xV*(zP-x)
            ymoment = -yV*(zP-x)
        else:
            xshear = 0.0
            yshear = 0.0
            xmoment = 0.0
            ymoment = 0.0
        xShear[i] = xshear
        yShear[i] = yshear
        xMoment[i] = xmoment
        yMoment[i] = ymoment
    return xShear, yShear, xMoment, yMoment

def shear_moment_PW(g_diameter,t_diameter,gP,Z,height,density,tilt_angle,tilt_direction):  
    #Cycle through the structure and calculate the shear force and bending moment at each point
    xShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for pole weight
    yShear = np.zeros(len(Z))  #Initialise a container to hold all shear force data for pole weight
    xMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for pole weight
    yMoment = np.zeros(len(Z)) #Initialise a container to hold all moment force data for pole weight
    for i, x in enumerate(Z):
        if x >= gP:
            x = x - gP
            x_diameter = pole_diameter(g_diameter,t_diameter,x,height)
            pole_weight = density*sin(pi*tilt_angle/180)*pi*(x_diameter**2+x_diameter*t_diameter+t_diameter**2)\
                        *(height-x)/(3*4*12*12)
            pole_weight_moment = density*sin(pi*tilt_angle/180)*pi*\
            (g_diameter**2*(height**2-x**2)/(2*4)+(g_diameter-t_diameter)**2*(height**4-x**4)/(4*4*height**2)\
            -2*g_diameter*(g_diameter-t_diameter)*(height**3-x**3)/(3*4*height))/(12*12)
        else:
            pole_weight = 0.0
            pole_weight_moment = 0.0
        xShear[i] = -pole_weight*cos(pi*tilt_direction/180)
        yShear[i] = -pole_weight*sin(pi*tilt_direction/180)
        xMoment[i] = pole_weight_moment*cos(pi*tilt_direction/180)
        yMoment[i] = pole_weight_moment*sin(pi*tilt_direction/180)
    return xShear, yShear, xMoment, yMoment   

def calcDeflection(M, gP, Z, E, delX, theta_0, v_0, g_diameter,t_diameter, height):
    theta_im1 = theta_0
    v_im1 = v_0
    Rotation =  np.zeros(len(Z))
    Rotation[0] = theta_im1
    Deflection =  np.zeros(len(Z))
    Deflection[0] = v_im1
    for i, m in enumerate(M):
        ind = i
        if i > 0 and Z[i] >= gP:
            x = Z[i] - gP
            x_diameter = pole_diameter(g_diameter,t_diameter,x,height)
            I = 3.14*(x_diameter/2/12)**4/4 #ft4
            
            M_im1 = M[ind-1]
            M_i = M[ind]
            M_avg = 0.5*(M_im1+M_i)
            
            theta_i = theta_im1 + (M_avg/(E*I))*delX
            v_i = v_im1 + 0.5*(theta_i+theta_im1)*delX
            Rotation[ind] = theta_i
            Deflection[ind] = v_i
            
            theta_im1 = theta_i
            v_im1 = v_i
    
    return Rotation, Deflection

def calcStress(M, gP, Z, g_diameter,t_diameter, height):
    Stress =  np.zeros(len(Z))
    for i, m in enumerate(M):
        if Z[i] >= gP:
            x = Z[i] - gP
            x_diameter = pole_diameter(g_diameter,t_diameter,x,height)
            I = 3.14*(x_diameter/2/12)**4/4
            stress = m*x_diameter/(2*I*12) # in lbs/ft2
            Stress[i] = stress * 0.00694 # lbs/ft2 to psi
    return Stress

def main(inputfile,**options):
    """Main pole creation function"""
    # options
    global ANALYSIS
    global wind_speed
    global wind_direction
    global direction_increment
    global speed_increment
    global modulus_elasticity
    global segment_dives
    global poles_selected
    # outputfile = "/dev/stdout"
    global outputfile
    global VERBOSE
    for opt,value in options.items():
        if opt == "analysis":
            ANALYSIS = value
        elif opt == "wind_speed":
            wind_speed = float(value)
        elif opt == "wind_direction":
            wind_direction = float(value)
        elif opt == "direction_increment":
            direction_increment = float(value)
        elif opt == "speed_increment":
            speed_increment = float(value)
        elif opt == "segment":
            segment_dives = float(value)
        elif opt == "modulus_elasticity":
            modulus_elasticity = float(value)
        elif opt == "poles_selected":
            poles_selected = value
        elif opt == "output":
            outputfile = value
        elif opt == "verbose":
            VERBOSE = True
        else:
            raise Exception(f"options '{opt}={value}' is not valid")    
    if ANALYSIS == None:
        raise Exception("option for analysis is required")
    if ANALYSIS not in ["worst_angle", "critical_speed", "loading_scenario"]:
        error(f"analysis {ANALYSIS} isn't supported",3)
    if wind_speed == None:
        wind_speed = DEFAULT_WIND_SPEED
    if wind_direction == None:
        wind_direction = DEFAULT_WIND_DIRECTION
    if direction_increment == None:
        direction_increment = DEFAULT_DIRECTION_INCREMENT
    if speed_increment == None:
        speed_increment = DEFAULT_SPEED_INCREMENT
    if segment_dives == None:
        segment_dives = DEFAULT_SEGMENT_DIVES
    if modulus_elasticity == None:
        modulus_elasticity = DEFAULT_MODULUE_ELASTICITY

    # input
    if inputfile.endswith(".glm"):
        glmfile = inputfile
        jsonfile = inputfile.replace(".glm",".json")
        code = os.system(f"gridlabd -C {glmfile} -o {jsonfile}")
        if code != 0:
            error(f"unable to compile '{glmfile}' into {jsonfile} (error code {code})",2)
    elif inputfile.endswith(".json"):
        jsonfile = inputfile
    else:
        error(f"main(inputfile='{inputfile}',options={options}): inputfile type not recognized",1)
    with open(jsonfile,"r") as f:
        model = json.load(f)
    objects = model["objects"]
    poles = {}
    # process poles in model
    for name in list(objects.keys()):
        data = model["objects"][name]
        if "class" in data.keys() and data["class"] == "pole":
            poles[name] = pole_data_template.copy()
            if "tilt_angle" in data.keys():
                poles[name]["tilt_angle"] = float(data["tilt_angle"].split()[0])
            if "tilt_direction" in data.keys():
                poles[name]["tilt_direction"] = float(data["tilt_direction"].split()[0])
            config_name = data["configuration"]
            config_data = model["objects"][config_name]
            if "ground_diameter"in config_data.keys():
                poles[name]["ground_diameter"] = float(config_data["ground_diameter"].split()[0])
            if "top_diameter"in config_data.keys():
                poles[name]["top_diameter"] = float(config_data["top_diameter"].split()[0])
            if "pole_length"in config_data.keys():
                poles[name]["pole_length"] = float(config_data["pole_length"].split()[0])
            if "pole_depth"in config_data.keys():
                poles[name]["pole_depth"] = float(config_data["pole_depth"].split()[0])
            if "fiber_strength"in config_data.keys():
                poles[name]["fiber_strength"] = float(config_data["fiber_strength"].split()[0])
            if "material_density"in config_data.keys():
                poles[name]["material_density"] = float(config_data["material_density"].split()[0])
            if "overload_factor_vertical"in config_data.keys():
                poles[name]["overload_factor_vertical"] = float(config_data["overload_factor_vertical"])
            if "overload_factor_transverse_general"in config_data.keys():
                poles[name]["overload_factor_transverse_general"] = float(config_data["overload_factor_transverse_general"])
            if "overload_factor_transverse_wire"in config_data.keys():
                poles[name]["overload_factor_transverse_wire"] = float(config_data["overload_factor_transverse_wire"])
            if "strength_factor_250b_wood"in config_data.keys():
                poles[name]["strength_factor_250b_wood"] = float(config_data["strength_factor_250b_wood"])
    
    for name in list(objects.keys()):
        data = model["objects"][name]
        if "class" in data.keys() and data["class"] == "pole_mount":
            equipment_name = data["equipment"]
            parent_name = data["parent"]
            equipment_data = model["objects"][equipment_name]
            if "class" in equipment_data.keys() and equipment_data["class"] == "overhead_line":
                line_config_name = equipment_data["configuration"]
                line_config_data = model["objects"][line_config_name]
                line_spacing_name =  line_config_data["spacing"]
                line_spacing_data =  model["objects"][line_spacing_name]
                direction = float(data["direction"].split()[0])
                offset = float(data["offset"].split()[0])
                pole_spacing = float(equipment_data["length"].split()[0])
                ice_thickness = float(equipment_data["ice_thickness"].split()[0])
                for phase in equipment_data["phases"]:
                    conductor_name =  line_config_data[f"conductor_{phase}"]
                    conductor_data = model["objects"][conductor_name]
                    conductor_weight = float(conductor_data["weight"].split()[0])
                    conductor_diameter = float(conductor_data["diameter"].split()[0])
                    conductor_tension = float(conductor_data["strength"].split()[0])
                    mount_height = float(line_spacing_data[f"distance_{phase}E"].split()[0])
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"] = mount_data_line_template.copy()
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["pole_spacing"] = pole_spacing
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["height"] = mount_height
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["offset"] = offset
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["cable_diameter"] = conductor_diameter
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["ice_thickness"] = ice_thickness
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["direction"] = direction
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["weight"] = conductor_weight
                    poles[parent_name]["pole_mounts"][f"{equipment_name}_{phase}"]["strength"] = conductor_tension
            elif "class" in equipment_data.keys() and equipment_data["class"] == "transformer":
                poles[parent_name]["pole_mounts"][equipment_name] = mount_data_equipment_template.copy()
                poles[parent_name]["pole_mounts"][equipment_name]["height"] = float(data["height"].split()[0])
                poles[parent_name]["pole_mounts"][equipment_name]["offset"] = float(data["offset"].split()[0])
                poles[parent_name]["pole_mounts"][equipment_name]["area"] = float(data["area"].split()[0])
                poles[parent_name]["pole_mounts"][equipment_name]["offset_direction"] = float(data["direction"].split()[0])
                poles[parent_name]["pole_mounts"][equipment_name]["weight"] = float(data["weight"].split()[0])
            else:
                error(f"equipment {equipment_name} isn't supported",4)
    # print(poles)
    for pole in poles.keys():
        if poles_selected is None or pole in poles_selected:
            PW_record = np.empty([0,2])
            GT_record = np.empty([0,2])
            CT_record = np.empty([0,2])
            CW_record = np.empty([0,2])
            EW_record = np.empty([0,2])
            WP_record = np.empty([0,2])
            WC_record = np.empty([0,2])
            WE_record = np.empty([0,2])
            pole_data = poles[pole]
            pole_analysis[pole] = pole_analysis_template.copy()
            pole_length = pole_data['pole_length'] - pole_data['pole_depth']
            guy_height = pole_data['guy_height']
            height = pole_length - guy_height
            if height <= 0:
                raise Exception(f"{height} ft is not a valid value for pole height")
            t_diameter = pole_data['top_diameter']
            g_diameter = pole_diameter(pole_data['ground_diameter'],t_diameter,guy_height,pole_length)
            density = pole_data['material_density']
            tilt_angle = pole_data['tilt_angle']
            tilt_direction = pole_data['tilt_direction']
            delta_height = pole_length/segment_dives
            Z = np.arange(0, pole_length + delta_height, delta_height) #Range of z-coordinates
            xShearForce = np.empty([0,len(Z)]) #Shear forces at each data point
            yShearForce = np.empty([0,len(Z)]) #Shear forces at each data point
            xBendingMoment = np.empty([0,len(Z)]) #Bending moment at each data point
            yBendingMoment = np.empty([0,len(Z)]) #Bending moment at each data point
            overload_factor_vertical = pole_data["overload_factor_vertical"]
            overload_factor_transverse_wire = pole_data["overload_factor_transverse_wire"]
            overload_factor_transverse_general = pole_data["overload_factor_transverse_general"]
            strength_factor_250b_wood = pole_data["strength_factor_250b_wood"]

            ## pole weight
            xV, yV = reactions_PW(g_diameter,t_diameter,height,density,tilt_angle,tilt_direction)
            xV = xV * overload_factor_vertical # consider overload factor
            yV = yV * overload_factor_vertical
            PW_record = np.append(PW_record, [np.array([xV, yV])], axis=0)
            reactions[0] = reactions[0] + xV
            reactions[1] = reactions[1] + yV
            xShear, yShear, xMoment, yMoment = shear_moment_PW(g_diameter,t_diameter,guy_height,Z,height,density,tilt_angle,\
                                                               tilt_direction)
            xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for pole weight
            yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for pole weight
            xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment record for pole weight
            yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment record for pole weight

            for pole_mount in pole_data["pole_mounts"].keys():
                mount_data = pole_data["pole_mounts"][pole_mount]
                if mount_data["equipment_is_line"]:
                    strength = mount_data["strength"]
                    heading_direction = mount_data["direction"]
                    zP = mount_data["height"]
                    weight = mount_data["weight"]*mount_data["pole_spacing"]/2 + pi*mount_data["ice_thickness"]*\
                        (mount_data["ice_thickness"]+mount_data["cable_diameter"])*mount_data["pole_spacing"]*\
                        ice_density*12/2
                    offset_direction = mount_data["direction"]
                    ## cable tension
                    if guy_height < zP:
                        xV, yV = reactions_CT(strength,tilt_angle,heading_direction)
                        xV = xV * overload_factor_transverse_wire
                        yV = yV * overload_factor_transverse_wire
                    else:
                        xV = 0.0
                        yV = 0.0
                    CT_record = np.append(CT_record, [np.array([xV, yV])], axis=0)
                    reactions[0] = reactions[0] + xV
                    reactions[1] = reactions[1] + yV
                    xShear, yShear, xMoment, yMoment = shear_moment_CT(xV,yV,zP,guy_height,Z,heading_direction)
                    xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for cable tension
                    yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for cable tension
                    xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment for cable tension
                    yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment for cable tension
                    ## cable weight
                    if guy_height < zP:
                        xV, yV = reactions_CW(g_diameter,t_diameter,height,weight,guy_height,zP,tilt_angle,tilt_direction,\
                                              offset_direction)
                        xV = xV * overload_factor_vertical
                        yV = yV * overload_factor_vertical
                    else:
                        xV = 0.0
                        yV = 0.0
                    CW_record = np.append(CW_record, [np.array([xV, yV])], axis=0)
                    reactions[0] = reactions[0] + xV
                    reactions[1] = reactions[1] + yV
                    xShear, yShear, xMoment, yMoment = shear_moment_CW(xV,yV,zP,guy_height,Z)
                    xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for cable weight
                    yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for cable weight
                    xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment for cable weight
                    yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment for cable weight
                else:
                    ## equipment weight
                    weight = mount_data["weight"]
                    zP = mount_data["height"]
                    offset_direction = mount_data["offset_direction"]
                    offset = mount_data["offset"]
                    if guy_height < zP:
                        xV, yV = reactions_EW(weight,guy_height,zP,offset,tilt_angle,tilt_direction,offset_direction)
                        xV = xV * overload_factor_vertical
                        yV = yV * overload_factor_vertical
                    else:
                        xV = 0.0
                        yV = 0.0
                    EW_record = np.append(EW_record, [np.array([xV, yV])], axis=0)
                    reactions[0] = reactions[0] + xV
                    reactions[1] = reactions[1] + yV
                    xShear, yShear, xMoment, yMoment = shear_moment_EW(xV,yV,zP,guy_height,Z)
                    xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for equipment weight
                    yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for equipment weight
                    xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment for equipment weight
                    yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment for equipment weight

            if ANALYSIS == "worst_angle":
                if np.isnan(wind_speed) or wind_speed == 0.0:
                    warning(f"wind speed hasn't defined, use DEFAULT_WIND_SPEED (8.8233 m/s)")
                    wind_speed = DEFAULT_WIND_SPEED
                if np.isnan(direction_increment) or direction_increment == 0.0:
                    warning(f"wind direction increment hasn't defined, use DEFAULT_DIRECTION_INCREMENT (2 degree)")
                    direction_increment = DEFAULT_DIRECTION_INCREMENT

                anaglysis_directions = np.arange(0, 360, direction_increment) #Range of wind direction
                anaglysis_stress =  np.zeros(len(anaglysis_directions))

                for i, anaglysis_direction in enumerate(anaglysis_directions):
                    wind_direction = anaglysis_direction
                    wind_presure = 0.00256*(2.24*wind_speed)**2 #2.24 account for m/s to mph conversion, in lb/ft2
                    xAnalysisShearForce = np.empty([0,len(Z)]) #Shear forces at each data point
                    yAnalysisShearForce = np.empty([0,len(Z)]) #Shear forces at each data point
                    xAnalysisBendingMoment = np.empty([0,len(Z)]) #Bending moment at each data point
                    yAnalysisBendingMoment = np.empty([0,len(Z)]) #Bending moment at each data point
                    ## wind load on pole
                    xV, yV = reactions_WP(wind_presure,wind_direction,g_diameter,t_diameter,height,tilt_angle)
                    xV = xV * overload_factor_transverse_general
                    yV = yV * overload_factor_transverse_general
                    WP_record = np.append(WP_record, [np.array([xV, yV])], axis=0)
                    reactions[0] = reactions[0] + xV
                    reactions[1] = reactions[1] + yV
                    xShear, yShear, xMoment, yMoment = shear_moment_WP(wind_presure,wind_direction,guy_height,Z,g_diameter,\
                                                                       t_diameter,height,tilt_angle)
                    xAnalysisShearForce = np.append(xAnalysisShearForce, [xShear],axis=0) #Store shear force record for pole wind load
                    yAnalysisShearForce = np.append(yAnalysisShearForce, [yShear],axis=0) #Store shear force record for pole wind load
                    xAnalysisBendingMoment = np.append(xAnalysisBendingMoment,[xMoment], axis=0) #Store bending moment for pole wind load
                    yAnalysisBendingMoment = np.append(yAnalysisBendingMoment,[yMoment], axis=0) #Store bending moment for pole wind load
                    ## wind load on pole mount
                    for pole_mount in pole_data["pole_mounts"].keys():
                        mount_data = pole_data["pole_mounts"][pole_mount]
                        if mount_data["equipment_is_line"]:
                            cable_area = (mount_data["ice_thickness"] + mount_data["cable_diameter"]) \
                                         * mount_data["pole_spacing"] # in ft2
                            heading_direction = mount_data["direction"]
                            zP = mount_data["height"]
                            xV, yV = reactions_WC(wind_presure,wind_direction,cable_area,tilt_angle,heading_direction)
                            xV = xV * overload_factor_transverse_general
                            yV = yV * overload_factor_transverse_general
                            WC_record = np.append(WC_record, [np.array([xV, yV])], axis=0)
                            reactions[0] = reactions[0] + xV
                            reactions[1] = reactions[1] + yV
                            xShear, yShear, xMoment, yMoment = shear_moment_WC(xV,yV,zP,guy_height,Z)
                            xAnalysisShearForce = np.append(xAnalysisShearForce,[xShear], axis=0) #Store shear force record for cablewind
                            yAnalysisShearForce = np.append(yAnalysisShearForce,[yShear], axis=0) #Store shear force record for cablewind
                            xAnalysisBendingMoment = np.append(xAnalysisBendingMoment,[xMoment], axis=0) #Store bending moment for cablewind
                            yAnalysisBendingMoment = np.append(yAnalysisBendingMoment,[yMoment], axis=0) #Store bending moment for cablewind
                        else:
                            equipment_area = mount_data["area"] # in ft2
                            zP = mount_data["height"]
                            xV, yV = reactions_WE(wind_presure,wind_direction,equipment_area,tilt_angle)
                            xV = xV * overload_factor_transverse_general
                            yV = yV * overload_factor_transverse_general
                            WE_record = np.append(WE_record, [np.array([xV, yV])], axis=0)
                            reactions[0] = reactions[0] + xV
                            reactions[1] = reactions[1] + yV
                            xShear, yShear, xMoment, yMoment = shear_moment_WE(xV,yV,zP,guy_height,Z)
                            xAnalysisShearForce = np.append(xAnalysisShearForce,[xShear], axis=0) #Store shear force record for equipmentwind
                            yAnalysisShearForce = np.append(yAnalysisShearForce,[yShear], axis=0) #Store shear force record for equipmentwind
                            xAnalysisBendingMoment = np.append(xAnalysisBendingMoment,[xMoment], axis=0) #Store bending moment for equipmentwind
                            yAnalysisBendingMoment = np.append(yAnalysisBendingMoment,[yMoment], axis=0) #Store bending moment for equipmentwind
                    ## total shear and moment
                    ShearForce = np.sqrt((sum(xShearForce)+sum(xAnalysisShearForce))**2+\
                                         (sum(yShearForce)+sum(yAnalysisShearForce))**2)
                    BendingMoment = np.sqrt((sum(xBendingMoment)+sum(xAnalysisBendingMoment))**2+\
                                            (sum(yBendingMoment)+sum(yAnalysisBendingMoment))**2)
                    ## pole stress
                    Stress = calcStress(BendingMoment, guy_height, Z, g_diameter,t_diameter, height)
                    Stress = 100 * Stress / (pole_data['fiber_strength']*strength_factor_250b_wood) # in %
                    ## record data
                    verbose(f"analysis direction: {wind_direction}, stress: {max(Stress)}")
                    if max(Stress) > 100:
                        warning(f"[{pole}] stress = {max(Stress)}% at direction {anaglysis_direction} degree")
                    anaglysis_stress[i] = max(Stress)
                ## record data
                verbose(f"analysis anagle: {anaglysis_directions}")
                verbose(f"analysis stress: {anaglysis_stress}")
                pole_analysis[pole]['worst_angle'] = anaglysis_directions[np.argmax(anaglysis_stress)]
                pole_analysis[pole]['worst_angle_speed'] = wind_speed
            elif ANALYSIS == "critical_speed":
                if np.isnan(wind_direction):
                    warning(f"wind direction hasn't defined, use DEFAULT_WIND_DIRECTION (0.0 degree)")
                    wind_direction = DEFAULT_WIND_DIRECTION
                if np.isnan(speed_increment) or speed_increment == 0.0:
                    warning(f"wind speed increment hasn't defined, use DEFAULT_SPEED_INCREMENT (5 m/s)")
                    speed_increment = DEFAULT_SPEED_INCREMENT

                anaglysis_windspeed = [] #list of wind speed
                anaglysis_stress =  []
                critical_wind_speed = 0.0
                critical_stress = 0.0
                while critical_stress < 100.0:
                    wind_speed = critical_wind_speed
                    verbose(f"analysis wind speed: {wind_speed}")
                    xAnalysisShearForce = np.empty([0,len(Z)]) #Shear forces at each data point
                    yAnalysisShearForce = np.empty([0,len(Z)]) #Shear forces at each data point
                    xAnalysisBendingMoment = np.empty([0,len(Z)]) #Bending moment at each data point
                    yAnalysisBendingMoment = np.empty([0,len(Z)]) #Bending moment at each data point
                    wind_presure = 0.00256*(2.24*wind_speed)**2 #2.24 account for m/s to mph conversion, in lb/ft2
                    ## wind load on pole
                    xV, yV = reactions_WP(wind_presure,wind_direction,g_diameter,t_diameter,height,tilt_angle)
                    xV = xV * overload_factor_transverse_general
                    yV = yV * overload_factor_transverse_general
                    WP_record = np.append(WP_record, [np.array([xV, yV])], axis=0)
                    reactions[0] = reactions[0] + xV
                    reactions[1] = reactions[1] + yV
                    xShear, yShear, xMoment, yMoment = shear_moment_WP(wind_presure,wind_direction,guy_height,Z,g_diameter,\
                                                                       t_diameter,height,tilt_angle)
                    xAnalysisShearForce = np.append(xAnalysisShearForce, [xShear],axis=0) #Store shear force record for pole wind load
                    yAnalysisShearForce = np.append(yAnalysisShearForce, [yShear],axis=0) #Store shear force record for pole wind load
                    xAnalysisBendingMoment = np.append(xAnalysisBendingMoment,[xMoment], axis=0) #Store bending moment for pole wind load
                    yAnalysisBendingMoment = np.append(yAnalysisBendingMoment,[yMoment], axis=0) #Store bending moment for pole wind load
                    ## wind load on pole mount
                    for pole_mount in pole_data["pole_mounts"].keys():
                        mount_data = pole_data["pole_mounts"][pole_mount]
                        if mount_data["equipment_is_line"]:
                            cable_area = (mount_data["ice_thickness"] + mount_data["cable_diameter"]) \
                                         * mount_data["pole_spacing"] # in ft2
                            heading_direction = mount_data["direction"]
                            zP = mount_data["height"]
                            xV, yV = reactions_WC(wind_presure,wind_direction,cable_area,tilt_angle,heading_direction)
                            xV = xV * overload_factor_transverse_general
                            yV = yV * overload_factor_transverse_general
                            WC_record = np.append(WC_record, [np.array([xV, yV])], axis=0)
                            reactions[0] = reactions[0] + xV
                            reactions[1] = reactions[1] + yV
                            xShear, yShear, xMoment, yMoment = shear_moment_WC(xV,yV,zP,guy_height,Z)
                            xAnalysisShearForce = np.append(xAnalysisShearForce,[xShear], axis=0) #Store shear force record for cablewind
                            yAnalysisShearForce = np.append(yAnalysisShearForce,[yShear], axis=0) #Store shear force record for cablewind
                            xAnalysisBendingMoment = np.append(xAnalysisBendingMoment,[xMoment], axis=0) #Store bending moment for cablewind
                            yAnalysisBendingMoment = np.append(yAnalysisBendingMoment,[yMoment], axis=0) #Store bending moment for cablewind
                        else:
                            equipment_area = mount_data["area"] # in ft2
                            zP = mount_data["height"]
                            xV, yV = reactions_WE(wind_presure,wind_direction,equipment_area,tilt_angle)
                            xV = xV * overload_factor_transverse_general
                            yV = yV * overload_factor_transverse_general
                            WE_record = np.append(WE_record, [np.array([xV, yV])], axis=0)
                            reactions[0] = reactions[0] + xV
                            reactions[1] = reactions[1] + yV
                            xShear, yShear, xMoment, yMoment = shear_moment_WE(xV,yV,zP,guy_height,Z)
                            xAnalysisShearForce = np.append(xAnalysisShearForce,[xShear], axis=0) #Store shear force record for equipmentwind
                            yAnalysisShearForce = np.append(yAnalysisShearForce,[yShear], axis=0) #Store shear force record for equipmentwind
                            xAnalysisBendingMoment = np.append(xAnalysisBendingMoment,[xMoment], axis=0) #Store bending moment for equipmentwind
                            yAnalysisBendingMoment = np.append(yAnalysisBendingMoment,[yMoment], axis=0) #Store bending moment for equipmentwind
                    ## total shear and moment
                    ShearForce = np.sqrt((sum(xShearForce)+sum(xAnalysisShearForce))**2+\
                                         (sum(yShearForce)+sum(yAnalysisShearForce))**2)
                    BendingMoment = np.sqrt((sum(xBendingMoment)+sum(xAnalysisBendingMoment))**2+\
                                            (sum(yBendingMoment)+sum(yAnalysisBendingMoment))**2)
                    ## pole stress
                    Stress = calcStress(BendingMoment, guy_height, Z, g_diameter,t_diameter, height)
                    Stress = 100 * Stress / (pole_data['fiber_strength']*strength_factor_250b_wood) # in %
                    ## record data
                    critical_stress = max(Stress)
                    verbose(f"critical_stress: {critical_stress}")
                    anaglysis_windspeed.append(wind_speed) #list of wind speed
                    anaglysis_stress.append(critical_stress)
                    ## update data
                    critical_wind_speed = critical_wind_speed + speed_increment
                ## record data
                verbose(f"analysis anagle: {anaglysis_windspeed}")
                verbose(f"analysis stress: {anaglysis_stress}")
                pole_analysis[pole]['critical_wind_speed'] = critical_wind_speed - speed_increment
                pole_analysis[pole]['critical_wind_direction'] = wind_direction
            elif ANALYSIS == "loading_scenario":
                if np.isnan(wind_speed):
                    warning(f"wind speed hasn't defined, use DEFAULT_WIND_SPEED (8.8233 m/s)")
                    wind_speed = DEFAULT_WIND_SPEED
                if np.isnan(wind_direction):
                    warning(f"wind direction hasn't defined, use DEFAULT_WIND_DIRECTION (0.0 degree)")
                    wind_direction = DEFAULT_WIND_DIRECTION
                wind_presure = 0.00256*(2.24*wind_speed)**2 #2.24 account for m/s to mph conversion, in lb/ft2
                ## wind load on pole
                xV, yV = reactions_WP(wind_presure,wind_direction,g_diameter,t_diameter,height,tilt_angle)
                xV = xV * overload_factor_transverse_general
                yV = yV * overload_factor_transverse_general
                WP_record = np.append(WP_record, [np.array([xV, yV])], axis=0)
                reactions[0] = reactions[0] + xV
                reactions[1] = reactions[1] + yV
                xShear, yShear, xMoment, yMoment = shear_moment_WP(wind_presure,wind_direction,guy_height,Z,g_diameter,\
                                                                   t_diameter,height,tilt_angle)
                xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for pole wind load
                yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for pole wind load
                xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment for pole wind load
                yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment for pole wind load
                ## wind load on pole mount
                for pole_mount in pole_data["pole_mounts"].keys():
                    mount_data = pole_data["pole_mounts"][pole_mount]
                    if mount_data["equipment_is_line"]:
                        cable_area = (mount_data["ice_thickness"] + mount_data["cable_diameter"]) \
                                     * mount_data["pole_spacing"] # in ft2
                        heading_direction = mount_data["direction"]
                        zP = mount_data["height"]
                        xV, yV = reactions_WC(wind_presure,wind_direction,cable_area,tilt_angle,heading_direction)
                        xV = xV * overload_factor_transverse_general
                        yV = yV * overload_factor_transverse_general
                        WC_record = np.append(WC_record, [np.array([xV, yV])], axis=0)
                        reactions[0] = reactions[0] + xV
                        reactions[1] = reactions[1] + yV
                        xShear, yShear, xMoment, yMoment = shear_moment_WC(xV,yV,zP,guy_height,Z)
                        xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for cablewind
                        yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for cablewind
                        xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment for cablewind
                        yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment for cablewind
                    else:
                        equipment_area = mount_data["area"] # in ft2
                        zP = mount_data["height"]
                        xV, yV = reactions_WE(wind_presure,wind_direction,equipment_area,tilt_angle)
                        xV = xV * overload_factor_transverse_general
                        yV = yV * overload_factor_transverse_general
                        WE_record = np.append(WE_record, [np.array([xV, yV])], axis=0)
                        reactions[0] = reactions[0] + xV
                        reactions[1] = reactions[1] + yV
                        xShear, yShear, xMoment, yMoment = shear_moment_WE(xV,yV,zP,guy_height,Z)
                        xShearForce = np.append(xShearForce, [xShear], axis=0) #Store shear force record for equipmentwind
                        yShearForce = np.append(yShearForce, [yShear], axis=0) #Store shear force record for equipmentwind
                        xBendingMoment = np.append(xBendingMoment, [xMoment], axis=0) #Store bending moment for equipmentwind
                        yBendingMoment = np.append(yBendingMoment, [yMoment], axis=0) #Store bending moment for equipmentwind
                ## total shear and moment
                ShearForce = np.sqrt(sum(xShearForce)**2+sum(yShearForce)**2)
                BendingMoment = np.sqrt(sum(xBendingMoment)**2+sum(yBendingMoment)**2)
                ## deflection
                xM = -sum(xBendingMoment)
                delX = Z[1] - Z[0]
                xRotation, xDeflection = calcDeflection(xM,guy_height,Z,modulus_elasticity,delX,initRot,initDef,g_diameter,t_diameter,height)
                yM = -sum(yBendingMoment)
                yRotation, yDeflection = calcDeflection(yM,guy_height,Z,modulus_elasticity,delX,initRot,initDef,g_diameter,t_diameter,height)
                ## pole stress
                Stress = calcStress(BendingMoment, guy_height, Z, g_diameter,t_diameter, height)
                Stress = 100 * Stress / (pole_data['fiber_strength']*strength_factor_250b_wood) # in %
                ## record data
                pole_analysis[pole]['analysis_height'] = Z
                pole_analysis[pole]['PW_record'] = np.sum(PW_record,axis=0)
                pole_analysis[pole]['CT_record'] = np.sum(CT_record,axis=0)
                pole_analysis[pole]['CW_record'] = np.sum(CW_record,axis=0)
                pole_analysis[pole]['EW_record'] = np.sum(EW_record,axis=0)
                pole_analysis[pole]['WP_record'] = np.sum(WP_record,axis=0)
                pole_analysis[pole]['WC_record'] = np.sum(WC_record,axis=0)
                pole_analysis[pole]['WE_record'] = np.sum(WE_record,axis=0)
                pole_analysis[pole]['reactions'] = reactions
                pole_analysis[pole]['xShearForce'] = np.sum(xShearForce,axis=0)
                pole_analysis[pole]['yShearForce'] = np.sum(yShearForce,axis=0)
                pole_analysis[pole]['xBendingMoment'] = np.sum(xBendingMoment,axis=0)
                pole_analysis[pole]['yBendingMoment'] = np.sum(yBendingMoment,axis=0)
                pole_analysis[pole]['xRotation'] = xRotation
                pole_analysis[pole]['yRotation'] = yRotation
                pole_analysis[pole]['xDeflection'] = xDeflection
                pole_analysis[pole]['yDeflection'] = yDeflection
                pole_analysis[pole]['stress'] = Stress

    if outputfile:
        outputdata = {}
        if ANALYSIS == "worst_angle":
            for key in pole_analysis.keys():
                anaglysis_results = pole_analysis[key]
                outputdata[key] = {
                    "worst_angle" : f"{anaglysis_results['worst_angle']} deg",
                    "wind_speed" : f"{anaglysis_results['worst_angle_speed']} m/s",
                }
            df_output = pd.DataFrame.from_dict(outputdata)
            df_output = df_output.T
            df_output.to_csv (outputfile, index = True, header=True)
        elif ANALYSIS == "critical_speed":
            for key in pole_analysis.keys():
                anaglysis_results = pole_analysis[key]
                outputdata[key] = {
                    "critical_wind_speed" : f"{anaglysis_results['critical_wind_speed']} m/s",
                    "wind_direction" : f"{anaglysis_results['critical_wind_direction']} deg",
                }
            df_output = pd.DataFrame.from_dict(outputdata)
            df_output = df_output.T
            df_output.to_csv (outputfile, index = True, header=True)
        elif ANALYSIS == "loading_scenario":
            for key in pole_analysis.keys():
                anaglysis_results = pole_analysis[key]
                ShearForce = np.sqrt(anaglysis_results['xShearForce']**2+anaglysis_results['xShearForce']**2)
                BendingMoment = np.sqrt(anaglysis_results['xBendingMoment']**2+anaglysis_results['yBendingMoment']**2)
                Deflection = np.sqrt(anaglysis_results['xDeflection']**2+anaglysis_results['yDeflection']**2)
                outputdata[f"{key}.analysis_height"] = anaglysis_results['analysis_height'].tolist()
                outputdata[f"{key}.shear_force"] = ShearForce.tolist()
                outputdata[f"{key}.bending_moment"] = BendingMoment.tolist()
                outputdata[f"{key}.pole_deflection"] = Deflection.tolist()
                outputdata[f"{key}.stress"] = anaglysis_results['stress'].tolist()
            df_output = pd.DataFrame.from_dict(outputdata)
            df_output.to_csv (outputfile)
        else:
            error(f"analysis {ANALYSIS} isn't supported",3)

    else:
        if ANALYSIS == "worst_angle":
            for key in pole_analysis.keys():
                anaglysis_results = pole_analysis[key]
                print(anaglysis_results)
                print(f"[{key}] : worst_angle = {anaglysis_results['worst_angle']} deg (wind_speed = {anaglysis_results['worst_angle_speed']} m/s)",file=sys.stdout)
        elif ANALYSIS == "critical_speed":
            for key in pole_analysis.keys():
                anaglysis_results = pole_analysis[key]
                print(f"[{key}] : critical_wind_speed = {anaglysis_results['critical_wind_speed']} m/s (wind_direction = {anaglysis_results['critical_wind_direction']} deg)",file=sys.stdout)
        elif ANALYSIS == "loading_scenario":
            for key in pole_analysis.keys():
                anaglysis_results = pole_analysis[key]
                ShearForce = np.sqrt(anaglysis_results['xShearForce']**2+anaglysis_results['xShearForce']**2)
                BendingMoment = np.sqrt(anaglysis_results['xBendingMoment']**2+anaglysis_results['yBendingMoment']**2)
                Deflection = np.sqrt(anaglysis_results['xDeflection']**2+anaglysis_results['yDeflection']**2)
                print(f"[{key}] : analysis_height = {anaglysis_results['analysis_height']} ft",file=sys.stdout)
                print(f"[{key}] : shear_force = {ShearForce} lbs",file=sys.stdout)
                print(f"[{key}] : bending_moment = {BendingMoment} lb*ft",file=sys.stdout)
                print(f"[{key}] : pole_deflection = {Deflection} ft",file=sys.stdout)
                print(f"[{key}] : stress = {anaglysis_results['stress']} %",file=sys.stdout)
        else:
            error(f"analysis {ANALYSIS} isn't supported",3)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        syntax(1)
    glmfile = None
    options = {}
    debug = False
    for arg in sys.argv[1:]:
        args = arg.split("=")
        if type(args) is list and len(args) > 1:
            token = args[0]
            value = args[1]
        elif type(args) is list:
            token = args[0]
            value = None
        else:
            token = args
            value = None
        if token in ["-h","--help","help"]:
            syntax()
        elif token in ["--debug"]:
            debug = not debug
        elif token.endswith(".glm") or token.endswith(".json"):
            glmfile = token
        elif token.startswith("--"):
            options[token[2:]] = value
        else:
            error(f"'{arg}' is not valid",1)
    if glmfile:
        try:
            main(glmfile,**options)
        except Exception as msg:
            if debug:
                raise
            else:
                error(msg,1)





































