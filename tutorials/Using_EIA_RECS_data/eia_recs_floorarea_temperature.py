import sys, json, math
th = []
tc = []
with open(sys.argv[0].replace(".py",".json"),"r") as f:
    glm = json.load(f)
    for name, data in glm["objects"].items():
        th.append(float(data["heating_setpoint"].split()[0]))
        tc.append(float(data["cooling_setpoint"].split()[0]))

import matplotlib.pyplot as plt
plt.hist2d(x=tc,y=th,bins=15,range=[[65,80],[65,80]])
plt.xlabel("Cooling setpoint (degF)")
plt.ylabel("Heating setpoint (degF)")
plt.title(f"Thermostat (N={len(glm['objects'].keys())})")
plt.colorbar()
plt.xticks(range(65,81,1))
plt.yticks(range(65,81,1))
plt.savefig(sys.argv[0].replace(".py",".png"))