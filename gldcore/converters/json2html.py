import os, sys, getopt
import json 
import folium
import numpy

icons = {
    "node" : {"icon":"circle","prefix":"fa"},
    "load" : {"icon":"angle-double-down","prefix":"fa"},
    "overhead_line" : {"icon":"bars","prefix":"fa"},
    "underground_line" : {"icon":"bars","prefix":"fa"},
    "triplex_line" : {"icon":"grip-lines","prefix":"fa"},
    "triplex_load" : {"icon":"angle-double-down","prefix":"fa"},
    "regulator" : {"icon":"adjust","prefix":"fa"},
    "switch" : {"icon":"square","prefix":"fa"},
    "transformer" : {"icon":"squarespace","prefix":"fa"},
    "meter" : {"icon":"clock","prefix":"fa"},
    "capacitor" : {"icon":"battery-half","prefix":"fa"}
}
voltage_colors = {
    "zero" : "black",
    "low" : "blue",
    "normal" : "lightgreen",
    "high" : "red"
}
low_voltage = 0.95
high_voltage = 1.05
def get_voltage_color(V,VN):
    VM = abs(V)/VN
    if VM == 0.0:
        return voltage_colors["zero"]
    elif VM < low_voltage:
        return voltage_colors["low"]
    elif VM > high_voltage:
        return voltage_colors["high"]
    else:
        return voltage_colors["normal"]

current_colors = {
    "zero" : "black",
    "normal" : "lightgreen",
    "continuous" : "yellow",
    "emergency" : "red",
}
def get_current_color(C,C0,C1):
    CM = abs(C)
    if CM == 0.0:
        return current_colors["zero"]
    elif CM < C0:
        return current_colors["normal"]
    elif CM < C1:
        return current_colors["continuous"]
    else:
        return current_colors["emergency"]

def get_color(tag):
    # try node scheme first
    try:
        VN = float(tag["nominal_voltage"].split()[0])
        VA = complex(tag["voltage_A"].split()[0])
        VB = complex(tag["voltage_B"].split()[0])
        VC = complex(tag["voltage_C"].split()[0])
        PH = tag["phases"]
        color = voltage_colors["normal"]
        if "A" in PH and color == voltage_colors["normal"]:
            color = get_voltage_color(VA,VN)
        if "B" in PH and color == voltage_colors["normal"]:
            color = get_voltage_color(VB,VN)
        if "C" in PH and color == voltage_colors["normal"]:
            color = get_voltage_color(VC,VN)
        # print("get_color():",tag["class"],tag["id"],"-->",PH,VA,VB,VC,VN,"-->",color)
        return color
    except:
        pass

    # try link scheme
    try:
        C0 = float(tag["continuous_rating"].split()[0])
        C1 = float(tag["emergency_rating"].split()[0])
        CA = complex(tag["current_in_A"].split()[0])
        CB = complex(tag["current_in_B"].split()[0])
        CC = complex(tag["current_in_C"].split()[0])
        PH = tag["phases"]
        color = current_colors["normal"]
        if "A" in PH and color == current_colors["normal"]:
            color = get_current_color(CA,C0,C1)
        if "B" in PH and color == current_colors["normal"]:
            color = get_current_color(CB,C0,C1)
        if "C" in PH and color == current_colors["normal"]:
            color = get_current_color(CC,C0,C1)
        # print("get_color():",tag["class"],tag["id"],"-->",PH,CA,CB,CC,C0,C1,"-->",color)
        return color
    except Exception as err:
        pass

    return "gray"

def main(argv):
    filename_json = ''
    filename_html = ''
    basename = ''
    output_type = 'oneline'
    zoomlevel = 'auto'
    show = False
    tiles = "OpenStreet"

    def help():
        print('Syntax:')
        print('json2html.py -i|--ifile <input-name> [-o|--ofile <output-name>] [-z|--zoom <zoomlevel>] [-s|--show] [-t|--tiles <name>')
        print('  -i|--ifile                 : [REQUIRED] json input file name.')
        print('  -o|--ofile                 : [OPTIONAL] png output file name (default is <input-name>.png)')
        print('  -z|--zoom <level>          : [OPTIONAL] map initial zoom level (default is "%s")' % zoomlevel)
        print('  -s|--show                  : [OPTIONAL] show map in browser (default is "%s")' % show)
        print('  -t|--tiles <name>          : [OPTIONAL] use alternate map tiles (default is "%s")' % tiles)

    try : 
        opts, args = getopt.getopt(sys.argv[1:],"hi:o:z:st:",["help","ifile=","ofile=","zoomlevel=","show","tiles="])
    except getopt.GetoptError:
        sys.exit(2)
    if not opts : 
        help()
        sys.exit(1)
    for opt, arg in opts:
        if opt in ("-h","--help"):
            help()
            sys.exit(0)
        elif opt in ("-i", "--ifile"):
            filename_json = arg
            if filename_html == '':
                if filename_json[-5:] == ".json":
                    basename = filename_json[:-5]
                else: 
                    basename = filename_json
                filename_html = basename + ".html"
        elif opt in ("-o", "--ofile"):
            filename_png = arg
        elif opt in ("-z","--zoomlevel"):
            zoomlevel = int(arg)
        elif opt in ("-s","--show"):
            show = True
        elif opt in ("-t","--tiles"):
            tiles = arg
        else:
            raise Exception("'%s' is an invalid command line option" % opt)

    with open(filename_json,"r") as f :
        data = json.load(f)
        assert(data['application']=='gridlabd')
        assert(data['version'] >= '4.2.0')

    lats = []
    lons = []
    tags = []
    for name, values in data["objects"].items():
        try:
            lat = float(values["latitude"])
            lon = float(values["longitude"])
            lats.append(lat)
            lons.append(lon)
            tags.append([(lat,lon),name,values])
        except:
            pass
    lats = numpy.array(lats)
    lons = numpy.array(lons)

    if zoomlevel == "auto":
        map = folium.Map(location=[lats.mean(),lons.mean()],tile=tiles)
        map.fit_bounds([[lats.min(),lons.min()],[lats.max(),lons.max()]])
    else:
        map = folium.Map(location=[lats.mean(),lons.mean()],tile=tiles,zoom_start=zoomlevel)
    for pos, name, tag in tags:
        style = "<STYLE>#box{overflow:scroll;height:20em}</STYLE>"
        popup = f"{style}<DIV ID=\"box\"><TABLE><CAPTION>{name}</CAPTION>\n"
        popup += "<TR><TH><HR/></TH><TD><HR/></TD></TR>"
        for item, value in tag.items():
            popup += f"<TR><TH>{item}</TH><TD>{value}</TD></TR>\n"
        popup += "<TR><TH><HR/></TH><TD><HR/></TD></TR>"
        popup += f"</TABLE></DIV>\n"
        oclass = tag["class"]
        color = get_color(tag)
        if oclass in icons.keys():
            icon = folium.Icon(**icons[oclass],color=color)
        else:
            icon = folium.Icon(icon="none",color=color)
        folium.Marker(pos,icon=icon,popup=popup).add_to(map)
    map.save(filename_html)
    if show:
        os.system(f"open {filename_html}")

if __name__ == '__main__':
    main(sys.argv.extend(["-i","autotest/IEEE-123.json","-s"]))
