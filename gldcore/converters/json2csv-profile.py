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
        
	def find(objects,property,value):
		result = []
		for name,values in objects.items():
			if property in values.keys() and values[property] == value:
				result.append(name)
		return result

	def get_string(values,prop):
		return values[prop]

	def get_complex(values,prop):
		return complex(get_string(values,prop).split(" ")[0].replace('i','j'))

	def get_real(values,prop):
		return get_complex(values,prop).real

	def get_voltages(values):
		ph = get_string(values,"phases")
		vn = abs(get_complex(values,"nominal_voltage"))
		result = []
		try:
			va = abs(get_complex(values,"voltage_A"))/vn
		except:
			va = None
		try:
			vb = abs(get_complex(values,"voltage_B"))/vn
		except:
			vb = None
		try:
			vc = abs(get_complex(values,"voltage_C"))/vn
		except:
			vc = None
		return ph,vn,va,vb,vc

	def profile(objects,root,pos=0):
		fromdata = objects[root]
		ph0,vn0,va0,vb0,vc0 = get_voltages(fromdata)

		count = 0
		for link in find(objects,"from",root):
			linkdata = objects[link]
			linktype = "-"
			if "length" in linkdata.keys():
				linklen = get_real(linkdata,"length")/5280
			else:
				linklen = 0.0
			if not "line" in get_string(linkdata,"class"):
				linktype = "--o"
			if "to" in linkdata.keys():
				to = linkdata["to"]
				todata = objects[to]
				ph1,vn1,va1,vb1,vc1 = get_voltages(todata)
				profile(objects,to,pos+linklen)
				count += 1
				if "A" in ph0 and "A" in ph1: plt.plot([pos,pos+linklen],[va0,va1],"%sk"%linktype)
				if "B" in ph0 and "B" in ph1: plt.plot([pos,pos+linklen],[vb0,vb1],"%sr"%linktype)
				if "C" in ph0 and "C" in ph1: plt.plot([pos,pos+linklen],[vc0,vc1],"%sb"%linktype)
				if limit:
					if (not va1 is None and va1>1+limit) or (not vb1 is None and vb1>1+limit) or (not vc1 is None and vc1>1+limit) : 
						print("json2png.py WARNING: node %s voltage is high (%g, %g, %g), phases = '%s', nominal voltage=%g" % (to,va1*vn1,vb1*vn1,vc1*vn1,ph1,vn1));
					if (not va1 is None and va1<1-limit) or (not vb1 is None and vb1<1-limit) or (not vc1 is None and vc1<1-limit) : 
						print("json2png.py WARNING: node %s voltage is low (%g, %g, %g), phases = '%s', nominal voltage=%g" % (to,va1*vn1,vb1*vn1,vc1*vn1,ph1,vn1));
		if count > 1 and with_nodes:
			plt.plot([pos,pos,pos],[va0,vb0,vc0],':*',color='grey',linewidth=1)
			plt.text(pos,min([va0,vb0,vc0]),"[%s]  "%root,color='grey',size=6,rotation=90,verticalalignment='top',horizontalalignment='center')

	for obj in find(objects=data["objects"],property="bustype",value="SWING"):
		profile(objects=data["objects"],root=obj)
	plt.xlabel('Distance (miles)')
	plt.ylabel('Voltage (pu)')
	plt.title(data["globals"]["modelname"]["value"])
	plt.grid()
	#plt.legend(["A","B","C"])
	#plt.tight_layout()
	if limit:
		plt.ylim([1-limit,1+limit])
	plt.savefig(filename_png, dpi=int(resolution))