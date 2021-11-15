"""Create a load model from load data

SYNTAX

  $ gridlabd load_model -p|--power=POWERCSV -w|--weather=WEATHERCSV -o|--output=OUTPUTFILE
        -n|--name=LOADNAME -t|--type=LOADTYPE [OPTIONS ...]

DESCRIPTION

The `load_model` utility creates a load model from load and weather data by fitting
a Nth-order transfer functions to the data using a least-squares linear regressions
of the equation

    P0 = -a1*P1 - ... -aN*PN + b0*T0 + ... + bN*TN + c0*S0 + ... cN*SN

where

    P0 is the power at time t
    P1 is the power at time t-1
    PN is the power at time t-N
    T0 is the temperature at time t
    TN is the temperature at time t-N
    S0 is the solar radiation at time t
    SN is the solar radiation at time t-N
    a, b, and c are the coefficient of the transfer functions


"""

import os, sys, warnings
import numpy as np
import pandas as pd
from datetime import datetime
import random

K = 24
name = "load"
modeltype = "filter"
phases = "ABC"
nominal_voltage = 120.0

def identify(Y,X,K):
    """Fit a discrete-time MISO transfer function to data
    
    Fits a discrete-time transfer function of the form
    
        Y(z)   b0 + b1*z^-1 + ... + bK z^-K
        ---- = -----------------------------
        X(z)    1 + a1*z^-1 + ... + bK z^-K
    
    to known values of X and Y.
    
    If the hold-out size N is positive, then the model is fit
    to L-N data values, where L is the length of X.  The hold-out
    test vector Z is then computed on the N last rows of X and Y.
    
    ARGUMENTS
    
        Y is the output (LxK)
        
        X are the inputs (LxKxM) where M is the number of input 
          variables
        
        K is the desired order of the transfer function
        
    RETURNS
    
        x,M where x = [a1,...,aK,b0,...bN] and N = columns of M
                
    where

        M is the historical data assembled from X and Y
        
        x is the model
        
        Z is the hold out data (if any)
    """
    L = len(Y)
    M = np.hstack([-np.hstack([Y[n:L-K+n] for n in range(K)]),
                   +np.hstack([X[n+1:L-K+n+1] for n in range(K)])])
    Mt = M.transpose()
    x = np.linalg.solve(Mt*M,Mt*Y[K:])
    return x,M

class convert:
    def power(x):
        try:
            return round(float(x),1)
        except:
            return float('NAN')
    def timestamp(x):
        return datetime.strptime(x,"%m/%d/%y %H:%M")
    def temperature(x):
        try:
            return round(float(x),1)
        except:
            return float('NAN')
    def solar(x):
        try:
            return round(float(x),1)
        except:
            return float('NAN')

    def datetime(x):
        return datetime.strptime(x,"%Y-%m-%d %H:%M:%S")

