import pandas as pd
import re
import gridlabd

class Node:
	def __init__(self, name: str) -> None:
		self.name = name
		self.group_id = None
		node_object: dict = gridlabd.get_object(name)
		self.object = node_object
		self.component = node_object['class']
		self.length = node_object['length'] if 'length' in node_object else None
		self.to_node =  node_object['to'] if 'to' in node_object else None
		self.from_node =  node_object['from'] if 'from' in node_object else None
		self.latitude = node_object['latitude'] if 'latitude' in node_object else None
		self.longitude = node_object['longitude'] if 'longitude' in node_object else None


	def addGroupID(self, id:int) -> None:
		self.group_id = id
	
	def print(self) -> None:
		return {'group_id':self.group_id,'class':self.component}

class Graph:
	def __init__(self) -> None:
		self.objects: dict[str, Node] = {}
		self.adj_list: dict[str, list[str]] = {}
		self.nodes: list[str] = []
		self.connections: list[str] = []
		self.groups: dict[int,list[str]] = {}
		self.dependencies: dict[int,list[str]] = {}

	def addObject(self, node: str) -> None:
		if node not in self.objects:
			self.objects[node] = Node(node)
	
	def addConnection(self, node: str) -> None:
		if node not in self.connections:
			self.connections.append(node)
			
	def addNode(self, node: str) -> None:
		if node not in self.adj_list:
			self.nodes.append(node)
			self.adj_list[node] = []
	
	def addEdge(self, node_a: str, node_b: str) -> None:
		if node_a in self.adj_list and node_b in self.adj_list:
			if node_b not in self.adj_list[node_a]:
				self.adj_list[node_a].append(node_b)
			if node_a not in self.adj_list[node_b]:
				self.adj_list[node_b].append(node_a)


	def removeEdge(self, node_a: str, node_b: str) -> None:
		if node_b in self.adj_list[node_a]:
			self.adj_list[node_a].remove(node_b)
		if node_a in self.adj_list[node_b]:
			self.adj_list[node_b].remove(node_a)

	def removeAllEdges(self, target: str) -> None:
		adj_nodes = self.adj_list[target]
		for node in adj_nodes:
			self.adj_list[node].remove(target)
		self.adj_list[target] = []

	def print(self) -> None:
		nodes = self.adj_list
		for node in nodes:
			print(f"{node}: {self.nodes.get(node).group_id} --> {repr(self.adj_list.get(node))}")
	
	

	def bfs(self, start_node: str, switch_components: list[str]) -> None:
		visited: list[str] = []
		queue: list[str] = []
		groups: dict[str,list[str]] = {}
		
		def group_name(index:int):
			return f'Area_{index}'
		def switch_name():
			return f'switch'

		count = 1
		group_id = group_name(count)

		self.objects.get(start_node).addGroupID(group_id)
		queue.append(start_node)

		groups[switch_name()] = []
		if group_id not in groups:
			groups[group_id] = []
		groups[group_id].append(start_node)

		while len(queue) != 0:
			curr_node = queue.pop()
			curr_node_object = self.objects.get(curr_node)
			if curr_node not in visited:
				for next_node in self.adj_list[curr_node]:
					next_node_object = self.objects.get(next_node)

					# Check if current node is node in the network that is not a node to divide the network into groups
					if curr_node_object.component not in switch_components:
						# Check if next node is not a switch (i.e Node -> Node, node connected to another node)
						if next_node_object.component not in switch_components:
							# Check if node has already been assigned a group_id
							if next_node_object.group_id is None:
								next_node_object.addGroupID(curr_node_object.group_id)
								groups[curr_node_object.group_id].append(next_node)
							else:
								# Resolve group conflict from cycle in graph
								if curr_node_object.group_id != next_node_object.group_id:
									max_val = max(curr_node_object.group_id, next_node_object.group_id)
									min_val = min(curr_node_object.group_id, next_node_object.group_id)

									max_group = groups.get(max_val)
									for node in max_group:
										self.objects.get(node).addGroupID(min_val)
										groups.get(min_val).append(node)
									
									groups.pop(max_val,None)
						# Next node is a Switch (i.e. Node -> Switch, node connected to a switch)
						else:
							next_node_object.addGroupID(switch_name())
							if next_node not in groups[switch_name()]:
								groups[switch_name()].append(next_node)
					# Current node is a node used to divide the network into groups
					else:
						# If next node is not a switch then then create a new group (i.e. Switch -> Node, start of new group)
						if next_node_object.component not in switch_components:
							# Check if node has already been assigned a group_id
							if next_node_object.group_id is None:
								count += 1
								group_id = group_name(count)
								next_node_object.addGroupID(group_id)
								if group_id not in groups:
									groups[group_id] = []
								groups[group_id].append(next_node)
							else:
								# Node already has a group_id
								pass
						# Next node is also a node used to divide the network (i.e. Switch -> Switch, two switches connected to each other)
						else:
							# next_node_object.addGroupID(switch_name())
							if next_node not in groups[switch_name()]:
								groups[switch_name()].append(next_node)
					
					queue = [next_node] + queue
				visited.append(curr_node)


		
		# Fix group_ids for nodes in each group
		group_id_count = 0
		for group, items in groups.items():
			group_label = ''
			if group_id_count == 0:
				group_label = switch_name()
			else:
				group_label = group_name(group_id_count)
			
			for item in items:
				item_object = self.objects[item]
				item_object.addGroupID(group_label)
					
				if group_label not in self.groups:
					self.groups[group_label] = []
				self.groups[group_label].append(item)

				# print(f'{item_object.group_id} : {group} - {item}')
			group_id_count += 1
				
		# Add group_id to lines connecting nodes in each group
		for link in self.connections:
			link_object = self.objects[link]
			link_to_object = link_object.to_node
			link_from_object = link_object.from_node

			link_to_group_id = self.objects[link_to_object].group_id
			link_from_group_id = self.objects[link_from_object].group_id

			if link_to_group_id == link_from_group_id:
				link_object.group_id = link_to_group_id
			else:
				print(link_object.name,link_from_group_id, link_to_group_id )
				raise Exception('Unable to assign group_id to _line, each end is connected to a different group') 

		# Create dependencies between groups in the network
		switches = self.groups.get(switch_name())
		for switch in switches:
			switch_object: dict = gridlabd.get_object(switch)
			to_node = switch_object['to']
			from_node = switch_object['from']
			
			to_node_object = self.objects.get(to_node)
			from_node_object = self.objects.get(from_node)

			to_node_group = to_node_object.group_id
			from_node_group = from_node_object.group_id

			if to_node_group not in self.dependencies:
				self.dependencies[to_node_group] = []
			
			if from_node_group not in self.dependencies:
				self.dependencies[from_node_group] = []

			if from_node_group not in self.dependencies[to_node_group]:
				self.dependencies[to_node_group].append(from_node_group)
			if to_node_group not in self.dependencies[from_node_group]:
				self.dependencies[from_node_group].append(to_node_group)
			
	def get_dependency_df(self) -> pd.DataFrame:
		dep_length = max([len(x) for x in self.dependencies.values()])

		columns = [f'dep{x + 1}' for x in range(dep_length)]
		dep_df = pd.DataFrame.from_dict(self.dependencies,orient='index',columns=columns).sort_index()
		dep_df.reset_index(inplace=True)
		dep_df.rename(columns={'index':'group_id'}, inplace=True)
		dep_df.fillna(0, inplace=True)
		return dep_df
	
	def get_network_df(self) -> pd.DataFrame:
		node_list = []
		for node_name, node_extra in self.objects.items():	
			
			node_info: dict = gridlabd.get_object(node_name)
			node_info['group_id'] = node_extra.group_id
			node_info['latitude'] = node_extra.latitude if node_extra.latitude else 37.315
			node_info['longitude'] = node_extra.longitude if node_extra.longitude else -122.2056
			if node_extra.length:
				lengthVal = re.findall(r'\d+', node_extra.length)[0]
				node_info['length'] = int(lengthVal)
			else:
				node_info['length'] = None

			node_list.append(node_info)

		df = pd.DataFrame.from_records(node_list)
		df = df[['group_id','name','class','latitude','longitude','length']]
		print(df)
		return df


def contruct_graph(objects: list[str], connection_components: list[str]) -> Graph:
	graph = Graph() 
	for object_name in objects:
		graph.addObject(object_name)

		object: dict = gridlabd.get_object(object_name)
		a = 'to' in object
		b = 'from' in object
		if a and b:
			object_to = object['to']
			object_from = object['from']
			object_class = object['class']

			graph.addObject(object_to)
			graph.addObject(object_from)

			graph.addNode(object_to)
			graph.addNode(object_from)

			if object_class in connection_components:
				graph.addConnection(object_name)
				graph.addEdge(object_from, object_to)
			else:
				graph.addNode(object_name)
				graph.addEdge(object_name, object_from)
				graph.addEdge(object_name, object_to)
			
	return graph
