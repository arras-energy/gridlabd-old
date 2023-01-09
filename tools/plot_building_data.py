import pandas
import matplotlib.pyplot as plt
import numpy

PARAMETERS = {
    "UA" : "W/K",
    "UI" : "W/K",
    "UM" : "W/K",
    "CM" : "J/K",
    "CA" : "J/K",
}

COUNTY = "CA_San_Mateo"

building_data = pandas.read_csv(f"{COUNTY}.csv",index_col=["building_type"])

for PARAMETER,PARAMETER_UNIT in PARAMETERS.items():
    plt.figure(figsize=(10,7))
    for building_type in building_data.index.unique():
        x = building_data.loc[building_type,"floor_area"]
        y = building_data.loc[building_type,PARAMETER]
        yx = numpy.log(y)/numpy.log(x)
        gm = numpy.exp(yx.mean())
        gs = numpy.exp(yx.std())
        plt.loglog(x,y,".",label=f"{building_type} ({gm:.3f}$\\pm${gs*3:.3f} {PARAMETER_UNIT}/m$^2$)")
    plt.xlabel('Floor area (m$^2$)')
    plt.ylabel(f'{PARAMETER} ({PARAMETER_UNIT})')
    plt.grid()
    plt.title(f"{COUNTY} Building {PARAMETER}")
    plt.legend()
    plt.savefig(f"building_data_{PARAMETER}.png")
