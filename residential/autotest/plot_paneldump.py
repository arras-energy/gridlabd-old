import pandas as pd
import matplotlib.pyplot as plt

panel = pd.read_csv("paneldump.csv")

gains = 0.0
for enduse in panel["enduse"].unique():
    data = panel[panel["enduse"]==enduse]
    subtotal = data["real[kWh]"].mean()*24
    if enduse != "HVAC":
        gains += subtotal
    print("%-15.15s %.2f kWh/d" % (enduse,subtotal))
    plt.figure(1)
    plt.clf()
    data.plot()
    plt.savefig("%s.png" % enduse.lower())

print("%-15.15s %.2f kWh/d" % ("GAINS",gains))
