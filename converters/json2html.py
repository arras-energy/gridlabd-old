"""Convert GLM to HTML Folium interactive map

Object properties that affect map generation:

  hidden
  map_color
  map_weight (link only)
  map_opacity (link only)

If defined in the object will be created (if hidden not TRUE) using these properties.
"""
import os, sys, getopt
import json 
import folium
from folium.plugins import MarkerCluster
import numpy

os.putenv(f'PYTHONPATH',sys.argv[0].replace('/json2html.py',''))
from json2html_config import *

data = {}

def warning(msg):
    print(f"WARNING [json2html]: {msg}",file=sys.stderr)

def main(argv):
    global icon_prefix
    global zoomlevel
    global show
    global tiles
    global cluster_ok
    global data

    filename_json = ''
    filename_html = ''
    basename = ''

    def help():
        print("Syntax:")
        print("json2html.py -i|--ifile <input-name> [OPTIONS ...]")
        print("Options:")
        print("  -c|--cluster               : [OPTIONAL] enable cluster markers (default is '%s')" % cluster_ok)
        print("  -g|--glyphs PREFIX         : [OPTIONAL] change the folium glyph prefix (default is '%s')" % icon_prefix)
        print("  -i|--ifile NAME            : [REQUIRED] json input file name.")
        print("  -o|--ofile NAME            : [OPTIONAL] png output file name (default is input <NAME>.png)")
        print("  -s|--show                  : [OPTIONAL] show map in browser (default is '%s')" % show)
        print("  -t|--tiles NAME            : [OPTIONAL] use alternate map tiles (default is '%s')" % tiles)
        print("  -z|--zoom LEVEL            : [OPTIONAL] map initial zoom level (default is '%s')" % zoomlevel)

    try : 
        opts, args = getopt.getopt(sys.argv[1:],"cg:hi:o:st:z:",['cluster',"glyphs=",'help',"ifile=","ofile=",'show',"tiles=","zoomlevel="])
    except getopt.GetoptError:
        sys.exit(2)
    if not opts : 
        help()
        sys.exit(1)
    for opt, arg in opts:
        if opt in ("-c","--cluster"):
            cluster_ok = False
        elif opt in ("-g","--glyphs"):
            icon_prefix = arg
            if arg not in icons.keys():
                raise Exception("glyph '%s' is not a valid folium marker prefix (i.e., %s)" % (arg,", ".join(list(icons.keys()))))
        elif opt in ("-h","--help"):
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
        elif opt in ("-s","--show"):
            show = True
        elif opt in ("-t","--tiles"):
            if not arg in map_tiles.keys():
                raise Exception(f"'%s' is not a valid map tile (i.e., %s)" % (arg,", ".join(map_tiles.keys())))
            tiles = map_tiles[arg]
        elif opt in ("-z","--zoomlevel"):
            zoomlevel = int(arg)
        else:
            raise Exception("'%s' is an invalid command line option" % opt)

    with open(filename_json,'r') as f :
        data = json.load(f)
        assert(data['application']=='gridlabd')
        assert(data['version'] >= '4.2.0')

    lats = []
    lons = []
    tags = []
    for name, values in data['objects'].items():
        try:
            lat = float(values['latitude'])
            lon = float(values['longitude'])
            lats.append(lat)
            lons.append(lon)
            tags.append([(lat,lon),name,values])
        except:
            if "from" in values.keys() and "to" in values.keys():
                tags.append([None,name,values])
            pass
    lats = numpy.array(lats)
    lons = numpy.array(lons)

    try:
        if zoomlevel == 'auto':
            map = folium.Map(location=[lats.mean(),lons.mean()],tiles=tiles)
            map.fit_bounds([[lats.min(),lons.min()],[lats.max(),lons.max()]])
        else:
            map = folium.Map(location=[lats.mean(),lons.mean()],tiles=tiles,zoom_start=zoomlevel)
    except:
        map = folium.Map(tiles=tiles,zoom_start=zoomlevel)
        pass
    try:
        if cluster_ok:
            cluster = MarkerCluster().add_to(map)
        else:
            cluster = map
    except Exception as msg:
        warning(f"marker cluster disabled ({msg})")
        cluster = map
        pass
    for pos, name, tag in tags:
        popup = get_popup(name,tag)
        oclass = tag['class']
        color = get_color(tag)
        if "hidden" in tag.keys() and tag["hidden"] == "TRUE":
            continue
        if icon_prefix in icons.keys():
            if oclass in icons[icon_prefix].keys():
                icon = folium.Icon(icon=icons[icon_prefix][oclass],color=color,prefix=icon_prefix)
            else:
                icon = None
        else:
            icon = folium.Icon(color=color)
        obj = None
        try: # attempt to handle as a link
            from_name = tag['from']
            from_obj = data['objects'][from_name]
            lat0 = float(from_obj['latitude'])
            lon0 = float(from_obj['longitude'])
            to_name = tag['to']
            to_obj = data['objects'][to_name]
            lat1 = float(to_obj['latitude'])
            lon1 = float(to_obj['longitude'])
            pos0 = (lat0,lon0)
            pos1 = (lat1,lon1)
            if pos0 != pos1:
                if "phases" in tag.keys():
                    weight = len(tag['phases'])*2
                    if tag['class'].startswith('underground'):
                        opacity = 0.3
                    else:
                        opacity = 0.7
                else:
                    opacity = 0.5
                    weight = 3
                    if "map_opacity" in tag.keys():
                        opacity = tag["map_opacity"]
                    if "map_weight" in tag.keys():
                        weight = tag["map_weight"]
                obj = folium.PolyLine([pos0,pos1],color=color,weight=weight,opacity=opacity,popup=popup,name=name)
            else:
                obj = folium.Marker(pos,icon=icon,popup=popup,name=name)
        except Exception as err: # apparently not a link, so it's a node or other object
            #print(f"DEBUG: {err}",file=sys.stderr)
            if not icon:
                warning(f"object '{name}' has no known icon (class '{oclass})'")
                icon = folium.Icon(color=color)
            if pos:
                obj = folium.Marker(pos,icon=icon,popup=popup,name=name)
        if obj:
            obj.add_to(cluster)
    if mouseposition:
        folium.plugins.MousePosition(auto_start=True,position=mouseposition).add_to(map)
    if search:
        if map != cluster:
            folium.plugins.Search(cluster,search_label='name').add_to(map)
        else:
            warning(f"cannot use search feature with clustering disabled")
    if geocoder:
        folium.plugins.Geocoder().add_to(map)
    if measurecontrol:
        folium.plugins.MeasureControl().add_to(map)
    if zoomtoggle:
        folium.plugins.ScrollZoomToggler().add_to(map)
    map.save(filename_html)
    if show:
        os.system(f"{show_command} {filename_html}")

