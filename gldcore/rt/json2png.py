import json 
import os 
import sys, getopt
from datetime import datetime 

filename_json = ''
filename_png = ''
basename = ''
output_type = 'summary'
try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:",["ifile=","ofile=","type="])
except getopt.GetoptError:
	sys.exit(2)
if not opts : 
	print('Syntax:')
	print('json2png.py -i|--ifile <modelinputfile> [-o|--ofile <outputfile>] [-t|--type <outputtype>]')
	print('-i|--ifile : [REQUIRED] json input file name.')
	print('-o|--ofile : [OPTIONAL] png output file name.')
	print("-t|--type")
for opt, arg in opts:
	if opt in ("-h","--help"):
		sys.exit()
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
	img = Image.new(mode="RGB",size=(320,200),color="white")
	draw = ImageDraw.Draw(img)

	def node(draw,x,y,text,vmargin=1,hmargin=1,fnt=ImageFont.load_default()):
		sz = draw.multiline_textsize(text,font=fnt)
		draw.rectangle([x-sz[0]/2-hmargin,y-sz[1]/2-vmargin,x+sz[0]/2+hmargin,y+sz[1]/2+vmargin],outline="black",fill="white")
		draw.multiline_text((x-sz[0]/2,y-sz[1]/2),text,font=fnt,fill="black")

	import hashlib
	md5 = hashlib.md5()
	with open(filename,"r") as f:
		md5.update(f.read().encode())
	node(draw,x=160,y=100,text="""Name..... %s
Digest... %s
Date..... %s""" % (filename,md5.hexdigest(),datetime.now().strftime("%y-%m-%d %H:%M:%S")),vmargin=2,hmargin=3)
	img.save(filename_png)

#
# -t profile
#
elif output_type == 'profile':

	def find(objects,property,value):
		result = []
		for name,values in objects.items():
			if property in values.keys() and values[property] == value:
				result.append(name)
		return result

	def profile(objects,root,pos=0):
		result = []
		for obj in find(objects,"from",root):
			branch = []
			line = objects[obj]
			keys = line.keys()
			if "to" in keys:
				to = line["to"]
				if "length" in keys:
					ln = abs(complex(line["length"].split(" ")[0]))
				else:
					ln = 0
				values = objects[to]
				keys = values.keys()
				if "voltage_A" in keys and "voltage_B" in keys and "voltage_C" in keys and "nominal_voltage" in keys:
					Vn = abs(complex(values["nominal_voltage"].split(" ")[0]))
					Va = abs(complex(values["voltage_A"].split(" ")[0].replace('i','j')))
					Vb = abs(complex(values["voltage_B"].split(" ")[0].replace('i','j')))
					Vc = abs(complex(values["voltage_C"].split(" ")[0].replace('i','j')))
					#print("%s@%g: (%g, %g, %g)" % (obj,pos+ln,Va/Vn,Vb/Vn,Vc/Vn))
					branch.append([pos+ln,Va/Vn,Vb/Vn,Vc/Vn])
				tail = profile(objects,to,pos+ln)
				if tail:
					branch.extend(profile(objects,to,pos+ln))
			result.extend(branch)
		return result

	import matplotlib.pyplot as plt
	plt.figure(1);
	ln = []
	va = []
	vb = []
	vc = []
	for obj in find(objects=data["objects"],property="bustype",value="SWING"):
		for p in profile(objects=data["objects"],root=obj):
			ln.append(p[0]/5280)
			va.append(p[1])
			vb.append(p[2])
			vc.append(p[3])
		plt.plot(ln,va,'k')
		plt.plot(ln,vb,'r')
		plt.plot(ln,vc,'b')
	plt.xlabel('Distance (miles)')
	plt.ylabel('Voltage (pu)')
	plt.title(data["globals"]["modelname"]["value"])
	plt.savefig(filename_png, dpi=1000)

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