if __name__ == '__main__':

    basename = os.path.basename(sys.argv[0]).replace('.py','')
    def error(msg,code=None):
        if code != None:
            print(f"ERROR [{basename}]: {msg}",file=sys.stderr)
            exit(code)
        else:
            raise Exception(msg)
    def syntax(code=0):
        if code == 0:
            print(__doc__)
        else:
            print(f"Syntax: {basename} -p|--power=POWERCSV -w|--weather=WEATHERCSV [-g|--glm=GLMNAME] [-n|--name=LOADNAME] [-t|--type=LOADTYPE] [--test] [-h|--help|help] [OPTIONS ...]")
        exit(code)

    power = None
    weather = None
    output_file = None
    for arg in sys.argv[1:]:
        args = arg.split("=")
        if type(args) is list and len(args) > 1:
            token = args[0]
            value = args[1]
        elif type(args) is list:
            token = args[0]
            value = None
        else:
            token = args
            value = None
        if token in ["-h","--help","help"]:
            syntax()
        elif token in ["-p","--power"]:
            power = value
        elif token in ["-w","--weather"]:
            weather = value
        elif token in ["-o","--output"]:
            output_file = value
        elif token in ["-n","--name"]:
            name = value
        elif token in ["--order"]:
            K = int(value)
        elif token in ["-t","--type"]:
            modeltype = value
        else:
            error(f"option '{token}' is not valid",1)
    if power and weather:
        power_data = pd.read_csv(power)
        ndx = list(map(lambda t:datetime(t.year,t.month,t.day,t.hour),pd.DatetimeIndex(power_data[power_data.columns[0]])))
        data = pd.DataFrame(power_data.groupby(ndx)[power_data.columns[1]].mean())
        data.index.name = "datetime"
        weather_data = pd.read_csv(weather)
        weather_data.index.name = "datetime"
        ndx = list(map(lambda t:datetime(t.year,t.month,t.day,t.hour),pd.DatetimeIndex(weather_data[weather_data.columns[0]])))
        data = weather_data.groupby(ndx).mean().join(data).dropna()
        data.index.name = "datetime"
        P = np.matrix(data["real_power"]).transpose()
        T = np.matrix(data["temperature"]).transpose()
        S = np.matrix(data["solar"]).transpose()
        W = np.matrix(np.floor(data.index.weekday/5)).transpose()
        # Y = np.matrix((data.index-data.index.min()).seconds/86400/365.24).transpose()
        X = np.hstack([T,S,W])        
        x,M = identify(P,X,K)
        if not output_file:
            output = sys.stdout
        else:
            output = open(output_file,"w")
        if output_file.endswith(".glm"):
            print(f"// generated from 'gridlabd {basename}'",file=output)
            print(f"// x={x.transpose().tolist()[0]}",file=output)
            if modeltype == "filter":
                print(f"// x={x.transpose().tolist()[0]}",file=output)

                print(f"filter {name}_temperature(z,timestep=1h) = (",file=output,end="")
                print(f"{x[K,0]:+f}",file=output,end="")            
                for k in range(1,K+1):
                    print(f"{x[k+K,0]:+f}*z^{-k:.0f}",file=output,end="")            
                print(") / (1",file=output,end="")
                for k in range(0,K):
                    print(f"{x[k,0]:+f}*z^{-k-1:.0f}",file=output,end="")
                print(")",file=output)

                print(f"filter {name}_solar(z,timestep=1h) = (",file=output,end="")
                print(f"{x[2*K+1,0]:+f}",file=output,end="")            
                for k in range(1,K+1):
                    print(f"{x[k+2*K+1,0]:+f}*z^{-k:.0f}",file=output,end="")            
                print(") / (1",file=output,end="")
                for k in range(0,K):
                    print(f"{x[k,0]:+f}*z^{-k-1:.0f}",file=output,end="")
                print(")",file=output)

                print(f"filter {name}_weekend(z,timestep=1h) = ",file=output,end="")
                print(f"{x[3*K+2,0]:+f}",file=output,end="")            
                print(" / (1",file=output,end="")
                for k in range(0,K):
                    print(f"{x[k,0]:+f}*z^{-k-1:.0f}",file=output,end="")
                print(")",file=output)
                weather_name = f"weather_{random.randint(1e15,1e16)}"
                print("class","weather","{",file=output)
                print("   ","double",data.columns[0],file=output,end=";\n")
                print("   ","double",data.columns[1],file=output,end=";\n")
                print("}",file=output)
                print("object weather {",file=output)
                print("   ","name",weather_name,file=output,end=";\n")
                print("   ","object","player","{",file=output)
                print("   ","   ","file",f"\"{weather}\"",file=output,end=";\n")
                print("   ","}",file=output,end=";\n")
                print("}",file=output)
                print("module","powerflow",file=output,end=";\n")
                print("object load {",file=output)
                print("   ","phases",phases,file=output,end=";\n")
                print("   ","nominal_voltage",nominal_voltage,"V",file=output,end=";\n")
                for source in ["temperature","solar","weekend"]:
                    print("   ","object","load","{",file=output)
                    if "D" in phases:
                        print("   ","   ","voltage_AB",f"0.333*{name}_{source}",file=output,end=";\n")
                        print("   ","   ","voltage_BC",f"0.333*{name}_{source}",file=output,end=";\n")
                        print("   ","   ","voltage_CA",f"0.333*{name}_{source}",file=output,end=";\n")
                    else:
                        print("   ","   ","voltage_A",f"0.333*{name}_{source}",file=output,end=";\n")
                        print("   ","   ","voltage_B",f"0.333*{name}_{source}",file=output,end=";\n")
                        print("   ","   ","voltage_C",f"0.333*{name}_{source}",file=output,end=";\n")
                    print("   ","   ","}",file=output,end=";\n")
                print("}",file=output)
            elif modeltype == "player":
                print()
            else:
                error(f"modeltype '{modeltype}' is not valid",2)
        else:
            error(f"output file '{output_file}' is not a support file format",3)
    else:
        syntax(1)    