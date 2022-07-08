""" python/examples/example_2/handlers.py

This module illustrates how to implement handlers for a gridlabd model using a
'on_init' and 'commit' handlers to share collected data with the  main python
module. """
# from typing import Dict

import numpy as np 
import matplotlib.pyplot as plt
import pandas as pd
import gridlabd

class Node:
	def __init__(self, name: str) -> None:
		self.name = name
		self.group_id = None
		c_name: object = gridlabd.get_object(name)
		
		self.component = c_name['class']
	
	def addGroupID(self, id:int):
		self.group_id = id
	
	def print(self):
		return {'group_id':self.group_id,'class':self.component}

class Graph:
	def __init__(self) -> None:
		self.adj_list: dict[str, list[str]] = {}
		self.nodes: dict[str, Node] = {}

	def addNode(self, node: str):
		if node not in self.adj_list:
			self.nodes[node] = Node(node)
			self.adj_list[node] = []
		else:
			pass
			# print(f"{node} is already in list")

	def addEdge(self, node_a: str, node_b: str):
		if node_a in self.adj_list and node_b in self.adj_list:
			if node_b not in self.adj_list[node_a]:
				self.adj_list[node_a].append(node_b)
			if node_a not in self.adj_list[node_b]:
				self.adj_list[node_b].append(node_a)


	def removeEdge(self, node_a: str, node_b: str):
		if node_b in self.adj_list[node_a]:
			self.adj_list[node_a].remove(node_b)
		if node_a in self.adj_list[node_b]:
			self.adj_list[node_b].remove(node_a)

	def removeAllEdges(self, target: str):
		adj_nodes = self.adj_list[target]
		for node in adj_nodes:
			self.adj_list[node].remove(target)
		self.adj_list[target] = []

	def print(self):
		nodes = self.adj_list.keys()
		for node in nodes:
			print(f"{node}: {self.nodes.get(node).group_id} --> {repr(self.adj_list.get(node))}")
	
	

	def bfs(self, start_node: str):
		visited: list[str] = []
		queue: list[str] = []
		groups: dict[int,list[str]] = {}
		dependencies: dict[int,list[str]] = {}
		group_id = 1

		self.nodes.get(start_node).addGroupID(group_id)
		queue.append(start_node)

		groups[0] = []
		if group_id not in groups.keys():
			groups[group_id] = []
		groups[group_id].append(start_node)


		while len(queue) != 0:
			curr_node = queue.pop()
			curr_node_object = self.nodes.get(curr_node)
			if curr_node not in visited:
				for next_node in self.adj_list[curr_node]:
					next_node_object = self.nodes.get(next_node)

					if curr_node_object.component != 'switch':
						if next_node_object.component != 'switch':
							if next_node_object.group_id is None:
								next_node_object.addGroupID(curr_node_object.group_id)
								groups[curr_node_object.group_id].append(next_node)
							else:
								# Resolve group conflict from cycle in graph
								if curr_node_object.group_id != next_node_object.group_id:
									print('conflict')
									print(f"curr: {curr_node_object.name}:{curr_node_object.group_id} - next: {next_node_object.name}:{next_node_object.group_id}")
									print(f"curr: {repr(groups.get(curr_node_object.group_id))}")
									print(f"next: {repr(groups.get(next_node_object.group_id))}")
									max_val = max(curr_node_object.group_id, next_node_object.group_id)
									min_val = min(curr_node_object.group_id, next_node_object.group_id)

									max_group = groups.get(max_val)
									for node in max_group:
										self.nodes.get(node).addGroupID(min_val)
										groups.get(min_val).append(node)
									groups.pop(max_val,None)
								pass
						else:
							next_node_object.addGroupID(0)
							groups[0].append(next_node)
					else:
						if next_node_object.component != 'switch':
							if next_node_object.group_id is None:
								group_id += 1
								next_node_object.addGroupID(group_id)
								if group_id not in groups.keys():
									groups[group_id] = []
								groups[group_id].append(next_node)
							else:
								# Switch node to substation node that already has an ID assigned 
								pass
						else:
							next_node_object.addGroupID(0)
							groups[0].append(next_node)
					
					queue = [next_node] + queue
				visited.append(curr_node)

		switches = groups.get(0)
		for switch in switches:
			switch_object = gridlabd.get_object(switch)
			to_node = switch_object['to']
			from_node = switch_object['from']
			
			to_node_object = self.nodes.get(to_node)
			from_node_object = self.nodes.get(from_node)

			to_node_group = to_node_object.group_id
			from_node_group = from_node_object.group_id

			if to_node_group not in dependencies:
				dependencies[to_node_group] = []
			
			if from_node_group not in dependencies:
				dependencies[from_node_group] = []

			if from_node_group not in dependencies[to_node_group]:
				dependencies[to_node_group].append(from_node_group)
			if to_node_group not in dependencies[from_node_group]:
				dependencies[from_node_group].append(to_node_group)
			




		group_labels = groups.keys()
		for group in group_labels:
			print(f"{group}: --> {repr(groups.get(group))}")
		
		for k in sorted(dependencies):
			print(f"{k}: --> {repr(dependencies.get(k))}")



def on_init(t):
	print(f'${t}: on_init')
	globals = gridlabd.get('globals')
	modules = gridlabd.get('modules')
	classes = gridlabd.get('classes')
	objects = gridlabd.get('objects')

	graph = Graph()

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
					graph.addNode(object_to)
					graph.addNode(object_from)
					graph.addEdge(object_from, object_to)
				else:
				# elif object_class == 'switch':
					# print(object_name,object_class,object_to,object_from)
					graph.addNode(object_name)
					graph.addNode(object_to)
					graph.addNode(object_from)
					graph.addEdge(object_name, object_from)
					graph.addEdge(object_name, object_to)
				# else:
				# 	print(object_name,object_class,object_to,object_from)
		except:
			pass
	
	graph.bfs('ND_650')
	# graph.print()
	# adj_list.removeAllEdges('SW_LK_L671_692_SW_1')
	# adj_list.removeEdge('ND_633','ND_632')
	# adj_list.graph()
	
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




