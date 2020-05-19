import sys
import pandas as pd 

def solve(gridlabd,**kwargs):
    """solve(gridlabd,model)
    
    Delivers a model to solve.  If the solution is found, then set
    model['iterations'] to the number of iterations required to obtain
    the solution.  If the solution is not found, then set model['iterations']
    to -1.

    The following arrays are set

        bustags (dict) - dict of columns names to the column numbers in
                         busdata

        busdata (dict) - dict of bus names to the bus data

        branchtags (dict) - dict of columns names to the columns numbers in
                            branchdata

        branchdata (dict) - dict of branch names to the branch data

    Return: None 
    """
    print(f"solving at {gridlabd.get_global('clock')} for data {kwargs}")
    kwargs['iterations'] = 0

def learn(gridlabd,**kwargs):
    print(f"learning {kwargs}")
    return None

def check_dumps(gridlabd):

    # print(f"dumping {gridlabd.get_global('modelname')} at {gridlabd.get_global('clock')}")
    bus = pd.read_csv("solver_bus.csv")
    assert((bus["EOL"]=="EOL").all())

    branch = pd.read_csv("solver_branch.csv")
    assert((branch["EOL"]=="EOL").all())
