import sys
import pandas as pd 
import pprint as pp

def solve(gridlabd,**kwargs):
    """solve(gridlabd,model)
    
    Delivers a model to solve.  

    The following data is given in kwargs

        bustags (dict) - dict of columns names to the column numbers in
                         busdata

        busdata (dict) - dict of bus id to the bus data

        branchtags (dict) - dict of columns names to the columns numbers in
                            branchdata

        branchdata (dict) - dict of branch id to the branch data

    Return: negative for failure
            positive for successful iteration count 
            0 for successful with no iteration done
    """
    # print(f"solving at {gridlabd.get_global('clock')}")
    # print(kwargs)
    print(f"Bus tags: {kwargs['bustags']}")
    print(f"Bus data: {kwargs['busdata']}")
    # print(f"Branch tags: {kwargs['branchtags']}")
    # print(f"Branch data: {kwargs['branchdata']}")
    # try:
    #     print(type(kwargs['busdata'][0][0]))
    # except:
    #     pass
    return -1

def learn(gridlabd,**kwargs):
    # print(f"learning {kwargs}")
    return None

def check_dumps(gridlabd):

    # print(f"dumping {gridlabd.get_global('modelname')} at {gridlabd.get_global('clock')}")
    bus = pd.read_csv("solver_bus.csv")
    assert((bus["EOL"]=="EOL").all())

    branch = pd.read_csv("solver_branch.csv")
    assert((branch["EOL"]=="EOL").all())
