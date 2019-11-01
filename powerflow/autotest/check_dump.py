#!/usr/local/bin/python3
import pandas as pd 

bus = pd.read_csv("solver_ml_bus.csv")
assert((bus["EOL"]=="EOL").all())

branch = pd.read_csv("solver_ml_branch.csv")
assert((branch["EOL"]=="EOL").all())

print(bus)
print(branch)