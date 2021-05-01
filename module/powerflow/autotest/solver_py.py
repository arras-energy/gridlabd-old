import sys, io
import pandas as pd 
import pprint as pp
import datetime as dt

def solve(gridlabd,**kwargs):
    """solve(gridlabd,kwargs)
    
    Delivers a model to solve.  

    The following data is given in kwargs

        bustags (dict) - dict of columns names to the column numbers in
                         busdata

        busdata (dict) - dict of bus id to the bus data

        branchtags (dict) - dict of columns names to the columns numbers in
                            branchdata

        branchdata (dict) - dict of branch id to the branch data

        options (dict) - dict of options defined in solver_py.conf file

    Return: negative for failure
            positive for successful iteration count 
            0 for successful with no iteration done
    """
    sys.stdout = output_stream
    sys.stderr = error_stream
    if 'options' in kwargs.keys() and 'dump' in kwargs['options']:
        if kwargs['options']['dump'] == 'dataframe':
            branch = pd.DataFrame(data=kwargs['branchdata'],index=kwargs['branchtags'])
            bus = pd.DataFrame(data=kwargs['busdata'],index=kwargs['bustags'])
            print(branch)
            print(bus)
        elif kwargs['options']['dump'] == 'pretty':
            pp.PrettyPrinter(indent=4,stream=output_stream).pprint(kwargs)
        elif kwargs['options']['dump'] == 'print':
            print(kwargs)
    return -1 

def learn(gridlabd,**kwargs):
    """learn(gridlabd,kwargs)
    
    Delivers a model to solve.  

    The following data is given in kwargs

        bustags (dict) - dict of columns names to the column numbers in
                         busdata

        busdata (dict) - dict of bus id to the bus data

        branchtags (dict) - dict of columns names to the columns numbers in
                            branchdata

        branchdata (dict) - dict of branch id to the branch data

        options (dict) - dict of options defined in solver_py.conf file

        iterations (int) - number of iterations used to find a new solution

        powerflow_type (int) - id of powerflow solution type (0=steady, 1=dynamic init, 2=dynamic transient)

        bad_computations (bool) - flag to indicate a solver error was encountered

        

    Return: negative for failure
            positive for successful iteration count 
            0 for successful with no iteration done
    """
    sys.stdout = output_stream
    sys.stderr = error_stream
    if 'options' in kwargs.keys() and 'dump' in kwargs['options']:
        if kwargs['options']['dump'] == 'dataframe':
            branch = pd.DataFrame(data=kwargs['branchdata'],index=kwargs['branchtags'])
            bus = pd.DataFrame(data=kwargs['busdata'],index=kwargs['bustags'])
            print(branch)
            print(bus)
        elif kwargs['options']['dump'] == 'pretty':
            pp.PrettyPrinter(indent=4,stream=output_stream).pprint(kwargs)
        elif kwargs['options']['dump'] == 'print':
            print(kwargs)
    return None
