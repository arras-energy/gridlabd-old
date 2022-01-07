from datetime import datetime
class converters:

    def real(x):
        try:
            return float(x)
        except:
            return float('NAN')
    
    def timestamp(x):
        return datetime.strptime(x,"%m/%d/%y %H:%M")
    
    def datetime(x):
        return datetime.strptime(x,"%Y-%m-%d %H:%M:%S")

    outputs = {"timestamp":timestamp,"real_power":real}
    inputs = {"datetime":datetime,"temperature":real,"solar":real}

