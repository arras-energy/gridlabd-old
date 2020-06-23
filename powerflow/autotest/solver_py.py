import sys, io
import pandas as pd 
import pprint as pp
import datetime as dt

def pheading(label,tagwidth=16,width=8,columns=range(5)):
    heading = f'{label:{tagwidth}}'
    for n in columns :
        heading = heading + f" {n:{width}}"
    print(heading)
    heading = '-'*tagwidth 
    for n in columns :
        heading = heading + ' ' + '-'*width
    print(heading)

def parray(name,values,tagwidth=16,width=8,prec=2,columns=range(5)) :
    row = f"{name:{tagwidth}}"
    for n in columns :
        value = values[n]
        if type(value) is float:
            row = row + ' ' + f'{values[n]:{width}.{prec}}'
        elif value:
            row = row + ' ' + f'{values[n]:{width}}'
        else:
            row = row + ' '*(width) + '-'
    print(row)

def pbus(bustags,busdata,tagwidth=16,width=8,prec=2,columns=range(5)):
    pheading('\nbus',tagwidth,width,columns)
    for tag,key in bustags.items():
        parray(tag,busdata[key],tagwidth,width,prec,columns)

def pbranch(branchtags,branchdata,tagwidth=16,width=8,prec=2,columns=range(5)):
    pheading('\nbranch',tagwidth,width,columns)
    for tag,key in branchtags.items():
        parray(tag,branchdata[key],tagwidth,width,prec,columns)

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
    branch = pd.DataFrame(data=kwargs['branchdata'],index=kwargs['branchtags'])
    bus = pd.DataFrame(data=kwargs['busdata'],index=kwargs['bustags'])
    print(branch)
    print(bus)
    return -1

def learn(gridlabd,**kwargs):
    branch = pd.DataFrame(data=kwargs['branchdata'],index=kwargs['branchtags'])
    bus = pd.DataFrame(data=kwargs['busdata'],index=kwargs['bustags'])
    print(branch)
    print(bus)
    return None
