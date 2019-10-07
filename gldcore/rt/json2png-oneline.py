import json 
import os 
import sys, getopt
from datetime import datetime 

def help():
    print('Syntax:')
    print('json2png-oneline.py -i|--ifile <input-name> [-o|--ofile <output-name>] [<options> ...]')
    print('  -i|--ifile                 : [REQUIRED] json input file name.')
    print('  -o|--ofile                 : [OPTIONAL] png output file name (default is <input-name>.png)')
    print('  -s|--size <width>x<height> : [OPTIONAL] image size in pixels (default is "%s")' % size)
    print('  -d|--dpi <resolution>      : [OPTIONAL] image resolution in dots per inch (default is "%s")' % resolution)

def main(argv):
    filename_json = ''
    filename_png = ''
    basename = ''
    output_type = 'summary'
    with_nodes = False
    resolution = "300"
    size = "300x200"
    limit = None

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
            size = arg

    with open(filename_json,"r") as f :
        data = json.load(f)
        assert(data['application']=='gridlabd')
        assert(data['version'] >= '4.2.0')

    filename = data["globals"]["modelname"]["value"]
    from PIL import Image, ImageDraw, ImageFont
    sz = size.split("x")
    sx = int(sz[0])
    sy = int(sz[1])
    img = Image.new(mode="RGB",size=(sx,sy),color="white")
    draw = ImageDraw.Draw(img)

    def node(draw,x,y,text,vmargin=1,hmargin=1,fnt=ImageFont.load_default()):
        sz = draw.multiline_textsize(text,font=fnt)
        draw.rectangle([x-sz[0]/2-hmargin,y-sz[1]/2-vmargin,x+sz[0]/2+hmargin,y+sz[1]/2+vmargin],outline="black",fill="white")
        draw.multiline_text((x-sz[0]/2,y-sz[1]/2),text,font=fnt,fill="black")

    import hashlib
    md5 = hashlib.md5()
    with open(filename,"r") as f:
        md5.update(f.read().encode())
    node(draw,x=sx/2,y=sy/2,text="""Name..... %s
    Digest... %s
    Date..... %s""" % (filename,md5.hexdigest(),datetime.now().strftime("%y-%m-%d %H:%M:%S")),vmargin=2,hmargin=3)
    img.save(filename_png)
