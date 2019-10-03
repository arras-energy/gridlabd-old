import json 
import os 
import sys, getopt

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

if output_type == 'summary':

	from PIL import Image, ImageDraw, ImageFont
	im = Image.new(mode="RGB",size=(320,200),color="white")
	draw = ImageDraw.Draw(im)
	fnt = ImageFont.load_default()

	def node(x,y,text,vmargin=1,hmargin=1):
		sz = draw.multiline_textsize(text,font=fnt)
		draw.rectangle([x-sz[0]/2-hmargin,y-sz[1]/2-vmargin,x+sz[0]/2+hmargin,y+sz[1]/2+vmargin],outline="black",fill="white")
		draw.multiline_text((x-sz[0]/2,y-sz[1]/2),text,font=fnt,fill="black")
	node(x=160,y=100,text=data["globals"]["modelname"]["value"],vmargin=2,hmargin=3)
	im.save(filename_png)

elif output_type == 'profile':

	def find(objects,property,value):
		result = []
		for name,values in objects.items():
			if property in values.keys() and values[property] == value:
				return result.append(name)
		return result
	def plot(objects,root,pos=0):
		for obj in find(objects,"from",root):
			to = objects[obj]["to"]
			values = objects[to]
			if "voltage_A" in values.keys():
				print("%s: (%s, %s, %s)" % (obj,objects[obj]["voltage_A"],objects[obj]["voltage_B"],objects[obj]["voltage_C"]))
			plot(objects,to)
	for obj in find(data["objects"],"bustype","SWING"):
		plot(data["objects"],obj)

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
