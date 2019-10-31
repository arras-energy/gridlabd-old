import json 
import os 
import sys, getopt
from datetime import datetime 

def help():
	print('Syntax:')
	print('json2png.py -i|--ifile <modelinputfile> [-o|--ofile <outputfile>] [-t|--type <outputtype> [<options> ...]')
	print('  -i|--ifile     : [REQUIRED] json input file name.')
	print('  -o|--ofile     : [OPTIONAL] png output file name.')
	print('  -t|--type      : [OPTIONAL] specify output type')
	print('Output types')
	print('  summary                      : [DEFAULT] output a summary of model')
	print('    -s|--size <width>x<height> : [OPTIONAL] image size in pixels')
	print('  profile                      : output the voltage profile')
	print('    -d|--dpi <resolution>      : [OPTIONAL] image resolution in dots per inch')
	print('    -l|--limit <percent>       : [OPTIONAL] voltage range limit in percent')
	print('    --with-nodes               : [OPTIONAL] label branching nodes')

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
	elif opt in ("-t","--type"):
		output_type = arg
	elif opt == '--with-nodes':
		with_nodes = True
	elif opt in ("-d","--dpi"):
		resolution = arg
	elif opt in ("-s","--size"):
		size = arg
	elif opt in ("-l","--limit"):
		limit = int(arg)/100
	else:
		raise Exception("'%s' is an invalid command line option" % opt)

with open(filename_json,"r") as f :
	data = json.load(f)
	assert(data['application']=='gridlabd')
	assert(data['version'] >= '4.2.0')

#
# -t summary
#
if output_type == 'summary':

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

#
# -t profile
#
elif output_type == 'profile':

	import matplotlib.pyplot as plt
	plt.figure(1);

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
		va = abs(get_complex(values,"voltage_A"))/vn
		vb = abs(get_complex(values,"voltage_B"))/vn
		vc = abs(get_complex(values,"voltage_C"))/vn
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
					if va1>1+limit or vb1>1+limit or vc1>1+limit : 
						print("json2png.py WARNING: node %s voltage is high (%g, %g, %g), phases = '%s', nominal voltage=%g" % (to,va1*vn1,vb1*vn1,vc1*vn1,ph1,vn1));
					if (va1>0 and va1<1-limit) or (vb1>0 and vb1<1-limit) or (vc1>0 and vc1<1-limit) : 
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

else:
	modname = sys.argv[0].replace("json2png.py","json2png-%s.py"%output_type)
	if os.path.exists(modname):

		import importlib, copy
		modspec = importlib.util.spec_from_file_location(output_type, modname)
		mod = importlib.import_module("json2png-%s"%output_type)
		argv = copy.deepcopy(sys.argv)
		argv[0] = modname
		mod.main(argv)

	else:

		raise Exception("type '%s' is not valid" % output_type)

### oneline method
# from PIL import Image, ImageDraw, ImageFont
# im = Image.new(mode="RGB",size=(600,400),color="white")
# draw = ImageDraw.Draw(im)
# fnt = ImageFont.load_default()

# def node(x,y,label):
# 	sz = draw.multiline_textsize(label,font=fnt)
# 	draw.rectangle([x-sz[0]/2-1,y-sz[1]/2-1,x+sz[0]/2+1,y+sz[1]/2+1],outline="black",fill="white")
# 	draw.multiline_text((x-sz[0]/2,y-sz[1]/2),label,font=fnt,fill="black")
# node(320,200,"test")
# draw.line((0,0,640,400),fill="black")
# draw.line((0,400,640,0),fill="black")
# im.save(filename_png)

### general graphing method
# import networkx as nx
# G = nx.DiGraph()
# for name, properties in data["objects"].items():
# 	keys = properties.keys();
# 	if "from" in keys and "to" in keys:
# 		f = properties["from"];
# 		t = properties["to"];
# 		G.add_nodes_from([f,t],weight=0);
# 		if "power_in" in keys and "power_out" in keys:
# 			i = properties["power_in"]
# 			o = properties["power_out"]
# 			p = abs(complex(max(i,o).split(" ")[0]))
# 			if i > o:
# 				G.add_edge(f,t,weight=p)
# 			else:
# 				G.add_edge(t,f,weight=p)
# 		else:
# 			G.add_edge(f,t,weight=0);
# #H = nx.DiGraph()
# print("Graph nodes:",G.number_of_nodes())
# print("Graph edges:",G.number_of_edges())

# import matplotlib.pyplot as plt

# plt.figure(1);
# H = nx.planar_layout(G)
# nx.draw_networkx(G, H, node_size=2, with_labels=False, font_size=6, font_color='b', label=basename)
# plt.tight_layout()
# plt.savefig(filename_png, dpi=1000)
