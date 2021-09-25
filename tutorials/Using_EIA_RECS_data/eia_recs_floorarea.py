import sys, json
area = []
with open(sys.argv[0].replace(".py",".json"),"r") as f:
    glm = json.load(f)
    for name, data in glm["objects"].items():
        area.append(float(data["floor_area"].split()[0]))
import matplotlib.pyplot as plt
plt.hist(area,20,range=(0,10000))
plt.xlabel("Floor area (sf)")
plt.ylabel("Number of homes")
plt.title(f"Floor areas (N={len(area)})")
plt.grid()
plt.savefig(sys.argv[0].replace(".py",".png"))