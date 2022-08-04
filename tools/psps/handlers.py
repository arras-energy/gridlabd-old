""" python/examples/example_2/handlers.py

This module illustrates how to implement handlers for a gridlabd model using a
'on_init' and 'commit' handlers to share collected data with the  main python
module. """
# from typing import Dict

from datetime import datetime, timedelta
import numpy as np 
import matplotlib.pyplot as plt
import pandas as pd
import gridlabd
from psps import display_results, optimize_psps
from graph import Graph, contruct_graph

# Initialize variables for PSPS
graph: Graph
df: pd.DataFrame
dep_df: pd.DataFrame


def on_init(t: int) -> bool:
	print(f'{t}: on_init')
	globals: list[str] = gridlabd.get('globals')
	modules: list[str] = gridlabd.get('modules')
	classes: list[str]  = gridlabd.get('classes')
	objects: list[str] = gridlabd.get('objects')

	connection_components = ['overhead_line', 'underground_line']
	switch_components = ['switch', 'recloser', 'sectionalizer']

	# Contrust Network representation
	global graph 
	graph = contruct_graph(objects, connection_components)
	start_node = graph.nodes[0]
	graph.bfs(start_node, switch_components)

	# Build dataframes for PSPS input
	global df
	df = graph.get_network_df()

	global dep_df
	dep_df = graph.get_dependency_df()
	print(dep_df)
	
	# Prepare data frames
	df = df.rename(columns={'latitude':'lat','longitude':'long'})
	df = df[df['group_id'].str.contains('switch') == False]
	return True

def on_precommit(t: int) -> int:
	print(f'{t}: on_precommit')
	use_weather_forecast = False
	data_start_date = datetime.fromtimestamp(t)
	print(data_start_date)
	# data_start_date = datetime(year=2021,month=10,day=15,hour=0)
	firerisk_alpha = 80

	init_df = dep_df[['group_id']]
	init_df = init_df.assign(status=1)


	model = optimize_psps(df, dep_df, init_df, use_weather_forecast, data_start_date, firerisk_alpha)
	# #############
    # Plot Results
    # #############
	results = pd.DataFrame(columns={'timestep','group_id','status'})
	for timestep in model.timestep:
		for group_id in model.areas:
			results = results.append(
				{
					'timestep': int(timestep),
					'group_id': group_id,
					'status': int(model.switch[timestep, group_id].value)}
					,ignore_index=True
				)
	# areaDataX['results'] = xr.DataArray(results, dims=['time', 'group_id'])
	print(results)
	
	# Temporary stop for debugging purpose
	raise Exception('Stop after one itteration')
	return t

def on_presync(t: int) -> int:
	print(f'{t}: on_presync')
	return t + 60*60


def on_sync(t: int) -> int:
	print(f'{t}: on_sync')
	return t + 60*60

def on_postsync(t: int) -> int:
	print(f'{t}: on_postsync')
	return t + 60*60

def on_commit(t: int) -> int:
	print(f'{t}: on_commit')
	return t

def on_term(t: int) -> None:
	print(f'{t}: on_term')





