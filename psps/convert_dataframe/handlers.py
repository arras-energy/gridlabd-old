""" python/examples/example_2/handlers.py

This module illustrates how to implement handlers for a gridlabd model using a
'on_init' and 'commit' handlers to share collected data with the  main python
module. """

import numpy as np 
import matplotlib.pyplot as plt
import pandas as pd
import gridlabd

class AdjList:
	def __init__(self) -> None:
		self.adj_list = {}

	def addNode(self, node):
		if node not in self.adj_list:
			self.adj_list[node] = []
		else:
			pass
			# print(f"{node} is already in list")

	def addEdge(self, node_a, node_b):
		if node_a in self.adj_list and node_b in self.adj_list:
			if node_b not in self.adj_list[node_a]:
				self.adj_list[node_a].append(node_b)
			if node_a not in self.adj_list[node_b]:
				self.adj_list[node_b].append(node_a)

	def graph(self):
		nodes = self.adj_list.keys()
		for node in nodes:
			print(f"{node} --> {repr(self.adj_list.get(node))}")



def on_init(t):
	print(f'${t}: on_init')
	globals = gridlabd.get('globals')
	modules = gridlabd.get('modules')
	classes = gridlabd.get('classes')
	objects = gridlabd.get('objects')

	adj_list = AdjList()

	for object_name in objects:
		try:
			object = gridlabd.get_object(object_name)
			object_to = object['to']
			object_from = object['from']
			if object_to is not None and object_from is not None:
				# allowed_classes = ['overhead_line','switch']
				object_class = object['class']

				if object_class == 'overhead_line':
					# print(object_name,object_class,object_to,object_from)
					adj_list.addNode(object_to)
					adj_list.addNode(object_from)
					adj_list.addEdge(object_from, object_to)
				else:
				# elif object_class == 'switch':
					# print(object_name,object_class,object_to,object_from)
					adj_list.addNode(object_name)
					adj_list.addNode(object_to)
					adj_list.addNode(object_from)
					adj_list.addEdge(object_name, object_from)
					adj_list.addEdge(object_name, object_to)
				# else:
				# 	print(object_name,object_class,object_to,object_from)
		except:
			pass
	
	adj_list.graph()
	
	return False

def on_precommit(t):
	print(f'${t}: on_precommit')
	global df
	df = pd.DataFrame()
	return t

def on_presync(t):
	print(f'${t}: on_presync')
	return t + 1


def on_sync(t):
	print(f'${t}: on_sync')
	return t + 1

def on_postsync(t):
	print(f'${t}: on_postsync')
	return t + 1

def on_commit(t):
	print(f'${t}: on_commit')
	global df
	print(df)
	return t

def on_term(t):
	print(f'${t}: on_term')

df = pd.DataFrame()

def commit(obj,t):
	
	values = gridlabd.get_object(obj)
	global df
	temp_df = pd.DataFrame([values])
	df = pd.concat([df,temp_df])
	return True

def create(obj, t):
	values = gridlabd.get_object(obj)
	print(values)
	return True




