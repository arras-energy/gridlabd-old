import json 
import os 
import sys, getopt
from datetime import datetime 
from PIL import Image, ImageDraw, ImageFont

def main(argv):
    filename_json = ''
    filename_png = ''
    basename = ''
    output_type = 'oneline'
    with_nodes = False
    resolution = "300"
    pagesize = "640x480"
    limit = None

    def help():
        print('Syntax:')
        print('json2png-oneline.py -i|--ifile <input-name> [-o|--ofile <output-name>] [<options> ...]')
        print('  -i|--ifile                 : [REQUIRED] json input file name.')
        print('  -o|--ofile                 : [OPTIONAL] png output file name (default is <input-name>.png)')
        print('  -s|--size <width>x<height> : [OPTIONAL] image size in pixels (default is "%s")' % size)
        print('  -d|--dpi <resolution>      : [OPTIONAL] image resolution in dots per inch (default is "%s")' % resolution)

    try : 
        opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:d:l:s:",["help","ifile=","ofile=","type=","with-nodes","dpi=","limit=","size="])
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
            if filename_png == '':
                if filename_json[-5:] == ".json":
                    basename = filename_json[:-5]
                else: 
                    basename = filename_json
                filename_png = basename + ".png"
        elif opt in ("-o", "--ofile"):
            filename_png = arg
        elif opt in ("-d","--dpi"):
            resolution = arg
        elif opt in ("-s","--size"):
            pagesize = arg
        elif opt in ("-t","--type"):
            assert(output_type=="oneline")
        else:
            raise Exception("'%s' is an invalid command line option" % opt)

    with open(filename_json,"r") as f :
        data = json.load(f)
        assert(data['application']=='gridlabd')
        assert(data['version'] >= '4.2.0')

    modelname = data["globals"]["modelname"]["value"]

    pagewidth = int(pagesize.split("x")[0])
    pageheight = int(pagesize.split("x")[1])
    image = Image.new(mode="RGB",size=(pagewidth,pageheight),color="white")
    page = ImageDraw.Draw(image)
    xspacing = 20
    yspacing = 30
    topmargin = 10
    xorigin = pagewidth/2
    yorigin = topmargin
    vmargin=1
    hmargin=2
    namefont=ImageFont.load_default()
    objects = data["objects"]
    levels = []
 
    def find(objects,property,value):
        result = []
        for name,values in objects.items():
            if property in values.keys() and values[property] == value:
                result.append(name)
        return result

    def bus(page,x,y,name,level=0):
        sz = page.multiline_textsize(name,font=namefont)
        x0 = x*xspacing - sz[0]/2 - hmargin + xorigin
        y0 = y*yspacing - sz[1]/2 - vmargin + yorigin
        x1 = x*xspacing + sz[0]/2 + hmargin + xorigin
        y1 = y*yspacing + sz[1]/2 + vmargin + yorigin
        swing = (objects[name]["bustype"] == "SWING")
        page.rectangle([x0,y0,x1+1,y1],outline="black",fill="white")
        page.multiline_text([x0+hmargin+1,y0+vmargin],name,font=namefont,fill="black")
        if swing:
            page.rectangle([x0-1,y0-1,x1+2,y1+1],outline="black")
        links = find(objects,"from",name)
        n = -(len(links)-1)/2
        for link in links:
            to = objects[link]["to"]
            print("level",level,"link:",link,"to",to)
            bus(page,n+x,y+1,to,level+1)
            page.line((x,y,x+n,y+yspacing-sz[1]))
            n += 1

    roots = find(objects,"bustype","SWING")
    for root in roots:
        print("root:",root)
        bus(page,0,0,root)
    image.save(filename_png)
