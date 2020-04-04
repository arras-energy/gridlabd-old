import pandas as pd 

def check_dumps(gridlabd):

    bus = pd.read_csv("solver_bus.csv")
    assert((bus["EOL"]=="EOL").all())

    branch = pd.read_csv("solver_branch.csv")
    assert((branch["EOL"]=="EOL").all())
