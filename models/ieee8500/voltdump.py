import os
import csv
import datetime
import re
import math
import cmath
import glmptime as glmptime

data = {}
nodes = ["Timestamp"]
lastnodes = []
timestamp = None
timezone = "UTC"
with open('output/volt_dump.csv', 'r') as dumpfile:
    print("Reading volt_dump...")
    reader = csv.reader(dumpfile)
    for row in reader :
        if row[0].startswith("#") :
            tpos = row[0].find(" at ")
            if tpos > 0 :
                timestamp = row[0][tpos+4:tpos+27]
                timestamp = glmptime.glmptime(timestamp)
                data[timestamp] = []
                timezone = row[0][tpos+24:tpos+27]
            header = []
        elif not header :
            header = row
            assert(header==['node_name', 'voltA_real', 'voltA_imag', 'voltB_real', 'voltB_imag', 'voltC_real', 'voltC_imag'])
            if lastnodes :
                assert(lastnodes==nodes)
            elif nodes :
                lastnodes = nodes
        else :
            try :
                node = row[0]
                A = complex(float(row[1]),float(row[2]))
                B = complex(float(row[3]),float(row[4]))
                C = complex(float(row[5]),float(row[6]))
                if not node+"_A" in nodes :
                    nodes.extend([node+"_A",node+"_B",node+"_C"])
                data[timestamp].extend([A,B,C])
            except :
                print("ERROR: ignored row '%s'" % row)

with open('output/voltages.csv','w') as voltages:
    print("Writing voltages...")
    writer = csv.writer(voltages)
    writer.writerow(nodes)
    for key in sorted(data.keys()) :
        row = [key.strftime("%Y-%m-%dT%H:%M:%S%z")]
        for value in data[key]:
            row.append("%g%+gd" % (abs(value),(cmath.phase(value))*180/3.1415926))
            #row.append("%g%+gd" % (value.real,value.imag))
        writer.writerow(row)



headers = ["Timestamp"]
data = {}
timestamp = []
# timestamp_current = []
re_complex = re.compile("([+-][0-9]*\\.?[0-9]+|[+-][0-9]+.[0-9]+[eE][0-9]+)([+-][0-9]*\\.?[0-9]+|[+-][0-9]+.[0-9]+[eE][0-9]+)([ijdr])")
def to_complex(s) :
    r = re.split(re_complex,s)
    if type(r) is list and len(r) > 4 :
        if r[3] == 'd' :
            m = float(r[1])
            a = float(r[2])*3.1415926/180.0
            return complex(m*math.cos(a),m*math.sin(a))
        elif r[3] == 'r' :
            m = float(r[1])
            a = float(r[2])
            return complex(m*math.cos(a),m*math.sin(a))
    try :
        return complex(s)
    except :
        raise Exception("complex('%s') is not valid" % s)


for filename in os.listdir("output") :
    if filename.startswith("power_dump_") :
        with open("output/"+filename,"r") as dumpfile :
            print("Timestamp Read %s..." % filename)
            reader = csv.reader(dumpfile)
            for row in reader:
                if row[0][0] == '#' :
                    if row[0]=="# timestamp" :
                        headers.extend(row[1:])
                    continue
                timestamp = glmptime.glmptime(row[0])
                if not timestamp in data.keys() :
                    data[timestamp] = []
                try :
                    data[timestamp].extend(list(map(lambda x:to_complex(x),row[1:])))
                except:
                    print("%s: error parsing row '%s', values ignored" % (filename,row))

with open("output/powers.csv","w") as powers:
    print("Writing powers...")
    writer = csv.writer(powers)
    writer.writerow(headers)
    for key,values in data.items() :
        data = [key.strftime("%Y-%m-%dT%H:%M:%S%z")]
        for value in values:
            #data.append("%g%+gj" % (value.real,value.imag))
            data.append("%g%+gd" % (abs(value),(cmath.phase(value))*180/3.1415926))
        writer.writerow(data)
