import sys, json, math
area = []
with open(sys.argv[0].replace(".py",".json"),"r") as f:
    glm = json.load(f)
    area = {"LITTLE":[],"NORMAL":[],"GOOD":[]}
    for name, data in glm["objects"].items():
        area[data["thermal_integrity_level"]].append(float(data["floor_area"].split()[0]))
import matplotlib.pyplot as plt
values = list(area.values())
keys = list(area.keys())
plt.hist(values,20,range=(0,10000),label=keys,stacked=True)
plt.xlabel("Floor area (sf)")
plt.ylabel("Number of homes")
plt.title(f"Floor areas (N={len(glm['objects'].keys())})")
plt.grid()
plt.legend(keys)
plt.savefig(sys.argv[0].replace(".py",".png"))