def get_voltage_color(V,VN):
    VM = abs(V)/VN
    if VM == 0.0:
        return voltage_colors['zero']
    elif VM < low_voltage:
        return voltage_colors['low']
    elif VM > high_voltage:
        return voltage_colors['high']
    else:
        return voltage_colors['normal']

def get_current_color(C,C0,C1):
    CM = abs(C)
    if CM == 0.0:
        return current_colors['zero']
    elif CM < C0:
        return current_colors['normal']
    elif CM < C1:
        return current_colors['continuous']
    else:
        return current_colors['emergency']

def get_color(tag):

    # direct color assignment
    if "map_color" in tag.keys():
        return tag["map_color"]

    # try node scheme first
    try:
        VN = float(tag['nominal_voltage'].split()[0])
        VA = complex(tag['voltage_A'].split()[0])
        VB = complex(tag['voltage_B'].split()[0])
        VC = complex(tag['voltage_C'].split()[0])
        PH = tag['phases']
        color = voltage_colors['normal']
        if 'A' in PH and color == voltage_colors['normal']:
            color = get_voltage_color(VA,VN)
        if 'B' in PH and color == voltage_colors['normal']:
            color = get_voltage_color(VB,VN)
        if 'C' in PH and color == voltage_colors['normal']:
            color = get_voltage_color(VC,VN)
        # print("get_color():",tag['class'],tag['id'],"-->",PH,VA,VB,VC,VN,"-->",color)
        return color
    except:
        pass

    # try link scheme
    try:
        C0 = float(tag['continuous_rating'].split()[0])
        C1 = float(tag['emergency_rating'].split()[0])
        CA = complex(tag['current_in_A'].split()[0])
        CB = complex(tag['current_in_B'].split()[0])
        CC = complex(tag['current_in_C'].split()[0])
        PH = tag['phases']
        color = current_colors['normal']
        if 'A' in PH and color == current_colors['normal']:
            color = get_current_color(CA,C0,C1)
        if 'B' in PH and color == current_colors['normal']:
            color = get_current_color(CB,C0,C1)
        if 'C' in PH and color == current_colors['normal']:
            color = get_current_color(CC,C0,C1)
        # print("get_color():",tag['class'],tag['id'],"-->",PH,CA,CB,CC,C0,C1,"-->",color)
        return color
    except Exception as err:
        pass

    # try pole scheme
    try:
        return pole_colors[tag['status']]
    except:
        pass
    return 'gray'

def get_popup(name,tag):
    style = '<STYLE>#box{overflow:scroll;height:20em}</STYLE>'
    popup = f'{style}<DIV ID="box"><TABLE><CAPTION>{name}</CAPTION>\n'
    popup += '<TR><TH><HR/></TH><TD><HR/></TD></TR>'
    for item, value in tag.items():
        if item in hidden_properties:
            continue
        label = item.title().replace("_","&nbsp;")
        try:
            module = data['classes'][value]['module']
            value = f'<A TARGET="_blank" HREF="https://docs.gridlabd.us/index.html?owner=slacgismo&project=gridlabd&branch=master&folder=/Module/Powerflow&doc=/Module/{module.title()}/{value.title()}.md">{value}</A>'
        except:
            pass
        popup += f'<TR><TH>{label}</TH><TD>&nbsp;</TD><TD>{value}</TD></TR>\n'
    popup += '<TR><TH><HR/></TH><TD><HR/></TD></TR>'
    popup += '</TABLE></DIV>\n'
    return popup

if __name__ == '__main__':
    main(sys.argv)
